/*******************************************************************************
* ����ժҪ	: �߳�ʵ�ֵķ�װ
*******************************************************************************/
#ifndef __THREAD_H_KB__  
#define __THREAD_H_KB__  

#include "kb_lib/type.h"

KB_WIN32_NAMESPACE_BEGIN(multithread)

class thread
{	
public:
    thread();
    virtual ~thread();

    void            start( bool create_suspend = false );	
    void            stop();

    void            suspend();
    void            resume();

    bool            is_in_thread();

    uint32          get_thread_id();
    HANDLE          get_thread_handle(); // ������ֱ������ HANDLE δ���壬�����Լ��� cpp �� #include "thread.h" ֮ǰ�� #include <Windows.h>

    HANDLE          get_stop_event();
    bool            is_stop_event_set();
	
protected:
    virtual void    thread_proc() = 0;
	
private:
    static unsigned __stdcall thread_entry(void* param);

    DWORD           m_thread_id;			// �̵߳�ID
    HANDLE          m_thread_handle;		// �̵߳ľ��
    HANDLE          m_stop_event;           // ֹͣ�̵߳��¼�
};

KB_WIN32_NAMESPACE_END(multithread)

#endif // end of __THREAD_H_KB__  
