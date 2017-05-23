#if defined(WIN32)

#include <Windows.h>
#include <cassert>

#elif defined(__LINUX__)

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#endif

#include "./file_utility.h"
#include "kb_lib/error.h"

using namespace std;

KB_NAMESPACE_BEGIN(filesystem)

/*******************************************************************************
* ��������	: file_utility::is_file
* ��������	: �ж�·���Ƿ��Ӧ�ļ�
* �Ρ�����	: const tchar* path		Ҫ�жϵ�·��
* �� �� ֵ	: bool					true��ʾ��Ӧ�ļ���flase��ʾ�����ļ�
*******************************************************************************/
bool file_utility::is_file(const tchar* path)
{
	if(NULL == path || '\0' == path[0])
	{
		return false;
	}

#if defined(WIN32)

    WIN32_FIND_DATA data;
    HANDLE handle = ::FindFirstFile( path, &data );
    if( handle == INVALID_HANDLE_VALUE )
    {
        return false;
    }
    else
    {
        bool ret = (0==(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY));
        ::FindClose( handle );
        return ret;
    }

#elif defined(__LINUX__)

    struct stat64 buf;    
    sint32 res = stat64(path, &buf);
    if(-1 == res)
    {
        return false;
    }

    return S_ISREG(buf.st_mode); 

#endif
}

/*******************************************************************************
* ��������	: file_utility::file_exist
* ��������	: �ж�һ���ļ��Ƿ����
* �Ρ�����	: const tchar* path		�ļ���Ӧ·��
* �� �� ֵ	: bool					true��ʾ���ڣ�false��ʾ������
*******************************************************************************/
bool file_utility::is_file_exist(const tchar* path)
{
	return is_file(path);
}

/*******************************************************************************
* ��������	: file_utility::get_filesize
* ��������	: �����ļ�·������ļ��Ĵ�С
* �Ρ�����	: const tchar* path		�ļ���Ӧ��·��
* �� �� ֵ	: sint64				�ļ��Ĵ�С��-1��ʾ��������ԭ�򱣴���errno��
*******************************************************************************/
sint64 file_utility::get_filesize(const tchar* path)
{
	if(NULL == path || '\0' == path[0])
	{
		set_last_error(ERR_WRONG_PARA);
		return -1;
	}

#if defined(WIN32)

    // ʹ�� GetFileAttributesExA ��Ȼ�ٶȿ�Ч�ʸߣ����ǿ�����Ϊ�ļ�û�д�Ȩ�޶�����ʧ��
    // ����Ϊ�����������ʹ�� FindFirstFile
    WIN32_FIND_DATA data;
    HANDLE handle = ::FindFirstFile( path, &data );
    if( handle == INVALID_HANDLE_VALUE )
    {
        return -1;
    }
    else
    {
        ::FindClose( handle );
        if ( 0==(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
        {
            return data.nFileSizeLow + ( ((unsigned __int64)data.nFileSizeHigh) << 32 );
        }
        else
        {
            // ��һ��Ŀ¼
            return -1;
        }
    }

#elif defined(__LINUX__)

    struct stat64 buf;
    if(-1 == stat64(path, &buf))
    {
		return -1;
    }

	// Linux�£�Ŀ¼Ҳ���ļ�
    if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
    {
        return (sint64)buf.st_size;
    }

    return -1;

#endif
}

#ifdef __LINUX__

/*******************************************************************************
* ��������	: file_utility::is_file
* ��������	: �ж��ļ��������Ƿ��Ӧ�ļ�
* �Ρ�����	: sint32 fd				�ļ�������
* �� �� ֵ	: bool					true��ʾ��Ӧ�ļ���flase��ʾ�����ļ�
*******************************************************************************/
bool file_utility::is_file(sint32 fd)
{
    // ��makefile�ж���_FILE_OFFSET_BITS=64����stat=stat64���ܹ�֧�ִ��ļ�
    struct stat64 buf;    
    sint32 res = fstat64(fd, &buf);
    if(-1 == res)
    {
        return false;
    }

    return S_ISREG(buf.st_mode);
}


/*******************************************************************************
* ��������	: file_utility::get_filesize
* ��������	: �����ļ�����������ļ��Ĵ�С
* �Ρ�����	: sint32 fd			�ļ���Ӧ��������
* �� �� ֵ	: sint64			�ļ��Ĵ�С��-1��ʾ��������ԭ�򱣴���errno��
*******************************************************************************/
sint64 file_utility::get_filesize(sint32 fd)
{
    if(fd <= 0)
    {
        set_last_error(ERR_WRONG_PARA);
        return -1;
    }

    struct stat64 buf;
    if(-1 == fstat64(fd, &buf))
    {
        return -1;
    }

    // Linux�£�Ŀ¼Ҳ���ļ�
    if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
    {
        return (sint64)buf.st_size;
    }

    return -1;
}

/*******************************************************************************
* ��������	: file_utility::is_symlink
* ��������	: �ж�·���Ƿ��Ӧ��������
* �Ρ�����	: const tchar* path		Ҫ�жϵ�·��
* �� �� ֵ	: bool					true��ʾ��Ӧ�������ӣ�flase��ʾ���Ƿ�������
*******************************************************************************/
bool file_utility::is_symlink(const tchar* path)
{
    if(NULL == path || '\0' == path[0])
    {
        return false;
    }

    struct stat64 buf;    
    sint32 res = lstat64(path, &buf);
    if(-1 == res)
    {
        return false;
    }

    return S_ISLNK(buf.st_mode); 
}

/*******************************************************************************
* ��������	: file_utility::create_symlink
* ��������	: ������������
* �Ρ�����	: const tchar* old_path		�ɵ�·��
* �Ρ�����	: const tchar* new_path		�µķ�������·��
* �� �� ֵ	: sint32					0��ʾ�����ɹ���-1��ʾ��������ԭ�򱣴���errno��
*******************************************************************************/
sint32 file_utility::create_symlink(const tchar* old_path,const tchar* new_path)
{
    if(NULL == old_path || NULL == new_path)
    {
        set_last_error(ERR_WRONG_PARA);
        return -1;
    }

    if('\0' == old_path[0] || '\0' == new_path[0])
    {
        set_last_error(ERR_WRONG_PARA);
        return -1;
    }

    if(strlen(old_path) >= MAX_PATH_LEN || strlen(new_path) >= MAX_PATH_LEN)
    {
        set_last_error(ERR_WRONG_PARA);
        return -1;
    }

    sint32 res = symlink(old_path, new_path);	

    return res;
}

/*******************************************************************************
* ��������	: file_utility::delete_symlink
* ��������	: ɾ����������
* �Ρ�����	: const tchar* symlink_path		���������ļ�
* �� �� ֵ	: sint32						0��ʾ�ɹ���-1��ʾ��������ԭ�򱣴���errno��
*******************************************************************************/
sint32 file_utility::delete_symlink(const tchar* symlink_path)
{
	if(NULL == symlink_path || '\0' == symlink_path[0])
	{
		set_last_error(ERR_WRONG_PARA);
		return -1;
	}

	if(!is_symlink(symlink_path))
	{
		set_last_error(ERR_NONE_SYMLINK); // ����·�����Ƿ�������
		return -1;
	}	

	sint32 res = unlink(symlink_path);	

	return res;
}

/*******************************************************************************
* ��������	: file_utility::get_real_path_from_symlink
* ��������	: ���ݷ�������·�������ʵ·��
* �Ρ�����	: const tchar *symlink_path	��������·��
* �Ρ�����	: tchar* real_path_buff		������ʵ·���Ļ���ָ��
* �Ρ�����	: uint32 buff_size			������ʵ·���Ļ����С
* �� �� ֵ	: sint32					0��ʾ�ɹ���-1��ʾ��������ԭ�򱣴���errno��
*******************************************************************************/
sint32 file_utility::get_real_path_from_symlink(const tchar* symlink_path, tchar* real_path_buff, uint32 buff_size)
{
	if(NULL == symlink_path || '\0' == symlink_path[0] 
	   || NULL == real_path_buff || 0 == buff_size)
	{
		set_last_error(ERR_WRONG_PARA);
		return -1;
	}

    sint32 res = readlink(symlink_path, real_path_buff, buff_size);
    if(res != -1)
    {
        if((uint32)res >= buff_size)
        {
            real_path_buff[buff_size - 1] = '\0';
        }
        else
        {
            real_path_buff[res] = '\0';
        }

        res = 0;
    }	

    return res;	
}

#endif // __LINUX__

KB_NAMESPACE_END(filesystem)
