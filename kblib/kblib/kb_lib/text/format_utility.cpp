#ifdef WIN32

#pragma warning(disable:4267)
#pragma warning(disable:4996)
#include <tchar.h>

#endif // end of WIN32

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "./format_utility.h"

KB_NAMESPACE_BEGIN(text)

template<>
std::string format_utility_t<char>::to_string(uint32 i)
{
    char out[16];
#if defined(WIN32)
    _ultoa(i, out, 10);
#elif defined(__LINUX__)
    sprintf(out, "%u", i);
#endif
    return out;
}

template<>
std::wstring format_utility_t<wchar_t>::to_string(uint32 i)
{
    wchar_t out[16];
#if defined(WIN32)
    _ultow(i, out, 10);
#elif defined(__LINUX__)
    swprintf(out, sizeof(out) / sizeof(wchar_t), L"%u", i);
#endif
    return out;
}

template<>
std::string format_utility_t<char>::to_string(sint32 i)
{
    char out[16];
#if defined(WIN32)
    _ltoa(i, out, 10);
#elif defined(__LINUX__)
    sprintf(out, "%d", i);
#endif	
    return out; 
}

template<>
std::wstring format_utility_t<wchar_t>::to_string(sint32 i)
{
    wchar_t out[16];
#if defined(WIN32)
    _ltow(i, out, 10);
#elif defined(__LINUX__)
    swprintf(out, sizeof(out) / sizeof(wchar_t), L"%d", i);
#endif	
    return out; 
}

template<>
std::string format_utility_t<char>::to_string(uint64 i)
{
    char out[32];
#if defined(WIN32)
    _ui64toa(i, out, 10);
#elif defined(__LINUX__)
    sprintf(out, "%llu", (long long unsigned int)i);
#endif
    return out;
}

template<>
std::wstring format_utility_t<wchar_t>::to_string(uint64 i)
{
    wchar_t out[32];
#if defined(WIN32)
    _ui64tow(i, out, 10);
#elif defined(__LINUX__)
    swprintf(out, sizeof(out) / sizeof(wchar_t), L"%llu", (long long unsigned int)i);
#endif
    return out;
}

template<>
std::string format_utility_t<char>::to_string(sint64 i)
{
    char out[32];
#if defined(WIN32)
    _i64toa(i, out, 10);
#elif defined(__LINUX__)
    sprintf(out, "%lld", (long long int)i);
#endif
    return out;
}

template<>
std::wstring format_utility_t<wchar_t>::to_string(sint64 i)
{
    wchar_t out[32];
#if defined(WIN32)
    _i64tow(i, out, 10);
#elif defined(__LINUX__)
    swprintf(out, sizeof(out) / sizeof(wchar_t), L"%lld", (long long int)i);
#endif
    return out;
}

template<>
std::string format_utility_t<char>::to_string(float32 f)
{
    char out[64];
    sprintf(out, "%f", f);
    return out;
}

template<>
std::wstring format_utility_t<wchar_t>::to_string(float32 f)
{
    wchar_t out[64];
#if defined(WIN32)
    swprintf(out, L"%f", f);
#elif defined(__LINUX__)
    swprintf(out, sizeof(out) / sizeof(wchar_t), L"%f", f);
#endif
    return out;
}

template<>
std::string format_utility_t<char>::to_string(float64 f)
{
    char out[128];
#if defined(WIN32)
    sprintf(out, "%f", f);
#elif defined(__LINUX__)
    sprintf(out, "%lf", f);
#endif
    return out;
}

template<>
std::wstring format_utility_t<wchar_t>::to_string(float64 f)
{
    wchar_t out[128];
#if defined(WIN32)
    swprintf(out, L"%f", f);
#elif defined(__LINUX__)
    swprintf(out, sizeof(out) / sizeof(wchar_t), L"%lf", f);
#endif
    return out;
}

template<>
uint32 format_utility_t<char>::str_to_uint32(const char* s)
{
#if defined(WIN32)
    return atol(s);
#elif defined(__LINUX__)
    uint32 out = 0;
    sscanf(s, "%u", &out);
    return out;
#endif
}

template<>
uint32 format_utility_t<wchar_t>::str_to_uint32(const wchar_t* s)
{
#if defined(WIN32)
    return _wtol(s);
#elif defined(__LINUX__)
    uint32 out = 0;
    swscanf(s, L"%u", &out);
    return out;
#endif
}

template<>
sint32 format_utility_t<char>::str_to_sint32(const char* s)
{
#if defined(WIN32)
    return atol(s);
#elif defined(__LINUX__)
    return atoi(s);
#endif
}

template<>
sint32 format_utility_t<wchar_t>::str_to_sint32(const wchar_t* s)
{
#if defined(WIN32)
    return _wtol(s);
#elif defined(__LINUX__)
    sint32 out = 0;
    swscanf(s, L"%i", &out);
    return out;
#endif
}

template<>
uint64 format_utility_t<char>::str_to_uint64(const char* s)
{
#if defined(WIN32)
    return _atoi64(s);
#elif defined(__LINUX__)	
    long long unsigned int out = 0;
    sscanf(s, "%llu", &out);
    return (uint64)out;
#endif
}

template<>
uint64 format_utility_t<wchar_t>::str_to_uint64(const wchar_t* s)
{
#if defined(WIN32)
    return _wtoi64(s);
#elif defined(__LINUX__)	
    long long unsigned int out = 0;
    swscanf(s, L"%llu", &out);
    return (uint64)out;
#endif
}

template<>
sint64 format_utility_t<char>::str_to_sint64(const char* s)
{
#if defined(WIN32)
    return _atoi64(s);
#elif defined(__LINUX__)
    return (sint64)atoll(s);
#endif
}

template<>
sint64 format_utility_t<wchar_t>::str_to_sint64(const wchar_t* s)
{
#if defined(WIN32)
    return _wtoi64(s);
#elif defined(__LINUX__)
    long long int out = 0;
    swscanf(s, L"%llu", &out);
    return (sint64)out;    
#endif
}

template<>
float32 format_utility_t<char>::str_to_float32(const char* s)
{
    float32 f;
    sscanf(s, "%f", &f);	
    return f;
}

template<>
float32 format_utility_t<wchar_t>::str_to_float32(const wchar_t* s)
{
    float32 f;
#if defined(WIN32)
    swscanf(s, L"%f", &f);
#elif defined(__LINUX__)
    swscanf(s, L"%f", &f);
#endif
    return f;
}

template<>
float64 format_utility_t<char>::str_to_float64(const char* s)
{
    float64 f;
#if defined(WIN32)
    sscanf(s, "%f", &f);
#elif defined(__LINUX__)
    sscanf(s, "%lf", &f);
#endif
    return f;
}

template<>
float64 format_utility_t<wchar_t>::str_to_float64(const wchar_t* s)
{
    float64 f;
#if defined(WIN32)
    swscanf(s, L"%f", &f);
#elif defined(__LINUX__)
    swscanf(s, L"%lf", &f);
#endif
    return f;
}

template<>
bool format_utility_t<char>::str_to_bool(const char* s)
{
#if defined(WIN32)
    return ( 0 == _stricmp(s, "true") || 0 == _stricmp(s, "1") );
#elif defined(__LINUX__)
    return (0 == strcasecmp(s, "true") || 0 == strcasecmp(s, "1"));
#endif
}

template<>
bool format_utility_t<wchar_t>::str_to_bool(const wchar_t* s)
{
#if defined(WIN32)
    return ( 0 == _wcsicmp(s, L"true") || 0 == _wcsicmp(s, L"1") );
#elif defined(__LINUX__)
    return ( 0 == wcscasecmp(s, L"true") || 0 == wcscasecmp(s, L"1") );
#endif
}

/*******************************************************************************
* 函数名称  : format_utility_t<_char>::kilo_format
* 功能描述  : 将一个数值以B,KB,MB,GB,TB为单位格式化输出。
* 参　　数  : uint64 n			  数值
* 返 回 值  : std::string       格式化字符串
*******************************************************************************/
template<>
std::string format_utility_t<char>::kilo_format( float64 n )
{
    static const uint64 KILO = 1024;
    static const uint64 MEGA = 1024 * KILO;
    static const uint64 GIGA = 1024 * MEGA;
    static const uint64 TERA = 1024 * GIGA;

    char buf[128] = {0};

    if( n > TERA )
    {
        sprintf( buf, "%0.2lfTB", 1.0 * n / TERA );
    }
    else if( n > GIGA )
    {
        sprintf( buf, "%0.2lfGB", 1.0 * n / GIGA );
    }
    else if( n > MEGA )
    {
        sprintf( buf, "%0.2lfMB", 1.0 * n / MEGA );
    }
    else if( n > KILO )
    {
        sprintf( buf, "%0.2lfKB", 1.0 * n / KILO );
    }
    else if( n >= 0 )
    {
        sprintf( buf, "%.0lfB", n );
    }

    return buf;
}

template<>
std::wstring format_utility_t<wchar_t>::kilo_format( float64 n )
{
    static const uint64 KILO = 1024;
    static const uint64 MEGA = 1024 * KILO;
    static const uint64 GIGA = 1024 * MEGA;
    static const uint64 TERA = 1024 * GIGA;

    wchar_t buf[128] = {0};

    if( n > TERA )
    {
#if defined(WIN32)
        swprintf( buf, L"%0.2lfTB", 1.0 * n / TERA );
#elif defined(__LINUX__)
        swprintf( buf, sizeof(buf) / sizeof(wchar_t), L"%0.2lfTB", 1.0 * n / TERA );
#endif
    }
    else if( n > GIGA )
    {
#if defined(WIN32)
        swprintf( buf, L"%0.2lfGB", 1.0 * n / GIGA );
#elif defined(__LINUX__)
        swprintf( buf, sizeof(buf) / sizeof(wchar_t), L"%0.2lfGB", 1.0 * n / GIGA );
#endif
    }
    else if( n > MEGA )
    {
#if defined(WIN32)
        swprintf( buf, L"%0.2lfMB", 1.0 * n / MEGA );
#elif defined(__LINUX__)
        swprintf( buf, sizeof(buf) / sizeof(wchar_t), L"%0.2lfMB", 1.0 * n / MEGA );
#endif
    }
    else if( n > KILO )
    {
#if defined(WIN32)
        swprintf( buf, L"%0.2lfKB", 1.0 * n / KILO );
#elif defined(__LINUX__)
        swprintf( buf, sizeof(buf) / sizeof(wchar_t), L"%0.2lfKB", 1.0 * n / KILO );
#endif
    }
    else if( n >= 0 )
    {
#if defined(WIN32)
        swprintf( buf, L"%.0lfB", n );
#elif defined(__LINUX__)
        swprintf( buf, sizeof(buf) / sizeof(wchar_t), L"%.0lfB", n );
#endif
    }

    return buf;
}

KB_NAMESPACE_END(text)
