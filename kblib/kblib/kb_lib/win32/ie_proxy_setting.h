/*******************************************************************************
* ����ժҪ  : һ����ȡIE������Ϣ�ĸ����࣬��Ҫ���� Wininet.lib
*******************************************************************************/
#ifndef __IE_PROXY_SETTING_H_KB__
#define __IE_PROXY_SETTING_H_KB__

#include "kb_lib/type.h"
#include "kb_lib/tchar.h"

KB_NAMESPACE_BEGIN(net)

struct proxy
{   
    enum type { NO_PROXY, SOCKS5, HTTP, FTP, DEFAULT = UINT32_MAX };

    proxy()
    {
        clear();
    }

    void clear()
    {
        m_type = NO_PROXY;
        m_host.erase();
        m_port = 0;
        m_user.erase();
        m_pass.erase();
    }

    type            m_type; // ��������
    tstring         m_host; // ����������
    uint16          m_port; // �������˿�
    tstring         m_user; // �û���
    tstring         m_pass; // ����
};

KB_NAMESPACE_END(net)

KB_NAMESPACE_BEGIN(win32)

class ie_proxy_setting  
{
public:
    ie_proxy_setting();
    virtual ~ie_proxy_setting();

    void    read_ie_setting();

    net::proxy   get_http_proxy_info();
    net::proxy   get_ftp_proxy_info();
    net::proxy   get_https_proxy_info();
    net::proxy   get_socks_proxy_info();

protected:

    net::proxy   parse_string( const tstring& setting_str );

private:
    net::proxy   m_http_setting;
    net::proxy   m_ftp_setting;
    net::proxy   m_https_setting;
    net::proxy   m_socks_setting;
};

KB_NAMESPACE_END(win32)

#endif // end of __IE_PROXY_SETTING_H_KB__



