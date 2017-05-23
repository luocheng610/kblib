/*******************************************************************************
* ����ժҪ  : ��ȡ���������������������Ϣ������MAC��ַ��IP��ַ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __LOCAL_HOST_H_KB__
#define __LOCAL_HOST_H_KB__

#pragma warning(disable:4812)
#pragma warning(disable:4996)

#include <vector>
#include <string>
#include "kb_lib/type.h"
#include "kb_lib/tchar.h"
#include "kb_lib/text/transcode.h"
#include "kb_lib/win32/net/net_utility.h"

KB_WIN32_NAMESPACE_BEGIN(net)

typedef kb::uint32 ipv4;

template<class _char> class local_host_t;

typedef local_host_t<tchar> loacl_host;

template<class _char>
class local_host_t
{
public:
    typedef typename std::basic_string<_char> _string;

public:
    local_host_t();
    virtual ~local_host_t();

public:
    uint32  get_ipaddr_num();

    _string get_ipaddr(kb::uint32 index);

    _string get_mac_addr(kb::uint32 index);

    _string get_net_mask(kb::uint32 index);

    _string get_netcard_description(kb::uint32 index);

    _string get_gateway_address(kb::uint32 index);

    _string get_adapter_name(kb::uint32 index);

    bool    is_local_ip_addr(ipv4 ip_addr);

protected:

    void init();

    struct adapter_info
    {
        _string physical_address;
        _string card_name;
        _string description;
        _string ip_address;
        _string subnet_mask;
        _string gate_way;
    };

    std::vector<adapter_info> m_info;
};


/*******************************************************************************
* ��������  : local_host_t<_char>::~local_host_t<_char>
* ��������  : local_host_t ����������
* �� �� ֵ  : 
*******************************************************************************/
template<class _char>
local_host_t<_char>::~local_host_t<_char>()
{
}

/*******************************************************************************
* ��������  : net_utility::get_ipaddr_num
* ��������  : ��ö�ȡ��������������Ϣ����
* �� �� ֵ  : uint32                Ϊ0ʱ��ʾû�ж�ȡ����Ϣ
*******************************************************************************/
template<class _char>
uint32 local_host_t<_char>::get_ipaddr_num()
{
    return (uint32)m_info.size();
}

/*******************************************************************************
* ��������  : net_utility::get_ipaddr_by_index
* ��������  : �����������õ������ĵ�һ��IP��ַ
* �Ρ�����  : uint32 index        ��Ϣ������
* �� �� ֵ  : std::wstring         ���ַ�����ʽ���ֳ���IP��ַ������"192.168.2.2"
�������������Χ���ؿ��ַ���
*******************************************************************************/
template<class _char>
std::basic_string<_char> local_host_t<_char>::get_ipaddr(uint32 index)
{
    if (index >= m_info.size())
    {
        return std::basic_string<_char>();
    }
    return m_info[index].ip_address;
}

/*************************************************************************************
* ��������  : net_utility::get_mac_addr
* ��������  : �����������õ�������MAC��ַ
* �Ρ�����  : uint32 index        ��Ϣ������
* �� �� ֵ  : std::wstring        ���ַ�����ʽ���ֳ���MAC��ַ������"00-1B-B9-78-B5-0B"
**************************************************************************************/
template<class _char>
std::basic_string<_char> local_host_t<_char>::get_mac_addr(uint32 index)
{
    if (index >= m_info.size())
    {
        return std::basic_string<_char>();
    }
    return m_info[index].physical_address;
}

/*******************************************************************************
* ��������  : net_utility::get_net_mask
* ��������  : �����������õ������ĵ�һ����������
* �Ρ�����  : uint32 index        ��Ϣ������
* �� �� ֵ  : std::wstring         ���ַ�����ʽ���ֳ����������룬����"255.255.255.0"
�������������Χ���ؿ��ַ���
*******************************************************************************/
template<class _char>
std::basic_string<_char> local_host_t<_char>::get_net_mask(uint32 index)
{
    if (index >= m_info.size())
    {
        return std::basic_string<_char>();
    }
    return m_info[index].subnet_mask;
}

/*******************************************************************************
* ��������  : net_utility::get_netcard_name
* ��������  : �����������õ���������������
* �Ρ�����  : uint32 index        ��Ϣ������
* �� �� ֵ  : std::wstring        ���ַ�����ʽ���ֳ���������������
�������������Χ���ؿ��ַ���
*******************************************************************************/
template<class _char>
std::basic_string<_char> local_host_t<_char>::get_netcard_description(uint32 index)
{
    if (index >= m_info.size())
    {
        return std::basic_string<_char>();
    }
    return m_info[index].description;
}

/*******************************************************************************
* ��������  : net_utility::get_gateway_address
* ��������  : �����������õ������ĵ�һ�����ص�ַ
* �Ρ�����  : uint32 index        ��Ϣ������
* �� �� ֵ  : std::wstring         ���ַ�����ʽ���ֳ������ص�ַ������"192.168.85.1"
�������������Χ���ؿ��ַ���
*******************************************************************************/
template<class _char>
std::basic_string<_char> local_host_t<_char>::get_gateway_address(uint32 index)
{
    if (index >= m_info.size())
    {
        return std::basic_string<_char>();
    }
    return m_info[index].gate_way;
}

/*******************************************************************************
* ��������  : net_utility::get_adapter_name
* ��������  : �����������õ�����������ϵͳ��
* �Ρ�����  : uint32 index        ��Ϣ������
* �� �� ֵ  : std::wstring        ���ַ�����ʽ���ֳ�������ϵͳ����
�������������Χ���ؿ��ַ���
*******************************************************************************/
template<class _char>
std::basic_string<_char> local_host_t<_char>::get_adapter_name(uint32 index)
{
    if (index >= m_info.size())
    {
        return std::basic_string<_char>();
    }
    return m_info[index].card_name;
}

/*******************************************************************************
* ��������	: socket_utility::is_local_ip_addr
* ��������	: �ж�һ��ip��ַ�ǲ��Ǳ���ip��ַ
* �Ρ�����	: ipv4 ip_addr		��Ҫ�жϵ�ip��ַ��������
* �� �� ֵ	: bool					true��ʾ�Ǳ���ip��false��ʾ���Ǳ���ip
*******************************************************************************/
template<class _char>
bool local_host_t<_char>::is_local_ip_addr(ipv4 ip_addr)
{
    _string addr = kb::win32::net::net_utility_t<_char>::ip_ntoa(ip_addr);
    for(uint32 x = 0; x < m_info.size(); x ++)
    {
        if(m_info[x].ip_address == addr)
        {
            return true;
        }
    }

    return false;
}


KB_WIN32_NAMESPACE_END(net)

#endif // end of __LOCAL_HOST_H_KB__
