#pragma warning(disable:4267)
#pragma warning(disable:4996)

#include ".\net_utility.h"
#include <WinSock2.h>

#include "kb_lib/text/transcode.h"

KB_WIN32_NAMESPACE_BEGIN(net)

/*******************************************************************************
* 函数名称  : net_utility::ip_aton
* 功能描述  : 将字符串形式的IP地址转化为uint32的IP地址
* 参　　数  : const tchar* string_ip     传入的字符串IP地址
* 返 回 值  : ipv4                      IP地址的数字表现形式，返回0xffffffff为转换失败
*******************************************************************************/
template<>
ipv4 net_utility_t<char>::ip_aton(const char* string_ip)
{
    if(NULL == string_ip || '\0' == string_ip[0])
    {
        return INADDR_NONE;
    }
    ipv4 ip_addr;
    ip_addr = inet_addr(string_ip);
    if (ip_addr == INADDR_NONE)
    {
        return INADDR_NONE;
    }
    return ip_addr;
}

template<>
ipv4 net_utility_t<wchar_t>::ip_aton(const wchar_t* string_ip)
{
    char* strIp = NULL;
    std::string str;
    if (NULL != string_ip)
    {
        kb::text::transcode::Unicode_to_ANSI(string_ip, wcslen(string_ip), str);
        strIp = const_cast<char*>(str.c_str());
    }
    if(NULL == strIp || '\0' == strIp[0])
    {
        return INADDR_NONE;
    }
    ipv4 ip_addr;
    ip_addr = inet_addr(strIp);
    if (ip_addr == INADDR_NONE)
    {
        return INADDR_NONE;
    }
    return ip_addr;
}
/*******************************************************************************
* 函数名称  : net_utility::ip_ntoa
* 功能描述  : 将uint32形式的IP地址转换为字符串
* 参　　数  : ipv4 ip_addr       uint32形式的IP地址，要求为主机序
* 返 回 值  : std::wstring        转换后的IP地址
*******************************************************************************/
template<>
std::wstring net_utility_t<wchar_t>::ip_ntoa(ipv4 ip_addr)
{
    wchar_t* ip_text = new wchar_t[20];
    unsigned char* ip = (unsigned char*)&ip_addr;
    swprintf(ip_text, L"%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    std::wstring str_addr(ip_text);
    delete [] ip_text;
    return str_addr;
}

template<>
std::string net_utility_t<char>::ip_ntoa(ipv4 ip_addr)
{
    std::wstring wstr = net_utility_t<wchar_t>::ip_ntoa(ip_addr);
    std::string str;
    kb::text::transcode::Unicode_to_ANSI(wstr.c_str(), wstr.length(), str);
    return str;
}

/*******************************************************************************
* 函数名称  : net_utility::is_ip
* 功能描述  : 判断一个字符串是否是IP地址
* 参　　数  : const tchar* string_ip    	要判断的字符串
* 返 回 值  : bool                      是为true, 不是为false
*******************************************************************************/
template<>
bool net_utility_t<char>::is_ip(const char* string_ip)
{
    ipv4 i = net_utility_t<char>::ip_aton(string_ip);
    return (i == INADDR_NONE) ? false : true;
}

template<>
bool net_utility_t<wchar_t>::is_ip(const wchar_t* string_ip)
{
    ipv4 i = net_utility_t<wchar_t>::ip_aton(string_ip);
    return (i == INADDR_NONE) ? false : true;
}

/*******************************************************************************
* 函数名称  : net_utility::resolve_to_address
* 功能描述  : 将域名或主机名转换为IP地址，该函数为阻塞式函数
* 参　　数  : const char* domain_name   要查询的域名或主机名
* 返 回 值  : std::string               域名的第一个IP地址，形如"112.3.25.6"
                                        返回"0.0.0.0"表示查询失败
*******************************************************************************/
template<>
std::string net_utility_t<char>::resolve_to_address(const char* domain_name)
{
    struct hostent* host;
    char szTmp[30];
    std::string ip("0.0.0.0");
    if(NULL == domain_name || '\0' == domain_name[0])
    {
        return ip;
    }

    host = gethostbyname(domain_name);
    if((NULL == host)||(NULL == host->h_addr_list))
    {		
        return ip;
    }

    for(sint32 i = 0; host->h_addr_list[i] != NULL; i ++)
    {
        strcpy(szTmp, inet_ntoa(*((struct in_addr *)host->h_addr_list[i])));
        ip = szTmp;
        break;
    }

    return ip;
}

/*******************************************************************************
* 函数名称  : net_utility::resolve_to_address
* 功能描述  : 将域名或主机名转换为IP地址，该函数为阻塞式函数
* 参　　数  : const char* domain_name   要查询的域名或主机名
* 返 回 值  : std::string               域名的第一个IP地址，形如"112.3.25.6"
返回"0.0.0.0"表示查询失败
*******************************************************************************/
template<>
std::wstring net_utility_t<wchar_t>::resolve_to_address(const wchar_t* domain_name)
{
    std::string strName;
    std::wstring wstr = L"0.0.0.0";
    if (NULL == domain_name)
    {
        return wstr;
    }
    kb::text::transcode::Unicode_to_ANSI(domain_name, wcslen(domain_name), strName);
    std::string str = net_utility_t<char>::resolve_to_address(strName.c_str());
    kb::text::transcode::ANSI_to_Unicode(str.c_str(), str.length(), wstr);
    return wstr;
}

/*******************************************************************************
* 函数名称  : net_utility::resolve_to_address
* 功能描述  : 将域名或主机名转换为IP地址，该函数为阻塞式函数
* 参　　数  : const char* domain_name          要查询的域名或主机名
* 参　　数  : std::vector<std::string>& ip     IP地址列表，列表为空表示查询失败
* 返 回 值  : void
*******************************************************************************/
template<>
void net_utility_t<wchar_t>::resolve_to_address(const wchar_t* domain_name, std::vector<std::wstring>& ip)
{
    ip.clear();
    char* strName = NULL;
    std::string str;

    if (NULL != domain_name)
    {
        kb::text::transcode::Unicode_to_ANSI(domain_name, wcslen(domain_name), str);
        strName = const_cast<char*>(str.c_str());
    }

    if(NULL == strName || '\0' == strName[0])
    {
        return;
    }

    struct hostent* host = gethostbyname(strName);
    if((NULL == host)||(NULL == host->h_addr_list))
    {		
        return;
    }	

    for(sint32 i = 0; host->h_addr_list[i] != NULL; i ++)
    {
        std::string szTmp = inet_ntoa(*((struct in_addr *)host->h_addr_list[i]));
        std::wstring szwTmp;
        kb::text::transcode::ANSI_to_Unicode(szTmp.c_str(), szTmp.length(), szwTmp);
        ip.push_back(szwTmp);
    }
}

/*******************************************************************************
* 函数名称  : net_utility::resolve_to_address_a
* 功能描述  : 将域名或主机名转换为IP地址，该函数为阻塞式函数
* 参　　数  : const char* domain_name          要查询的域名或主机名
* 参　　数  : std::vector<std::string>& ip     IP地址列表，列表为空表示查询失败
* 返 回 值  : void
*******************************************************************************/
template<>
void net_utility_t<char>::resolve_to_address(const char* domain_name, std::vector<std::string>& ip)
{
    ip.clear();

    if(NULL == domain_name || '\0' == domain_name[0])
    {
        return;
    }

    struct hostent* host = gethostbyname(domain_name);
    if((NULL == host)||(NULL == host->h_addr_list))
    {		
        return;
    }	

    for(sint32 i = 0; host->h_addr_list[i] != NULL; i ++)
    {
        std::string szTmp = inet_ntoa(*((struct in_addr *)host->h_addr_list[i]));
        ip.push_back(szTmp);
    }
}

/*******************************************************************************
* 函数名称  : net_utility::get_localhost_name
* 功能描述  : 得到本机名
* 返 回 值  : std::wstring    本机主机名
*******************************************************************************/
template<>
std::string net_utility_t<char>::get_localhost_name()
{
    char szTmp[100];
    std::string ip("unknown");

    gethostname(szTmp, 100);
    ip = szTmp;

    return ip;
}

/*******************************************************************************
* 函数名称  : net_utility::get_localhost_name
* 功能描述  : 得到本机名
* 返 回 值  : std::string    本机主机名
*******************************************************************************/
template<>
std::wstring net_utility_t<wchar_t>::get_localhost_name()
{
    std::string ip = net_utility_t<char>::get_localhost_name();;
    std::wstring wip;
    kb::text::transcode::ANSI_to_Unicode(ip.c_str(), ip.length(), wip);

    return wip;
}
KB_WIN32_NAMESPACE_END(net)