#if defined(WIN32)
#include <tchar.h>
#include <mbstring.h>
#endif

#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "./string_utility.h"
#include "kb_lib/safe_delete.h"

using namespace std;

KB_NAMESPACE_BEGIN(text)

/*******************************************************************************
* ��������  : string_utility_t<_char>::compare_nocase
* ��������  : �����ִ�Сд���ַ����Ƚ�
* �Ρ�����  : const _char* str1         ���ڱȽϵ��ַ���1
* �Ρ�����  : const _char* str2         ���ڱȽϵ��ַ���2
* �� �� ֵ  : sint32                    �ַ���1���ַ���2�Ƚϣ���ȷ���0��С�ڷ��ظ��������ڷ�������
*******************************************************************************/
template<>
sint32 string_utility_t<char>::compare_nocase(const char* str1, const char* str2)
{
#if defined(WIN32)
    return _stricmp( str1, str2 );
#elif defined(__LINUX__)
    return strcasecmp( str1, str2 );
#endif
}

template<>
sint32 string_utility_t<wchar_t>::compare_nocase(const wchar_t* str1, const wchar_t* str2)
{
#if defined(WIN32)
    return _wcsicmp( str1, str2 );
#elif defined(__LINUX__)
    return wcscasecmp( str1, str2 );
#endif
}

#ifdef WIN32

/*******************************************************************************
* ��������  : get_substr_by_display_width
* ��������  : ����ʾ�ı���ʱ������ֻ��Ҫ��ʾǰ�����ַ�����������Ӣ���ַ��������ַ���
��ʾ��Ȳ�һ��������������������ַ�����ȡ�����׽������ַ��ضϣ����߳���
��ʾ��ȡ������ṩ�ú�����������⡣
* �Ρ�����  : const _char* str           �ı����׵�ַ��
* �Ρ�����  : uint32 display_width       ��ȡ����ʾ��ȡ�
* �� �� ֵ  : _string 
*******************************************************************************/
template<>
std::string string_utility_t<char>::get_substr_by_display_width( const char* str, uint32 display_width )
{
    std::string ret;
    ret.reserve( display_width );
    const char* p = str;
    uint32 display_size = 0;
    while ( *p )
    {
        unsigned int c = _mbsnextc( (const unsigned char*)p );
        uint32 c_size = (c<256?1:2);
        if ( display_size + c_size > display_width )
        {
            break;
        }
        else
        {
            if ( c<256 )
            {
                ret += p[0];
                p++;
            }
            else
            {
                ret += p[0];
                ret += p[1];
                p+=2;
            }
            display_size += c_size;
        }
    }
    return ret;
}

template<>
std::wstring string_utility_t<wchar_t>::get_substr_by_display_width( const wchar_t* str, uint32 display_width )
{
    std::wstring ret;
    ret.reserve( display_width );
    const wchar_t* p = str;
    uint32 display_size = 0;
    while ( *p )
    {
        wchar_t c = *p;
        uint32 c_size = (c<256?1:2);
        if ( display_size + c_size > display_width )
        {
            break;
        }
        else
        {
            ret += c;
            p++;
            display_size += c_size;
        }
    }
    return ret;
}

#endif

KB_NAMESPACE_END(text)
