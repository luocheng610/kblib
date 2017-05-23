#include <Windows.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "kb_lib/tchar.h"

#include "./critical_section.h"

KB_WIN32_NAMESPACE_BEGIN(multithread)

critical_section::TryEnterCriticalSectionProc critical_section::m_fnTryEnterCriticalSection = NULL;

critical_section::critical_section()
{
    if ( NULL == m_fnTryEnterCriticalSection )
    {
        HINSTANCE hKernel32 = GetModuleHandle( TSTRING("kernel32.dll") );
        if ( hKernel32 )
        {
            m_fnTryEnterCriticalSection = (TryEnterCriticalSectionProc)GetProcAddress( hKernel32, "TryEnterCriticalSection" );
        }
    }
	::InitializeCriticalSection(&m_cs);
}

critical_section::~critical_section()
{
    ::DeleteCriticalSection(&m_cs);
}

/*******************************************************************************
* 函数名称	: critical_section::lock
* 功能描述	: 获得临界区，同一个线程内可以重入
* 返 回 值	: void
*******************************************************************************/
void critical_section::lock()
{
	::EnterCriticalSection(&m_cs);
}

/*******************************************************************************
* 函数名称	: critical_section::unlock
* 功能描述	: 释放临界区
* 返 回 值	: void
* 作　　者	: 杨晓虎修订
* 设计日期	: 2008年9月11日
* 修改日期		   修改人		  修改内容
*******************************************************************************/
void critical_section::unlock()
{
    ::LeaveCriticalSection(&m_cs);
}

/*******************************************************************************
* 函数名称	: critical_section::try_lock
* 功能描述	: 尝试获得临界区，如果不能获得临界区，调用线程不会堵塞，立即返回			  
* 返 回 值	: bool	true表示获得临界区，false表示未获得临界区
*******************************************************************************/
bool critical_section::try_lock()
{
    if ( NULL == m_fnTryEnterCriticalSection )
    {
        return false;
    }
    else
    {
        return TRUE == m_fnTryEnterCriticalSection(&m_cs);
    }
}

KB_WIN32_NAMESPACE_END(multithread)
