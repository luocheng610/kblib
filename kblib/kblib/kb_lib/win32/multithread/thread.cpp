#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./thread.h"

KB_WIN32_NAMESPACE_BEGIN(multithread)

thread::thread()
{
    m_thread_id = 0;
    m_thread_handle = NULL;
    m_stop_event = ::CreateEvent( NULL, TRUE, FALSE, NULL );
}

thread::~thread()
{
    stop();
    if ( m_stop_event )
    {
        ::CloseHandle( m_stop_event );
        m_stop_event = NULL;
    }
}

/*******************************************************************************
* ��������  : thread::start
* ��������  : �����̣߳������ݲ��������Ƿ����������߳�
* �Ρ�����  : bool create_suspend       true:����������߳� false:���������߳�
* �� �� ֵ  : void
*******************************************************************************/
void thread::start( bool create_suspend )
{
    if ( NULL == m_thread_handle )
    {
        ::ResetEvent( m_stop_event );
        m_thread_handle = CreateThread(      NULL,			    // �̳д����̣߳����߽��̣�������
											 0,				    // �߳�ʹ��ȱʡ��ջ�ߴ磬Ϊ1M
											 (LPTHREAD_START_ROUTINE)thread_entry,	    // �̴߳�����ִ�еĺ���
											 (void*)this,       // ִ�к����Ĳ���
                                             CREATE_SUSPENDED,  // �̴߳������Ƿ�����ִ��
											 &m_thread_id);
        // ��������������̣߳����߳̿�ʼ���е�ʱ��m_thread_id���ܻ�δ����ֵ��
        // ��������£����� get_thread_handle �� get_thread_id ���᲻��ȷ��
        // ���Ա�����ȷ�� m_thread_id ����ֵ���ٿ�ʼ�����̡߳�
        if ( !create_suspend )
        {
            ::ResumeThread( m_thread_handle );
        }
	    assert( NULL != m_thread_handle );
    }
}

/*******************************************************************************
* ��������  : thread::stop
* ��������  : ִֹͣ���̲߳����١���Ҫ�������� thread_proc ����ϣ�һ������
              stop event ���ã��������˳��̺߳���ִ�С�stop �ĵȴ�ʱ��ȡ����
              thread_proc ����Ӧ�˳��¼���ʱ�䡣
* �� �� ֵ  : void
*******************************************************************************/
void thread::stop()
{
    if ( m_thread_handle )
    {
        ::SetEvent( m_stop_event );
        ::WaitForSingleObject( m_thread_handle, INFINITE );
        ::CloseHandle( m_thread_handle );
        m_thread_handle = NULL;
        m_thread_id = NULL;
    }
}

/*******************************************************************************
* ��������  : thread::suspend
* ��������  : �����̣߳��߳̽�ֹͣ����
* �� �� ֵ  : void
*******************************************************************************/
void thread::suspend()
{
    if ( m_thread_handle )
    {
        ::SuspendThread( m_thread_handle );
    }
}

/*******************************************************************************
* ��������  : thread::resume
* ��������  : �ù�����ָ̻߳�����
* �� �� ֵ  : void
*******************************************************************************/
void thread::resume()
{
    if ( m_thread_handle )
    {
        ::ResumeThread( m_thread_handle );
    }
}

/*******************************************************************************
* ��������  : thread::is_in_thread
* ��������  : ��ǰ�Ƿ����߳���ִ��
* �� �� ֵ  : bool      true:��ʾ��ǰ���߳��� false:��ǰ�����߳���
*******************************************************************************/
bool thread::is_in_thread()
{
    return m_thread_id == ::GetCurrentThreadId();
}

/*******************************************************************************
* ��������	: thread::get_thread_id
* ��������	: ��ȡ���̵߳�id
* �� �� ֵ	: uint32
*******************************************************************************/
uint32 thread::get_thread_id()
{
	return m_thread_id;
}

/*******************************************************************************
* ��������  : thread::get_thread_handle
* ��������  : ����̵߳ľ��
* �� �� ֵ  : HANDLE        �̵߳ľ��
*******************************************************************************/
HANDLE thread::get_thread_handle()
{
    return m_thread_handle;
}

/*******************************************************************************
* ��������  : thread::get_stop_event
* ��������  : ����߳�ֹͣ�¼��ľ��
* �� �� ֵ  : HANDLE        �߳�ֹͣ�¼��ľ��
*******************************************************************************/
HANDLE thread::get_stop_event()
{
    return m_stop_event;
}

/*******************************************************************************
* ��������  : thread::is_stop_event_set
* ��������  : ��ѯ�߳�ֹͣ�¼��Ƿ���λ�����������ã�
* �� �� ֵ  : bool          true:����λ false��δ��λ
*******************************************************************************/
bool thread::is_stop_event_set()
{
    return WAIT_OBJECT_0 == ::WaitForSingleObject( m_stop_event, 0 );
}

/*******************************************************************************
* ��������	: thread::thread_entry
* ��������	: �߳���������ں���
* �Ρ�����	: void *thread_class
* �� �� ֵ	: void*
*******************************************************************************/
unsigned thread::thread_entry(void* param)
{
    thread* thread_ptr = (thread*)param;	
    thread_ptr->thread_proc();
    return 0;
}

KB_WIN32_NAMESPACE_END(multithread)
