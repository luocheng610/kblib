/*******************************************************************************
* ����ժҪ  : �����ٽ������ٽ����뻥������mutex����ȣ�Ч��Ҫ�ߡ�������Ϊ�ٽ�������
              ���Ķ��󣬶����������û�̬������һ������¶��Ƽ�ʹ���ٽ�����
*******************************************************************************/
#ifndef __CRITICAL_SECTION_H_KB__
#define __CRITICAL_SECTION_H_KB__

#include "kb_lib/type.h"
#include "kb_lib/win32/multithread/lock_guard.h"

KB_WIN32_NAMESPACE_BEGIN(multithread)

class critical_section
{
public:
    critical_section();
    virtual ~critical_section();

    void lock();
    void unlock();
    bool try_lock();

    operator CRITICAL_SECTION()
    {
        return m_cs;
    }
    CRITICAL_SECTION* operator &()
    {
        return &m_cs;
    }
private:
    CRITICAL_SECTION	m_cs;		// Windows�е��ٽ���

    typedef BOOL (*TryEnterCriticalSectionProc)(
        LPCRITICAL_SECTION lpCriticalSection  // critical section
        );

    static TryEnterCriticalSectionProc  m_fnTryEnterCriticalSection;
};

typedef lock_guard<critical_section>    cs_lock_guard;
typedef lock_guard<critical_section>    critical_section_lock_guard;

KB_WIN32_NAMESPACE_END(multithread)

#endif // end of __CRITICAL_SECTION_H_KB__
