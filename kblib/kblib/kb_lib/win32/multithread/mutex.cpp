#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "kb_lib/tchar.h"

#include "./mutex.h"

KB_WIN32_NAMESPACE_BEGIN(multithread)

/*******************************************************************************
* 函数名称  : mutex::mutex
* 功能描述  : 互斥锁缺省构造函数，创建一个全新的互斥锁
* 返 回 值  : 
*******************************************************************************/
mutex::mutex()
{
    m_mutex = ::CreateMutex( NULL, FALSE, NULL );
    assert( m_mutex );
}

/*******************************************************************************
* 函数名称  : mutex::mutex
* 功能描述  : 互斥锁的构造函数，可以打开一个已经存在的互斥锁，或者创建一个指定名称
              的互斥锁。如果参数不合法，则创建一个全新的互斥锁。
* 参　　数  : bool open_existing
* 参　　数  : const tchar* mutex_name
* 返 回 值  : 
*******************************************************************************/
mutex::mutex( bool open_existing, const tchar* mutex_name )
{
    if ( open_existing && mutex_name && mutex_name[0] )
    {
        m_mutex = ::OpenMutex( MUTEX_ALL_ACCESS, FALSE, mutex_name );
    }
    else if ( mutex_name && mutex_name[0] )
    {
        m_mutex = ::CreateMutex( NULL, FALSE, mutex_name );
    }
    else
    {
        m_mutex = ::CreateMutex( NULL, FALSE, NULL );
    }
    assert( m_mutex );
}

/*******************************************************************************
* 函数名称  : mutex::~mutex
* 功能描述  : 析构函数，销毁互斥锁对象。
* 返 回 值  : 
*******************************************************************************/
mutex::~mutex()
{
    ::CloseHandle( m_mutex );
    m_mutex = NULL;
}

/*******************************************************************************
* 函数名称	: mutex::lock
* 功能描述	: 获得互斥锁，同一个线程可以重入
* 返 回 值	: void
*******************************************************************************/
void mutex::lock()
{
    ::WaitForSingleObject( m_mutex, INFINITE );
}

/*******************************************************************************
* 函数名称	: mutex::unlock
* 功能描述	: 释放互斥锁
* 返 回 值	: void
*******************************************************************************/
void mutex::unlock()
{
    ::ReleaseMutex( m_mutex );
}

/*******************************************************************************
* 函数名称	: mutex::try_lock
* 功能描述	: 尝试获得互斥锁，如果不能获得互斥锁，调用线程不会堵塞，立即返回			  
* 返 回 值	: bool	true表示获得互斥锁，false表示未获得互斥锁
*******************************************************************************/
bool mutex::try_lock()
{
    return WAIT_OBJECT_0 == ::WaitForSingleObject( m_mutex, 0 );
}

KB_WIN32_NAMESPACE_END(multithread)
