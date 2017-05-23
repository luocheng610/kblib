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
* 函数名称	: file_utility::is_file
* 功能描述	: 判断路径是否对应文件
* 参　　数	: const tchar* path		要判断的路径
* 返 回 值	: bool					true表示对应文件，flase表示不是文件
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
* 函数名称	: file_utility::file_exist
* 功能描述	: 判断一个文件是否存在
* 参　　数	: const tchar* path		文件对应路径
* 返 回 值	: bool					true表示存在，false表示不存在
*******************************************************************************/
bool file_utility::is_file_exist(const tchar* path)
{
	return is_file(path);
}

/*******************************************************************************
* 函数名称	: file_utility::get_filesize
* 功能描述	: 根据文件路径获得文件的大小
* 参　　数	: const tchar* path		文件对应的路径
* 返 回 值	: sint64				文件的大小，-1表示出错，错误原因保存在errno中
*******************************************************************************/
sint64 file_utility::get_filesize(const tchar* path)
{
	if(NULL == path || '\0' == path[0])
	{
		set_last_error(ERR_WRONG_PARA);
		return -1;
	}

#if defined(WIN32)

    // 使用 GetFileAttributesExA 虽然速度快效率高，但是可能因为文件没有打开权限而返回失败
    // 所以为了稳妥起见，使用 FindFirstFile
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
            // 是一个目录
            return -1;
        }
    }

#elif defined(__LINUX__)

    struct stat64 buf;
    if(-1 == stat64(path, &buf))
    {
		return -1;
    }

	// Linux下，目录也是文件
    if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
    {
        return (sint64)buf.st_size;
    }

    return -1;

#endif
}

#ifdef __LINUX__

/*******************************************************************************
* 函数名称	: file_utility::is_file
* 功能描述	: 判断文件描述符是否对应文件
* 参　　数	: sint32 fd				文件描述符
* 返 回 值	: bool					true表示对应文件，flase表示不是文件
*******************************************************************************/
bool file_utility::is_file(sint32 fd)
{
    // 在makefile中定义_FILE_OFFSET_BITS=64，则stat=stat64，能够支持大文件
    struct stat64 buf;    
    sint32 res = fstat64(fd, &buf);
    if(-1 == res)
    {
        return false;
    }

    return S_ISREG(buf.st_mode);
}


/*******************************************************************************
* 函数名称	: file_utility::get_filesize
* 功能描述	: 根据文件描述符获得文件的大小
* 参　　数	: sint32 fd			文件对应的描述符
* 返 回 值	: sint64			文件的大小，-1表示出错，错误原因保存在errno中
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

    // Linux下，目录也是文件
    if(S_ISREG(buf.st_mode) || S_ISDIR(buf.st_mode))
    {
        return (sint64)buf.st_size;
    }

    return -1;
}

/*******************************************************************************
* 函数名称	: file_utility::is_symlink
* 功能描述	: 判断路径是否对应符号连接
* 参　　数	: const tchar* path		要判断的路径
* 返 回 值	: bool					true表示对应符号连接，flase表示不是符号连接
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
* 函数名称	: file_utility::create_symlink
* 功能描述	: 创建符号链接
* 参　　数	: const tchar* old_path		旧的路径
* 参　　数	: const tchar* new_path		新的符号链接路径
* 返 回 值	: sint32					0表示创建成功，-1表示出错，错误原因保存在errno中
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
* 函数名称	: file_utility::delete_symlink
* 功能描述	: 删除符号链接
* 参　　数	: const tchar* symlink_path		符号链接文件
* 返 回 值	: sint32						0表示成功，-1表示出错，错误原因保存在errno中
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
		set_last_error(ERR_NONE_SYMLINK); // 输入路径不是符号链接
		return -1;
	}	

	sint32 res = unlink(symlink_path);	

	return res;
}

/*******************************************************************************
* 函数名称	: file_utility::get_real_path_from_symlink
* 功能描述	: 根据符号链接路径获得真实路径
* 参　　数	: const tchar *symlink_path	符号链接路径
* 参　　数	: tchar* real_path_buff		保存真实路径的缓存指针
* 参　　数	: uint32 buff_size			保存真实路径的缓存大小
* 返 回 值	: sint32					0表示成功，-1表示出错，错误原因保存在errno中
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
