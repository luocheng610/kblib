#pragma warning(disable:4267)
#pragma warning(disable:4996)

#include ".\net_utility.h"
#include <WinSock2.h>

#include "kb_lib/text/transcode.h"

KB_WIN32_NAMESPACE_BEGIN(net)

/*******************************************************************************
* ��������  : net_utility::ip_aton
* ��������  : ���ַ�����ʽ��IP��ַת��Ϊuint32��IP��ַ
* �Ρ�����  : const tchar* string_ip     ������ַ���IP��ַ
* �� �� ֵ  : ipv4                      IP��ַ�����ֱ�����ʽ������0xffffffffΪת��ʧ��
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
* ��������  : net_utility::ip_ntoa
* ��������  : ��uint32��ʽ��IP��ַת��Ϊ�ַ���
* �Ρ�����  : ipv4 ip_addr       uint32��ʽ��IP��ַ��Ҫ��Ϊ������
* �� �� ֵ  : std::wstring        ת�����IP��ַ
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
* ��������  : net_utility::is_ip
* ��������  : �ж�һ���ַ����Ƿ���IP��ַ
* �Ρ�����  : const tchar* string_ip    	Ҫ�жϵ��ַ���
* �� �� ֵ  : bool                      ��Ϊtrue, ����Ϊfalse
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
* ��������  : net_utility::resolve_to_address
* ��������  : ��������������ת��ΪIP��ַ���ú���Ϊ����ʽ����
* �Ρ�����  : const char* domain_name   Ҫ��ѯ��������������
* �� �� ֵ  : std::string               �����ĵ�һ��IP��ַ������"112.3.25.6"
                                        ����"0.0.0.0"��ʾ��ѯʧ��
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
* ��������  : net_utility::resolve_to_address
* ��������  : ��������������ת��ΪIP��ַ���ú���Ϊ����ʽ����
* �Ρ�����  : const char* domain_name   Ҫ��ѯ��������������
* �� �� ֵ  : std::string               �����ĵ�һ��IP��ַ������"112.3.25.6"
����"0.0.0.0"��ʾ��ѯʧ��
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
* ��������  : net_utility::resolve_to_address
* ��������  : ��������������ת��ΪIP��ַ���ú���Ϊ����ʽ����
* �Ρ�����  : const char* domain_name          Ҫ��ѯ��������������
* �Ρ�����  : std::vector<std::string>& ip     IP��ַ�б��б�Ϊ�ձ�ʾ��ѯʧ��
* �� �� ֵ  : void
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
* ��������  : net_utility::resolve_to_address_a
* ��������  : ��������������ת��ΪIP��ַ���ú���Ϊ����ʽ����
* �Ρ�����  : const char* domain_name          Ҫ��ѯ��������������
* �Ρ�����  : std::vector<std::string>& ip     IP��ַ�б��б�Ϊ�ձ�ʾ��ѯʧ��
* �� �� ֵ  : void
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
* ��������  : net_utility::get_localhost_name
* ��������  : �õ�������
* �� �� ֵ  : std::wstring    ����������
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
* ��������  : net_utility::get_localhost_name
* ��������  : �õ�������
* �� �� ֵ  : std::string    ����������
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