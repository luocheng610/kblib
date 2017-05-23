#pragma warning(disable:4996)

#if defined(WIN32)

#include <Windows.h>
#include <tchar.h>

#elif defined(__LINUX__)

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#endif

#include "kb_lib/error.h"
#include "kb_lib/filesystem/path_utility.h"
#include "./dir_utility.h"

using namespace std;

KB_NAMESPACE_BEGIN(filesystem)

/*******************************************************************************
* 函数名称	: dir_utility::is_dir
* 功能描述	: 判断路径是否对应目录
* 参　　数	: const tchar* path	要判断的路径
* 返 回 值	: bool				true表示对应目录，flase表示不是目录
*******************************************************************************/
bool dir_utility::is_dir(const tchar* path)
{
    if ( NULL == path || '\0' == path[0] )
    {
        kb::set_last_error( ERR_WRONG_PARA );
        return false;
    }

#if defined(WIN32)

    // 因为 FindFirstFile 在查找目录的时候，不支持目录后带 '\\'
    // 所以必须先去除
    tstring path_fixed = path;
    if ( path_fixed[path_fixed.size()-1]=='\\' ) 
    {
        path_fixed.resize( path_fixed.size()-1 );
    }
    WIN32_FIND_DATA data;
    HANDLE handle = ::FindFirstFile( path_fixed.c_str(), &data );
    if( handle == INVALID_HANDLE_VALUE )
    {
        return false;
    }
    else
    {
        bool ret = (0!=(data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY));
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

    return S_ISDIR(buf.st_mode); 

#endif
}

/*******************************************************************************
* 函数名称	: dir_utility::is_empty_directory
* 功能描述	: 判断一个文件夹是否为空文件夹
* 参　　数	: tchar* dir_path			需要进行判断的文件夹
* 返 回 值	: bool						true表示空文件夹，false表示非空文件夹
*******************************************************************************/
bool dir_utility::is_empty_directory(const tchar* dir_path)
{
    if( NULL == dir_path || '\0' == dir_path[0] )
    {
        return false;
    }

#if defined(WIN32)

    if( !is_dir(dir_path) )
    {
        return false;
    }
    tstring path_find = dir_path;
    if ( path_find[path_find.size()-1]!='\\' )
    {
        path_find += '\\';
    }
    path_find += TSTRING("*.*");

    WIN32_FIND_DATA wfd;
    HANDLE handle = ::FindFirstFile( path_find.c_str(), &wfd );
    if( handle == INVALID_HANDLE_VALUE )
    {
        return false;
    }
    else
    {
        do 
        { 
            if (wfd.cFileName[0] == '.') 
            {
                continue; // 过滤 . 和 .. 这两个目录 
            }
            else if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
            {
                // 有子目录，则说明非空
                FindClose(handle); // 关闭查找句柄 
                return false;
            } 
            else
            {
                // 有文件，则说明非空
                FindClose(handle); // 关闭查找句柄 
                return false;
            } 
        }
        while (FindNextFile(handle, &wfd)); 

        FindClose(handle); // 关闭查找句柄 

        return true;
    }

#elif defined(__LINUX__)

    DIR *dir = opendir(dir_path);
    if(NULL == dir)
    {
        return false;
    }

    bool dir_status = true;
    struct dirent* dir_entry = NULL;

    while((dir_entry = readdir(dir)) != NULL)
    {
        tchar* file_name = dir_entry->d_name;
        if(0 == strcmp(file_name, ".") || 0 == strcmp(file_name, "..")) // 过滤 . 和 .. 这两个目录 
        {
            continue;
        }

        dir_status = false;

        break;
    }

    closedir(dir);

    return dir_status;

#endif
}

/*******************************************************************************
* 函数名称	: dir_utility::get_current_working_directory
* 功能描述	: 获取当前的工作目录
* 返 回 值	: tstring			当前的工作目录
*******************************************************************************/
tstring dir_utility::get_current_working_directory()
{
#if defined(WIN32)

    tchar current_dir[MAX_PATH] = {0};
    DWORD ret = GetCurrentDirectory(MAX_PATH, current_dir);
    if( 0 == ret )
    {
        return TSTRING("");
    }
    else
    {
        if ( ret < MAX_PATH )
        {
            return current_dir;
        }
        else
        {
            tchar* larger_buffer = new tchar[ret+1];
            if ( larger_buffer )
            {
                ret = GetCurrentDirectory(ret+1,larger_buffer);
                tstring cur_dir( larger_buffer, ret );
                delete []larger_buffer;
                return cur_dir;
            }
            else
            {
                return TSTRING("");
            }
        }
    }

#elif defined(__LINUX__)

    tchar current_dir[MAX_PATH_LEN] = {0};
    if(NULL == getcwd(current_dir, sizeof(current_dir)))
    {
        return "";
    }
    return current_dir;

#endif
}

/*******************************************************************************
* 函数名称	: dir_utility::set_current_working_directory
* 功能描述	: 设置当前的工作目录
* 参　　数	: const tchar *dir		要设置的工作目录
* 返 回 值	: bool					true表示设置成功，false表示设置失败，失败原因保存在errno中
*******************************************************************************/
bool dir_utility::set_current_working_directory(const tchar* dir)
{
    if(NULL == dir || '\0' == dir[0])
    {
        set_last_error(ERR_WRONG_PARA);
        return false;
    }
#if defined(WIN32)

    return TRUE==::SetCurrentDirectory( dir );

#elif defined(__LINUX__)

    sint32 res = chdir(dir);
    return 0 == res;

#endif
}

/*******************************************************************************
* 函数名称	: dir_utility::delete_dir
* 功能描述	: 删除单个文件夹
* 参　　数	: const char* dir_path		要删除的文件夹
* 返 回 值	: bool						true表示成功删除文件夹，false表示失败
*******************************************************************************/
bool dir_utility::delete_dir(const tchar* dir_path)
{
#if defined(WIN32)
    return FALSE!=::RemoveDirectory(dir_path);
#elif defined(__LINUX__)
	return 0 == rmdir(dir_path);
#endif
}

/*******************************************************************************
* 函数名称	: dir_utility::delete_dir_tree
* 功能描述	: 删除整个文件夹，包括文件夹下的文件
* 参　　数	: const char* dir_path		要删除的文件夹
* 返 回 值	: bool						true表示成功删除文件夹，false表示失败
*******************************************************************************/
bool dir_utility::delete_dir_tree(const tchar* dir_path)
{
    if( NULL == dir_path || '\0' == dir_path[0] || !is_dir(dir_path) )
    {
        set_last_error(ERR_WRONG_PARA);
        return false;
    }

#if defined(WIN32)

    tstring strDirectory(dir_path);
    TCHAR cEnd = strDirectory[strDirectory.length()-1];
    if (cEnd != _T('\\') && cEnd != _T('/'))
    {
        strDirectory += _T("\\");
    }

    TCHAR szFind[MAX_PATH] = {0};
    _stprintf(szFind, _T("%s*.*"), strDirectory.c_str());

    WIN32_FIND_DATA fd;
    HANDLE hFile = ::FindFirstFile(szFind, &fd);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        do 
        {
            if (_tcsicmp(fd.cFileName, _T(".")) == 0
                || _tcsicmp(fd.cFileName, _T("..")) == 0)
            {
                continue;
            }
            else if ( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
            {
                TCHAR szFoder[MAX_PATH] = {0};
                _stprintf(szFoder, _T("%s%s"), strDirectory.c_str(), fd.cFileName);
                delete_dir_tree(szFoder);
                ::RemoveDirectory(szFoder);
            }
            else if (_tcsicmp(fd.cFileName, TSTRING("Thumbs.db")) == 0)
            {
                TCHAR szFile[MAX_PATH] = {0};
                _stprintf(szFile, _T("%sThumbs.db"), strDirectory.c_str());
                ::DeleteFile(szFile);
            }
            else
            {
                TCHAR szFileName[MAX_PATH];
                _stprintf(szFileName, TSTRING("%s%s"), strDirectory.c_str(), fd.cFileName);
                ::SetFileAttributes(szFileName, FILE_ATTRIBUTE_NORMAL); // 去掉只读属性
                ::DeleteFile(szFileName);
            }
        }
        while(::FindNextFile(hFile, &fd));

        ::FindClose(hFile);

        ::RemoveDirectory(dir_path);
        return !is_dir( dir_path );
    }
    else
    {
        return false;
    }

#elif defined(__LINUX__)

	// 只能删除文件夹，不能删除文件
	if(!is_dir(dir_path))
	{
		set_last_error(ERR_WRONG_PARA);
		return false;
	}

	char cmd[512];
	sprintf(cmd, "rm -rf %s", dir_path);
	cmd[sizeof(cmd) - 1] = '\0';
	return -1 == ::system(cmd) ? false : true;

#endif

}


#if defined(WIN32)

bool dir_utility::make_dir(const tchar* path)
{
    if (NULL == path || '\0' == path[0])
    {
        return false;
    }

    return TRUE == CreateDirectory( path, NULL );
}

bool dir_utility::make_dir_tree(const tchar* path)
{
    if ( is_dir(path) )
    {
        // 如果已经存在，则返回TRUE
        return true;
    }
    BOOL ret = CreateDirectory(path, NULL);
    if ( ret == FALSE )
    {
        tstring upper_dir = filesystem::path_utility::get_upper_dir(path);

        if ( !upper_dir.empty() )
        {
            bool bCreateUpperDir = make_dir_tree( upper_dir.c_str() );
            if ( bCreateUpperDir )
            {
                return FALSE!=CreateDirectory( path, NULL );
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return TRUE;
    }
}

#elif defined(__LINUX__)

/*******************************************************************************
* 函数名称	: dir_utility::is_dir
* 功能描述	: 判断文件描述符是否对应目录
* 参　　数	: sint32 fd			文件描述符
* 返 回 值	: bool				true表示对应目录，flase表示不是目录
*******************************************************************************/
bool dir_utility::is_dir(sint32 fd)
{
	// 在makefile中定义_FILE_OFFSET_BITS=64，则stat=stat64，能够支持大文件
	struct stat64 buf;    
    sint32 res = fstat64(fd, &buf);
	if(-1 == res)
	{
		return false;
	}

	return S_ISDIR(buf.st_mode);
}


/*******************************************************************************
* 函数名称	: dir_utility::make_dir
* 功能描述	: 创建一个目录
* 参　　数	: const tchar* path		要创建的目录
* 参　　数	: mode_t mode			创建模式（可读、可写、执行）
* 返 回 值	: sint32				0表示创建成功，-1表示失败，错误原因保存在errno中
*******************************************************************************/
sint32 dir_utility::make_dir(const tchar* path, mode_t mode)
{
	if(NULL == path || '\0' == path[0])
	{
		set_last_error(ERR_WRONG_PARA);
		return -1;
	}

    struct stat64 buf;
    sint32 res = stat64(path, &buf);
    if(-1 == res) // path不存在，或者没有执行权限
    {
        res = mkdir(path, mode);		

        return res;
    }
    else if(!S_ISDIR(buf.st_mode)) // path 存在，但不是目录
    {
        set_last_error(EEXIST);
        return -1;
    }

    return 0;
}

/*******************************************************************************
* 函数名称	: dir_utility::make_dir_tree
* 功能描述	: 递归创建目录			
* 参　　数	: const tchar* path		要创建的目录
* 参　　数	: mode_t mode			创建模式（可读、可写、执行）
* 返 回 值	: sint32				0表示创建成功，-1表示失败，错误原因保存在errno中
*******************************************************************************/
sint32 dir_utility::make_dir_tree(const tchar* path, mode_t mode)
{
    tchar path_copy[MAX_PATH_LEN] = {0};    
    tchar* path_traverser = path_copy;	

    if(NULL == path || '\0' == path[0] || strlen(path) >= MAX_PATH_LEN)
    {
        set_last_error(ERR_WRONG_PARA);
        return -1;
    }

    strcpy(path_copy, path);

    sint32 res = 0;

    // 跳过'/'，可能包括多个'/'
    while('/' == *path_traverser)
    {
        path_traverser++;
    }

    while(*path_traverser != '\0')
    {
        if('/' == *path_traverser)
        {            
            *path_traverser = '\0';
            res = make_dir(path_copy, mode);            
            *path_traverser = '/';

            if(-1 == res)
            {
                return res;
            }
        }

        path_traverser++;
    }

    // 创建最后一个目录
    res = make_dir(path);	

    return res;
}

#endif

KB_NAMESPACE_END(filesystem)
