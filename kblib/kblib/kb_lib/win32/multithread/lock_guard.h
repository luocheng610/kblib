/*******************************************************************************
* ����ժҪ	: ��һ����װ�����������������ź������Է��û�ʹ����ʱ���ǽ���
*******************************************************************************/
#ifndef __LOCK_GUARD_H_KB__  
#define __LOCK_GUARD_H_KB__

#include <stdio.h>

KB_WIN32_NAMESPACE_BEGIN(multithread)

template <class LOCK>
class lock_guard
{
public:    
    lock_guard(LOCK& input_lock)
        :m_lock(input_lock)
    {
        m_lock.lock();
    }
    ~lock_guard()
    {
        m_lock.unlock();
    }
    
private:    
    LOCK&	m_lock;
};

KB_WIN32_NAMESPACE_END(multithread)

#endif // end of __LOCK_GUARD_H_KB__
