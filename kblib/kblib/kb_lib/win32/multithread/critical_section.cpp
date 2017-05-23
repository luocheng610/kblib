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
* ��������	: critical_section::lock
* ��������	: ����ٽ�����ͬһ���߳��ڿ�������
* �� �� ֵ	: void
*******************************************************************************/
void critical_section::lock()
{
	::EnterCriticalSection(&m_cs);
}

/*******************************************************************************
* ��������	: critical_section::unlock
* ��������	: �ͷ��ٽ���
* �� �� ֵ	: void
* ��������	: �������޶�
* �������	: 2008��9��11��
* �޸�����		   �޸���		  �޸�����
*******************************************************************************/
void critical_section::unlock()
{
    ::LeaveCriticalSection(&m_cs);
}

/*******************************************************************************
* ��������	: critical_section::try_lock
* ��������	: ���Ի���ٽ�����������ܻ���ٽ����������̲߳����������������			  
* �� �� ֵ	: bool	true��ʾ����ٽ�����false��ʾδ����ٽ���
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
