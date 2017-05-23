#if defined(WIN32)
#include <Windows.h>
#include <shlwapi.h>
#elif defined(__LINUX__)
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <cassert>
#include "./disk_utility.h"
#include "./path_utility.h"
#include "kb_lib/error.h"
#include "kb_lib/text/string_utility.h"

KB_NAMESPACE_BEGIN(filesystem)


/*******************************************************************************
* 函数名称	: disk_utility::get_partition_capacity
* 功能描述	: 根据文件路径获得对应磁盘分区的容量
              注意：一个磁盘可能有多个分区，本函数获得是一个磁盘分区的容量，而不是整个磁盘的容量
* 参　　数	: const tchar* path		文件路径	
* 返 回 值	: sint64				磁盘容量(单位为K)，-1表示错误，错误原因保存在errno中
*******************************************************************************/
sint64 disk_utility::get_partition_capacity(const tchar* path)
{
    if( NULL == path || '\0' == path[0])
    {
        kb::set_last_error( ERR_WRONG_PARA );
        return -1;
    }

#if defined(WIN32)

    tstring root = path_utility::get_path_root(path);

    if( !root.empty() )
    {
        ULARGE_INTEGER uliFreeAvailable;
        ULARGE_INTEGER uliTotal;
        ULARGE_INTEGER uliFree;
        if ( ::GetDiskFreeSpaceEx(root.c_str(),&uliFreeAvailable,&uliTotal,&uliFree) )
        {
            return (sint64)uliTotal.QuadPart;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

#elif defined(__LINUX__)

    struct statfs buff;
    sint32 res = statfs(path, &buff);
    if(res != -1)
    {
        return (sint64)(buff.f_blocks * (buff.f_bsize / 1024));
    }

    return res;

#endif
}

/*******************************************************************************
* 函数名称	: disk_utility::get_free_partition_space
* 功能描述	: 根据文件路径获得对应磁盘分区的空闲磁盘容量
* 参　　数	: const tchar* path	文件路径	
* 返 回 值	: sint64			空闲磁盘分区容量(单位为K)，-1表示错误，错误原因保存在errno中
*******************************************************************************/
sint64 disk_utility::get_free_partition_space(const tchar* path)
{
    if( NULL == path || '\0' == path[0])
    {
        kb::set_last_error( ERR_WRONG_PARA );
        return -1;
    }

#if defined(WIN32)

    tstring root = path_utility::get_path_root(path);

    if( !root.empty() )
    {
        ULARGE_INTEGER uliFreeAvailable;
        ULARGE_INTEGER uliTotal;
        ULARGE_INTEGER uliFree;
        if ( ::GetDiskFreeSpaceEx(root.c_str(),&uliFreeAvailable,&uliTotal,&uliFree) )
        {
            return (sint64)uliFreeAvailable.QuadPart;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

#elif defined(__LINUX__)

    struct statfs buff;
    sint32 res = statfs(path, &buff);
    if(res != -1)
    {
        return (sint64)(buff.f_bfree * (buff.f_bsize / 1024));
    }

    return res;

#endif
}

/*******************************************************************************
* 函数名称	: disk_utility::get_partition_utilization_ratio
* 功能描述	: 根据文件路径获得对应磁盘分区的占有率
* 参　　数	: const tchar* path		文件路径	
* 返 回 值	: float64				磁盘分区的使用率，<0表示错误，错误原因保存在errno中
*******************************************************************************/
float64 disk_utility::get_partition_utilization_ratio(const tchar* path)
{
    if ( NULL == path || '\0' == path[0] )
    {
        kb::set_last_error( ERR_WRONG_PARA );
        return -1;
    }

#if defined(WIN32)

    tstring root = path_utility::get_path_root(path);

    if( !root.empty() )
    {
        ULARGE_INTEGER uliFreeAvailable;
        ULARGE_INTEGER uliTotal;
        ULARGE_INTEGER uliFree;
        if ( ::GetDiskFreeSpaceEx(root.c_str(),&uliFreeAvailable,&uliTotal,&uliFree) )
        {
            return ((float64)(uliTotal.QuadPart - uliFreeAvailable.QuadPart)) / uliTotal.QuadPart;
        }
        else
        {
            return -1.0;
        }
    }
    else
    {
        return -1.0;
    }

#elif defined(__LINUX__)

	struct statfs buff;
	sint32 res = statfs(path, &buff);
	if(res != -1)
	{
		return ((float64)(buff.f_blocks - buff.f_bfree)) / buff.f_blocks;
	}

	return -1.0;

#endif
}

/*******************************************************************************
* 函数名称  : disk_utility::get_disk_format
* 功能描述  : 获得路径所在磁盘的格式（仅提供给Windows使用）
* 参　　数  : const tchar* path     路径。可以是文件路径或目录路径。或者形如：c:\
* 返 回 值  : tstring               磁盘格式。如 NTFS，或 FAT32
*******************************************************************************/
tstring  disk_utility::get_disk_format(const tchar* path)
{
#if defined(WIN32)

    tchar disk_format[MAX_PATH] = {0};
    GetVolumeInformation( path_utility::get_path_root(path).c_str(),
        NULL, 0, NULL, NULL, NULL, disk_format, MAX_PATH);
    return text::string_utility::to_upper(disk_format);

#elif defined(__LINUX__)
	
    assert(0);
    return TSTRING("");

#endif
}

#ifdef __LINUX__

/*******************************************************************************
* 函数名称	: disk_utility::get_partition_capacity
* 功能描述	: 根据文件描述符获得对应磁盘分区的容量
              注意：一个磁盘可能有多个分区，本函数获得是一个磁盘分区的容量，而不是整个磁盘的容量
* 参　　数	: sint32 fd				文件描述符
* 返 回 值	: sint64				磁盘容量(单位为K)，-1表示错误，错误原因保存在errno中
*******************************************************************************/
sint64 disk_utility::get_partition_capacity(sint32 fd)
{
    struct statfs buff;
    sint32 res = fstatfs(fd, &buff);
    if(res != -1)
    {
        return (sint64)(buff.f_blocks * (buff.f_bsize / 1024)); // statfs.f_bsize单位为字节
    }

    return res;
}

/*******************************************************************************
* 函数名称	: disk_utility::get_free_partition_space
* 功能描述	: 根据文件描述符获得对应磁盘分区的空闲磁盘容量              
* 参　　数	: sint32 fd			文件描述符
* 返 回 值	: sint64			空闲磁盘分区容量(单位为K)，-1表示错误，错误原因保存在errno中
*******************************************************************************/
sint64 disk_utility::get_free_partition_space(sint32 fd)
{
    struct statfs buff;
    sint32 res = fstatfs(fd, &buff);
    if(res != -1)
    {
        return (sint64)(buff.f_bfree * (buff.f_bsize / 1024));
    }

    return res;
}

/*******************************************************************************
* 函数名称	: disk_utility::get_partition_utilization_ratio
* 功能描述	: 根据文件描述符获得对应磁盘分区的占有率              
* 参　　数	: sint32 fd			文件描述符
* 返 回 值	: float64			磁盘分区的使用率，<0表示错误，错误原因保存在errno中
*******************************************************************************/
float64 disk_utility::get_partition_utilization_ratio(sint32 fd)
{
    struct statfs buff;
    sint32 res = fstatfs(fd, &buff);
    if(res != -1)
    {
        return ((float64)(buff.f_blocks - buff.f_bfree)) / buff.f_blocks;
    }

    return -1.0;
}

#endif // __LINUX__

KB_NAMESPACE_END(filesystem)
