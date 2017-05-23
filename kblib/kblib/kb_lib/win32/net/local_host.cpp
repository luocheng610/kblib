//#pragma warning(disable:4267)
#include ".\local_host.h"
#include <WinSock2.h>
#include <IPHlpApi.h>
#include <tchar.h>

#include "kb_lib/text/transcode.h"


KB_WIN32_NAMESPACE_BEGIN(net)

/*******************************************************************************
* 函数名称  : local_host::init
* 功能描述  : 初始化local_host类，读取网卡配置信息并保存
* 返 回 值  : void
*******************************************************************************/

template<class _char>
void local_host_t<_char>::init()
{
    uint32 len = 0;
    uint32 nErrCode = ::GetAdaptersInfo(NULL, &len);
    if ( nErrCode == ERROR_BUFFER_OVERFLOW && len != 0 )
    {
        PIP_ADAPTER_INFO pAInfo = (PIP_ADAPTER_INFO)malloc(len);
        if ( pAInfo )
        {
            nErrCode = ::GetAdaptersInfo(pAInfo, &len);
            if ( nErrCode == ERROR_SUCCESS )
            {
                PIP_ADAPTER_INFO pAInfoEnum = pAInfo;

                while (pAInfoEnum != NULL)
                {
                    adapter_info ainfo;
                    kb::text::transcode::ANSI_to_tstring(pAInfoEnum->AdapterName, (kb::uint32)strlen(pAInfoEnum->AdapterName), ainfo.card_name);
                    kb::text::transcode::ANSI_to_tstring(pAInfoEnum->Description, (kb::uint32)strlen(pAInfoEnum->Description), ainfo.description);
                    kb::text::transcode::ANSI_to_tstring(pAInfoEnum->GatewayList.IpAddress.String, (kb::uint32)strlen(pAInfoEnum->GatewayList.IpAddress.String), ainfo.gate_way);
                    kb::text::transcode::ANSI_to_tstring(pAInfoEnum->IpAddressList.IpAddress.String, (kb::uint32)strlen(pAInfoEnum->IpAddressList.IpAddress.String), ainfo.ip_address);
                    kb::text::transcode::ANSI_to_tstring(pAInfoEnum->IpAddressList.IpMask.String, (kb::uint32)strlen(pAInfoEnum->IpAddressList.IpMask.String), ainfo.subnet_mask);

                    char mac[18];
                    sprintf(mac, "%02X-%02X-%02X-%02X-%02X-%02X", 
                        pAInfoEnum->Address[0], pAInfoEnum->Address[1],
                        pAInfoEnum->Address[2], pAInfoEnum->Address[3], 
                        pAInfoEnum->Address[4], pAInfoEnum->Address[5]);
                    kb::text::transcode::ANSI_to_tstring(mac, (kb::uint32)strlen(mac), ainfo.physical_address);

                    m_info.push_back(ainfo);

                    pAInfoEnum = pAInfoEnum->Next;
                }
            }

            free(pAInfo);
        }
    }
}

/*******************************************************************************
* 函数名称  : local_host_t<char>::local_host_t<char>
* 功能描述  : 构造函数的特例化
* 返 回 值  : 
*******************************************************************************/
template<>
local_host_t<char>::local_host_t<char>()
{
    local_host_t<char>::init();
}

template<>
local_host_t<wchar_t>::local_host_t<wchar_t>()
{
    local_host_t<wchar_t>::init();
}

KB_WIN32_NAMESPACE_END(net)