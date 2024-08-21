#include "Thread.h"

ChildrenThread::ChildrenThread(wxFrame* frame) :wxThread(wxTHREAD_DETACHED)//分离线程
{
    m_frame = frame;
}
ChildrenThread::~ChildrenThread()
{

}
void* ChildrenThread::Entry()
{
    blog_WidgetsFrame* frame = wxDynamicCast(m_frame, blog_WidgetsFrame);
    if (frame)
    {
        //获取锁
        frame->mutex->Lock();
        wxThreadEvent evt1(wxEVT_COMMAND_THREAD, SIGN_1);
        evt1.SetString(wxT("子线程第一次执行的结果通知"));
        //发送信号，并等待主线程通知
        threadWait(evt1);

        wxThreadEvent evt2(wxEVT_COMMAND_THREAD, SIGN_1);
        evt2.SetString(wxT("子线程第二次执行的结果通知"));
        //直接发送信号，不在等待主线程
        frame->GetEventHandler()->QueueEvent(evt2.Clone());

        //最后记着释放锁
        frame->mutex->Unlock();
    }
    return NULL;
}
void ChildrenThread::threadWait(wxThreadEvent& event)
{
    blog_WidgetsFrame* frame = wxDynamicCast(m_frame, blog_WidgetsFrame);
    frame->GetEventHandler()->QueueEvent(event.Clone());
    frame->condition->Wait();

}
