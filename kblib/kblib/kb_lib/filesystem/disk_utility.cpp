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
* ��������	: disk_utility::get_partition_capacity
* ��������	: �����ļ�·����ö�Ӧ���̷���������
              ע�⣺һ�����̿����ж�������������������һ�����̷������������������������̵�����
* �Ρ�����	: const tchar* path		�ļ�·��	
* �� �� ֵ	: sint64				��������(��λΪK)��-1��ʾ���󣬴���ԭ�򱣴���errno��
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
* ��������	: disk_utility::get_free_partition_space
* ��������	: �����ļ�·����ö�Ӧ���̷����Ŀ��д�������
* �Ρ�����	: const tchar* path	�ļ�·��	
* �� �� ֵ	: sint64			���д��̷�������(��λΪK)��-1��ʾ���󣬴���ԭ�򱣴���errno��
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
* ��������	: disk_utility::get_partition_utilization_ratio
* ��������	: �����ļ�·����ö�Ӧ���̷�����ռ����
* �Ρ�����	: const tchar* path		�ļ�·��	
* �� �� ֵ	: float64				���̷�����ʹ���ʣ�<0��ʾ���󣬴���ԭ�򱣴���errno��
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
* ��������  : disk_utility::get_disk_format
* ��������  : ���·�����ڴ��̵ĸ�ʽ�����ṩ��Windowsʹ�ã�
* �Ρ�����  : const tchar* path     ·�����������ļ�·����Ŀ¼·�����������磺c:\
* �� �� ֵ  : tstring               ���̸�ʽ���� NTFS���� FAT32
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
* ��������	: disk_utility::get_partition_capacity
* ��������	: �����ļ���������ö�Ӧ���̷���������
              ע�⣺һ�����̿����ж�������������������һ�����̷������������������������̵�����
* �Ρ�����	: sint32 fd				�ļ�������
* �� �� ֵ	: sint64				��������(��λΪK)��-1��ʾ���󣬴���ԭ�򱣴���errno��
*******************************************************************************/
sint64 disk_utility::get_partition_capacity(sint32 fd)
{
    struct statfs buff;
    sint32 res = fstatfs(fd, &buff);
    if(res != -1)
    {
        return (sint64)(buff.f_blocks * (buff.f_bsize / 1024)); // statfs.f_bsize��λΪ�ֽ�
    }

    return res;
}

/*******************************************************************************
* ��������	: disk_utility::get_free_partition_space
* ��������	: �����ļ���������ö�Ӧ���̷����Ŀ��д�������              
* �Ρ�����	: sint32 fd			�ļ�������
* �� �� ֵ	: sint64			���д��̷�������(��λΪK)��-1��ʾ���󣬴���ԭ�򱣴���errno��
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
* ��������	: disk_utility::get_partition_utilization_ratio
* ��������	: �����ļ���������ö�Ӧ���̷�����ռ����              
* �Ρ�����	: sint32 fd			�ļ�������
* �� �� ֵ	: float64			���̷�����ʹ���ʣ�<0��ʾ���󣬴���ԭ�򱣴���errno��
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
