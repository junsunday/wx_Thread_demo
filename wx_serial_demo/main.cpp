#include <wx/wxprec.h>  
#ifndef WX_PRECOMP  
#include <wx/wx.h>  
#endif  
#include "wxGUI.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
using namespace cv;
using namespace std;

wxBitmap MatToWxBitmap(const Mat& mat,bool Grayimage );
wxBitmap MatToWxBitmap1(const Mat& mat);
Mat InvertColors(Mat image);

Mat srcImg;
list<Mat> frameSeq;//帧序列
VideoCapture cap;
mutex mutex_frame, mutex_process;
condition_variable cond_process;
vector<thread> mythreads;
bool stop = false;

//主窗口类
class Opencvframe : public MyFrame1
{
public:
	Opencvframe() : MyFrame1(NULL, wxID_ANY) {};

private:
	void OnButton(wxCommandEvent& event);
	void capture();
	void control();
	void Proce_gray();
	void Proce_contours();
	void Proce_HSV();
};

Opencvframe* frame;
class calcApp : public wxApp
{
public:
	virtual bool OnInit();
};

wxIMPLEMENT_APP(calcApp);

bool calcApp::OnInit()
{
	frame = new Opencvframe();
	frame->Show(true);
	return true;
}

void Opencvframe::OnButton(wxCommandEvent& event)
{
	bool state = m_toggleBtn->GetValue();
	if (state)
	{
		cap.set(CAP_PROP_FRAME_WIDTH, 480);
		cap.set(CAP_PROP_FRAME_HEIGHT, 360);
		cap.open(0);//0：笔记本自带摄像头 1：外部USB摄像头
		if (!cap.isOpened())
		{
			return;
		}

		m_toggleBtn->SetLabel("关闭摄像头");

		stop = false;
		mythreads.push_back(thread(&Opencvframe::capture, frame));
		mythreads.push_back(thread(&Opencvframe::control, frame));
		mythreads.push_back(thread(&Opencvframe::Proce_gray, frame));
		mythreads.push_back(thread(&Opencvframe::Proce_contours, frame));
		mythreads.push_back(thread(&Opencvframe::Proce_HSV, frame));
	}
	else
	{
		m_toggleBtn->SetLabel("打开摄像头");

		stop = true;
		cond_process.notify_all();
		for (auto& thread : mythreads)
		{
			thread.join();
		}
		cap.release();
		mythreads.clear();
	}
}


//摄像头捕获子线程
void Opencvframe::capture()
{
	Mat capframe;
	while (!stop)
	{
		cap >> capframe;
		mutex_frame.lock();//保护frameSeq
		frameSeq.push_front(capframe);
		mutex_frame.unlock();
		this_thread::sleep_for(chrono::milliseconds(100));//减慢拍照速度
	}
}

void Opencvframe::control()
{
	unsigned int i = 0;
	char temp[16];
	while (!stop)
	{
		if (!frameSeq.empty())
		{
			this_thread::sleep_for(chrono::milliseconds(10));
			mutex_frame.lock();//保护frameSeq
			mutex_process.lock();//保护frame
			srcImg = frameSeq.back();//队列中读数据
			sprintf(temp, "frame:%d", i++);//计数
			mutex_process.unlock();
			cond_process.notify_all();//激活所有处理线程
			
			frameSeq.pop_back();//队列中删除数据
			mutex_frame.unlock();
			Mat src = srcImg.clone();
			putText(src, temp, Point(10, 40), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(80), 2, 8);
			wxBitmap imwx = MatToWxBitmap(src,false);//Mat转wxBitmap
			wxClientDC dc(m_panelOrigin);
			dc.DrawBitmap(imwx, 0, 0);
		}
	}
	wxClientDC dc(m_panelOrigin);
	dc.Clear();
}

//灰度化处理子线程
void Opencvframe::Proce_gray()
{
	Mat grayImg;
	while (!stop)
	{
		unique_lock<mutex> locker_gray(mutex_process);
		cond_process.wait(locker_gray);//保护frame
		Mat src = srcImg.clone();
		locker_gray.unlock();//提前解锁

		cvtColor(src, grayImg, COLOR_RGB2GRAY);
		wxBitmap imwx = MatToWxBitmap(grayImg,true);//Mat转wxBitmap
		//不知道什么问题，MatToWxBitmap函数在灰度图上会报错，所以这里用另一种方法
		//wxImage GrayTowximage(grayImg.cols, grayImg.rows, grayImg.data, true);
		//wxBitmap imwx(GrayTowximage);
		wxClientDC dc(m_panelGray);
		dc.DrawBitmap(imwx, 0, 0);
	} 
	wxClientDC dc(m_panelGray);
	dc.Clear();
}

//轮廓查找子线程
void Opencvframe::Proce_contours()
{
	Mat contoursImg;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	while (!stop)
	{
		unique_lock<mutex> locker_contours(mutex_process);
		cond_process.wait(locker_contours);//保护frame
		Mat src = srcImg.clone();
		locker_contours.unlock();//提前解锁

		cvtColor(src, src, COLOR_RGB2GRAY);
		blur(src, src, Size(3, 3));
		Canny(src, src, 40, 100);
		findContours(src, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
		contoursImg = Mat::zeros(src.size(), CV_8UC3);
		for (int i = 0; i < contours.size(); i++)
		{
			Scalar color(rand() & 255, rand() & 255, rand() & 255);
			drawContours(contoursImg, contours, i, color, 2, 8);
		}

		wxBitmap imwx = MatToWxBitmap(contoursImg,false);//Mat转wxBitmap
		wxClientDC dc(m_panelContours);

		dc.DrawBitmap(imwx, 0, 0);
	}
	wxClientDC dc(m_panelContours);
	dc.Clear();
}

//HSV 颜色空间阈值
void Opencvframe::Proce_HSV()
{
	Mat hsvImg;
	while (!stop)
	{
		unique_lock<mutex> locker_hsv(mutex_process);
		cond_process.wait(locker_hsv);//保护frame
		Mat src = srcImg.clone();
		locker_hsv.unlock();//提前解锁

		Mat Invimag = InvertColors(src);
		//cvtColor(src, hsvImg, COLOR_RGB2HSV);
		//// 设置 HSV 阈值
		//int hMin = 0, sMin = 0, vMin = 0;
		//int hMax = 180, sMax = 200, vMax = 255;
		//Mat thresholdedImag;
		//inRange(hsvImg, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), thresholdedImag);
		//Mat ToBGRImag;
		//cvtColor(thresholdedImag, ToBGRImag, COLOR_HSV2BGR);
		wxBitmap imwx = MatToWxBitmap(Invimag,false);//Mat 转 wxBitmap
		///*wxImage ToWximage(ToBGRImag.cols, ToBGRImag.rows, false);
		//
		//for (int y = 0; y < ToBGRImag.rows; ++y)
		//{
		//	for (int x = 0; x < ToBGRImag.cols; ++x)
		//	{
		//		Vec3b bgr= ToBGRImag.at<Vec3b>(y, x);
		//		ToWximage.SetRGB(x, y, bgr[2], bgr[1], bgr[0]);
		//	}
		//}
  //      wxBitmap imwx(ToWximage);*/
		
		wxClientDC dc(m_panelHVS);
		dc.DrawBitmap(imwx, 0, 0);
	}
	wxClientDC dc(m_panelHVS);
	dc.Clear();
}

//Mat 转 wxBitmap
wxBitmap MatToWxBitmap(const Mat& mat,bool GrayImage)
{
	if (!GrayImage)
	{
		if (mat.empty()) {
			return wxBitmap();
		}

		Mat temp;
		if (mat.channels() == 3) {
			cvtColor(mat, temp, COLOR_BGR2RGB);
		}
		else if (mat.channels() == 4) {
			cvtColor(mat, temp, COLOR_BGRA2RGBA);
		}
		else {
			temp = mat;
		}

		wxImage image(temp.cols, temp.rows, temp.data, true);
		if (!image.IsOk()) {
			wxLogMessage("Failed to create wxImage from Mat");
			return wxBitmap();
		}
		return wxBitmap(image);
	}
	else
	{
		wxImage Grayimage(mat.cols, mat.rows, false);
		for (int y = 0; y < mat.rows; ++y) {
			for (int x = 0; x < mat.cols; ++x) {
				uchar grayValue = mat.at<uchar>(y, x);
				Grayimage.SetRGB(x, y, grayValue, grayValue, grayValue);
			}
		}

		return wxBitmap(Grayimage);
	}
}
wxBitmap MatToWxBitmap1(const Mat& mat)
{
	if (mat.empty()) {
		return wxBitmap();
	}

	Mat temp;
	if (mat.channels() == 3) {
		cvtColor(mat, temp, COLOR_BGR2RGB);
	}
	else if (mat.channels() == 4) {
		cvtColor(mat, temp, COLOR_BGRA2RGBA);
	}
	else {
		temp = mat;
	}

	wxImage image(temp.cols, temp.rows, temp.data, true);
	if (!image.IsOk()) {
		wxLogMessage("Failed to create wxImage from Mat");
		return wxBitmap();
	}
	return wxBitmap(image);
}
Mat InvertColors(Mat image) 
{
	Mat invertedImage;
	image.copyTo(invertedImage);

	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			for (int c = 0; c < image.channels(); ++c) {
				invertedImage.at<cv::Vec3b>(y, x)[c] = 255 - image.at<cv::Vec3b>(y, x)[c];
			}
		}
	}

	return invertedImage;
}

//对话框模式尝试失败
wxClientDC* setpanel(wxWindow* parent)
{
	parent->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer_top;
	bSizer_top = new wxBoxSizer(wxVERTICAL);

	wxPanel* m_panel_video  = new wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL); //
	bSizer_top->Add(m_panel_video, 1, wxEXPAND | wxALL, 5);

	wxClientDC* m_dc = new wxClientDC(m_panel_video);

	parent->SetSizer(bSizer_top);
	parent->Layout();

	parent->Centre(wxBOTH);
	return m_dc;
}