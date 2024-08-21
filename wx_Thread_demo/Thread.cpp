#include "Thread.h"

ChildrenThread::ChildrenThread(wxFrame* frame) :wxThread(wxTHREAD_DETACHED)//�����߳�
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
        //��ȡ��
        frame->mutex->Lock();
        wxThreadEvent evt1(wxEVT_COMMAND_THREAD, SIGN_1);
        evt1.SetString(wxT("���̵߳�һ��ִ�еĽ��֪ͨ"));
        //�����źţ����ȴ����߳�֪ͨ
        threadWait(evt1);

        wxThreadEvent evt2(wxEVT_COMMAND_THREAD, SIGN_1);
        evt2.SetString(wxT("���̵߳ڶ���ִ�еĽ��֪ͨ"));
        //ֱ�ӷ����źţ����ڵȴ����߳�
        frame->GetEventHandler()->QueueEvent(evt2.Clone());

        //�������ͷ���
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
