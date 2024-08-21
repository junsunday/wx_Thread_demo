#pragma once
#ifndef THREAD_H
#define THREAD_H

#include <wx/wx.h>
#include "serial.h"
class ChildrenThread : public wxThread
{
public:
    ChildrenThread(wxFrame* frame);
    virtual ~ChildrenThread();
    virtual void* Entry();
    void threadWait(wxThreadEvent& event);
protected:

private:
    wxFrame* m_frame;
};

#endif // CHILDRENTHREAD_H

