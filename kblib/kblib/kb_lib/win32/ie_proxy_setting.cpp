#include <Windows.h>
#include <wininet.h>
#include <vector>
#include "./ie_proxy_setting.h"
#include "kb_lib/text/format_utility.h"
#include "kb_lib/text/string_utility.h"

using namespace kb::net;

KB_NAMESPACE_BEGIN(win32)

ie_proxy_setting::ie_proxy_setting()
{
    read_ie_setting();
}

ie_proxy_setting::~ie_proxy_setting()
{
}

/*******************************************************************************
* ��������  : ie_proxy_setting::get_http_proxy_info
* ��������  : ���IE���������е�ΪHTTP���õĴ�����
* �� �� ֵ  : proxy
*******************************************************************************/
proxy ie_proxy_setting::get_http_proxy_info()
{
	return m_http_setting;
}

/*******************************************************************************
* ��������  : ie_proxy_setting::get_ftp_proxy_info
* ��������  : ���IE���������е�ΪFTP���õĴ�����
* �� �� ֵ  : proxy
*******************************************************************************/
proxy ie_proxy_setting::get_ftp_proxy_info()
{
	return m_ftp_setting;
}

/*******************************************************************************
* ��������  : ie_proxy_setting::get_https_proxy_info
* ��������  : ���IE���������е�ΪHTTPS���õĴ�����
* �� �� ֵ  : proxy
*******************************************************************************/
proxy ie_proxy_setting::get_https_proxy_info()
{
	return m_https_setting;
}

/*******************************************************************************
* ��������  : ie_proxy_setting::get_socks_proxy_info
* ��������  : ���IE���������е�ΪSOCKS���õĴ�����
* �� �� ֵ  : proxy
*******************************************************************************/
proxy ie_proxy_setting::get_socks_proxy_info()
{
	return m_socks_setting;
}

/*******************************************************************************
* ��������  : ie_proxy_setting::parse_string
* ��������  : ����IE�е������ַ������õ�������Ϣ
* �Ρ�����  : const tstring& setting_str    �����ַ���
* �� �� ֵ  : proxy                         ���ش�����Ϣ
*******************************************************************************/
proxy ie_proxy_setting::parse_string( const tstring& setting_str )
{
    tstring::size_type protocol_pos = setting_str.find(TSTRING("://"));
    tstring::size_type host_pos;
    proxy result;
    if( protocol_pos == tstring::npos )
    {
        host_pos = 0;
        result.m_type = proxy::DEFAULT;
    }
    else
    {
        host_pos = protocol_pos+3;
        tstring protocol_str;
        protocol_str.assign( setting_str,0, protocol_pos );
        if( protocol_str == TSTRING("http") )
        {
            result.m_type = proxy::HTTP;
        }
        else if( protocol_str == TSTRING("ftp") )
        {
            result.m_type = proxy::FTP;
        }
        else
        {
            result.m_type = proxy::NO_PROXY;
        }
    }
    tstring::size_type port_pos = setting_str.find(':',host_pos );
    if( port_pos != tstring::npos )
    {
        result.m_host.assign( setting_str, host_pos, port_pos-host_pos );
        tstring port_str;
        port_str.assign( setting_str, port_pos+1, setting_str.length()-port_pos-1 );
        result.m_port = text::format_utility::str_to_uint8( port_str );
    }
    else
    {
        result.m_type = proxy::NO_PROXY;
    }

    return result;
}

/*******************************************************************************
* ��������  : ie_proxy_setting::read_ie_setting
* ��������  : ��ȡIE�еĴ�����Ϣ����
* �� �� ֵ  : void
*******************************************************************************/
void ie_proxy_setting::read_ie_setting()
{
    tchar setting_buffer[1024];	
    DWORD len=1024;
	
    DWORD ret = InternetQueryOption( NULL, INTERNET_OPTION_PROXY, setting_buffer, &len);
    if( ret == 0 )
    {
        return;
    }
    INTERNET_PROXY_INFO* info_ptr=(INTERNET_PROXY_INFO*)setting_buffer;
    if( info_ptr->lpszProxy == NULL )
    {
        // �����û���������ȡ��IE PROXY�����ﲻ��յ�����
        m_http_setting.clear();
        m_ftp_setting.clear();
        m_https_setting.clear();
        m_socks_setting.clear();
        return;
    }

    tstring all_setting_str( info_ptr->lpszProxy );
    std::vector<tstring> setting_str_vector;

    // ���ո�ָ��ַ���
    kb::text::string_utility::split_string( all_setting_str.c_str(),TSTRING(" "), setting_str_vector );
    
    std::vector<tstring>::iterator it = setting_str_vector.begin();
    std::vector<tstring>::iterator end = setting_str_vector.end();
    while( it != end )
    {
        tstring::size_type pos = it->find(TSTRING("ftp="));
        proxy info;
        if( pos != tstring::npos )
        {
            tstring setting_str;
            setting_str.assign( *it, pos+4, it->length()-pos-4 );
            info = parse_string( setting_str );
            if( info.m_type != proxy::NO_PROXY )
            {
                m_ftp_setting = info;
                if( m_ftp_setting.m_type == proxy::DEFAULT )
                {
                    m_ftp_setting.m_type = proxy::FTP;
                }
            }
            it++;
            continue;
        }
        
        pos = it->find(TSTRING("http="));
        if( pos != tstring::npos )
        {
            tstring setting_str;
            setting_str.assign( *it, pos+5, it->length()-pos-5 );
            info = parse_string( setting_str );
            if( info.m_type != proxy::NO_PROXY )
            {
                m_http_setting = info;
                if( m_http_setting.m_type == proxy::DEFAULT )
                {
                    m_http_setting.m_type = proxy::HTTP;
                }
            }
            it++;
            continue;
        }
        
        pos = it->find(TSTRING("socks="));
        if( pos != tstring::npos )
        {
            tstring setting_str;
            setting_str.assign( *it, pos+6, it->length()-pos-6 );
            info = parse_string( setting_str );
            if( info.m_type != proxy::NO_PROXY )
            {
                m_socks_setting = info;
                m_socks_setting.m_type = proxy::SOCKS5;
            }
            it++;
            continue;
        }
        
        pos = it->find(TSTRING("https="));
        if( pos != tstring::npos )
        {
            tstring setting_str;
            setting_str.assign( *it, pos+6, it->length()-pos-6 );
            info = parse_string( setting_str );
            if( info.m_type != proxy::NO_PROXY )
            {
                m_https_setting = info;
                if( m_https_setting.m_type == proxy::DEFAULT )
                {
                    m_https_setting.m_type = proxy::HTTP;
                }
            }
            
            it++;
            continue;
        }
        
        pos = it->find(TSTRING("gopher="));
        if( pos != tstring::npos )
        {
            it++;
            continue;
        }
        
        //������Э��ʹ����ͬ�ķ�����
        info = parse_string( *it );
        if( info.m_type != proxy::NO_PROXY )
        {
            m_http_setting = info;
            m_http_setting.m_type = proxy::HTTP;
            m_ftp_setting = info;
            m_ftp_setting.m_type = proxy::HTTP;
            m_https_setting = info;
            m_https_setting.m_type = proxy::HTTP;
            break;
        }
        it++;
	}
}

KB_NAMESPACE_END(win32)