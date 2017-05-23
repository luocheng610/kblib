#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "kb_lib/tchar.h"

#include "./mutex.h"

KB_WIN32_NAMESPACE_BEGIN(multithread)

/*******************************************************************************
* ��������  : mutex::mutex
* ��������  : ������ȱʡ���캯��������һ��ȫ�µĻ�����
* �� �� ֵ  : 
*******************************************************************************/
mutex::mutex()
{
    m_mutex = ::CreateMutex( NULL, FALSE, NULL );
    assert( m_mutex );
}

/*******************************************************************************
* ��������  : mutex::mutex
* ��������  : �������Ĺ��캯�������Դ�һ���Ѿ����ڵĻ����������ߴ���һ��ָ������
              �Ļ�����������������Ϸ����򴴽�һ��ȫ�µĻ�������
* �Ρ�����  : bool open_existing
* �Ρ�����  : const tchar* mutex_name
* �� �� ֵ  : 
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
* ��������  : mutex::~mutex
* ��������  : �������������ٻ���������
* �� �� ֵ  : 
*******************************************************************************/
mutex::~mutex()
{
    ::CloseHandle( m_mutex );
    m_mutex = NULL;
}

/*******************************************************************************
* ��������	: mutex::lock
* ��������	: ��û�������ͬһ���߳̿�������
* �� �� ֵ	: void
*******************************************************************************/
void mutex::lock()
{
    ::WaitForSingleObject( m_mutex, INFINITE );
}

/*******************************************************************************
* ��������	: mutex::unlock
* ��������	: �ͷŻ�����
* �� �� ֵ	: void
*******************************************************************************/
void mutex::unlock()
{
    ::ReleaseMutex( m_mutex );
}

/*******************************************************************************
* ��������	: mutex::try_lock
* ��������	: ���Ի�û�������������ܻ�û������������̲߳����������������			  
* �� �� ֵ	: bool	true��ʾ��û�������false��ʾδ��û�����
*******************************************************************************/
bool mutex::try_lock()
{
    return WAIT_OBJECT_0 == ::WaitForSingleObject( m_mutex, 0 );
}

KB_WIN32_NAMESPACE_END(multithread)
