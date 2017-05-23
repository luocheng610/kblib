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
* 函数名称  : thread::start
* 功能描述  : 创建线程，并根据参数决定是否立即运行线程
* 参　　数  : bool create_suspend       true:创建后挂起线程 false:立即运行线程
* 返 回 值  : void
*******************************************************************************/
void thread::start( bool create_suspend )
{
    if ( NULL == m_thread_handle )
    {
        ::ResetEvent( m_stop_event );
        m_thread_handle = CreateThread(      NULL,			    // 继承创建线程（或者进程）的属性
											 0,				    // 线程使用缺省的栈尺寸，为1M
											 (LPTHREAD_START_ROUTINE)thread_entry,	    // 线程创建后执行的函数
											 (void*)this,       // 执行函数的参数
                                             CREATE_SUSPENDED,  // 线程创建后是否立即执行
											 &m_thread_id);
        // 如果创建即运行线程，则当线程开始运行的时候，m_thread_id可能还未被赋值。
        // 这种情况下，调用 get_thread_handle 和 get_thread_id 都会不正确，
        // 所以必须先确保 m_thread_id 被赋值，再开始运行线程。
        if ( !create_suspend )
        {
            ::ResumeThread( m_thread_handle );
        }
	    assert( NULL != m_thread_handle );
    }
}

/*******************************************************************************
* 函数名称  : thread::stop
* 功能描述  : 停止执行线程并销毁。需要派生类在 thread_proc 中配合，一旦发现
              stop event 被置，则立刻退出线程函数执行。stop 的等待时间取决于
              thread_proc 的响应退出事件的时间。
* 返 回 值  : void
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
* 函数名称  : thread::suspend
* 功能描述  : 挂起线程，线程将停止运行
* 返 回 值  : void
*******************************************************************************/
void thread::suspend()
{
    if ( m_thread_handle )
    {
        ::SuspendThread( m_thread_handle );
    }
}

/*******************************************************************************
* 函数名称  : thread::resume
* 功能描述  : 让挂起的线程恢复运行
* 返 回 值  : void
*******************************************************************************/
void thread::resume()
{
    if ( m_thread_handle )
    {
        ::ResumeThread( m_thread_handle );
    }
}

/*******************************************************************************
* 函数名称  : thread::is_in_thread
* 功能描述  : 当前是否在线程中执行
* 返 回 值  : bool      true:表示当前在线程中 false:当前不在线程中
*******************************************************************************/
bool thread::is_in_thread()
{
    return m_thread_id == ::GetCurrentThreadId();
}

/*******************************************************************************
* 函数名称	: thread::get_thread_id
* 功能描述	: 获取该线程的id
* 返 回 值	: uint32
*******************************************************************************/
uint32 thread::get_thread_id()
{
	return m_thread_id;
}

/*******************************************************************************
* 函数名称  : thread::get_thread_handle
* 功能描述  : 获得线程的句柄
* 返 回 值  : HANDLE        线程的句柄
*******************************************************************************/
HANDLE thread::get_thread_handle()
{
    return m_thread_handle;
}

/*******************************************************************************
* 函数名称  : thread::get_stop_event
* 功能描述  : 获得线程停止事件的句柄
* 返 回 值  : HANDLE        线程停止事件的句柄
*******************************************************************************/
HANDLE thread::get_stop_event()
{
    return m_stop_event;
}

/*******************************************************************************
* 函数名称  : thread::is_stop_event_set
* 功能描述  : 查询线程停止事件是否被置位（非阻塞调用）
* 返 回 值  : bool          true:被置位 false：未置位
*******************************************************************************/
bool thread::is_stop_event_set()
{
    return WAIT_OBJECT_0 == ::WaitForSingleObject( m_stop_event, 0 );
}

/*******************************************************************************
* 函数名称	: thread::thread_entry
* 功能描述	: 线程启动的入口函数
* 参　　数	: void *thread_class
* 返 回 值	: void*
*******************************************************************************/
unsigned thread::thread_entry(void* param)
{
    thread* thread_ptr = (thread*)param;	
    thread_ptr->thread_proc();
    return 0;
}

KB_WIN32_NAMESPACE_END(multithread)
