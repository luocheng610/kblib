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
* 函数名称  : string_utility_t<_char>::compare_nocase
* 功能描述  : 不区分大小写的字符串比较
* 参　　数  : const _char* str1         用于比较的字符串1
* 参　　数  : const _char* str2         用于比较的字符串2
* 返 回 值  : sint32                    字符串1与字符串2比较，相等返回0，小于返回负数，大于返回正数
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
* 函数名称  : get_substr_by_display_width
* 功能描述  : 在显示文本的时候，往往只需要显示前几个字符。但是由于英文字符和中文字符的
显示宽度不一样，所以如果仅仅按照字符来截取，容易将中文字符截断，或者超出
显示宽度。所以提供该函数解决该问题。
* 参　　数  : const _char* str           文本的首地址。
* 参　　数  : uint32 display_width       截取的显示宽度。
* 返 回 值  : _string 
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
