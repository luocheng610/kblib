/*******************************************************************************
* ����ժҪ  : ����һ����������Ϣ��ȡ��صĹ��ߺ���
*******************************************************************************/
#ifndef __NET_UTILITY_H_KB__
#define __NET_UTILITY_H_KB__
#include <vector>
#include <string>

#include "kb_lib/type.h"
#include "kb_lib/tchar.h"

KB_WIN32_NAMESPACE_BEGIN(net)

typedef kb::uint32 ipv4;

template<class _char> class net_utility_t;

typedef net_utility_t<tchar> net_utility;

template<class _char>
class net_utility_t
{
public:
    typedef std::basic_string<_char> _string;

    /*******************************************************************************
    * ��������	: socket_utility::is_loopback_addr
    * ��������	: �ж��Ƿ�Ϊ�ػ�ip��ַ
    * �Ρ�����	: ipv4 ip_addr		��Ҫ�жϵ�ip��ַ��������
    * �� �� ֵ	: bool					true��ʾ�ǻػ�ip��false��ʾ���ǻػ�ip
    *******************************************************************************/
    static bool is_loopback_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000ff) == 0x0000007f);
    }

    /*******************************************************************************
    * ��������	: socket_utility::is_multicast_addr
    * ��������	: �ж�һ��ip��ַ�ǲ����鲥ip��ַ
    * �Ρ�����	: ipv4 ip_addr		��Ҫ�жϵ�ip��ַ��������
    * �� �� ֵ	: bool					true��ʾ���鲥ip��false��ʾ�����鲥ip
    *******************************************************************************/
    static bool is_multicast_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000f0) == 0x000000e0);
    }

    /*******************************************************************************
    * ��������	: socket_utility::is_badclass_addr
    * ��������	: �ж�һ��ip��ַ�ǲ��ǷǷ�ip��ַ
    * �Ρ�����	: ipv4 ip_addr		��Ҫ�жϵ�ip��ַ��������
    * �� �� ֵ	: bool					true��ʾ�ǷǷ�ip��false��ʾ���ǷǷ�ip
    *******************************************************************************/
    static bool is_badclass_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000f0) == 0x000000f0);
    }

    /*******************************************************************************
    * ��������	: socket_utility::is_zeronet_addr
    * ��������	: �ж�һ��ip��ַ�ǲ���������ַ��
    * �Ρ�����	: ipv4 ip_addr		��Ҫ�жϵ�ip��ַ��������
    * �� �� ֵ	: bool					true��ʾ��������ַ�࣬false��ʾ����������ַ��
    *******************************************************************************/
    static bool is_zeronet_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000ff) == 0x00000000);
    }

    /*******************************************************************************
    * ��������	: socket_utility::is_localmcast_addr
    * ��������	: �ж�һ��ip��ַ�ǲ��Ǿ����鲥ip��ַ
    * �Ρ�����	: ipv4 ip_addr			��Ҫ�жϵ�ip��ַ��������
    * �� �� ֵ	: bool						true��ʾ�Ǿ����鲥ip��false��ʾ���Ǿ����鲥ip
    *******************************************************************************/
    static bool is_localmcast_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x00FFFFFF) == 0x000000E0);
    }

    static ipv4 ip_aton(const _char* string_ip);

    static _string ip_ntoa(ipv4 ip_addr);

    static bool is_ip(const _char* string_ip);

    static _string resolve_to_address(const _char* domain_name);

    static void resolve_to_address(const _char* domain_name, std::vector<_string>& ip);

    static _string get_localhost_name();

};

KB_WIN32_NAMESPACE_END(net)
#endif // end of __NET_UTILITY_H_KB__


