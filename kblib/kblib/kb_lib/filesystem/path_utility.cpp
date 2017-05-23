#if defined(WIN32)
#include <Windows.h>
#include <shlobj.h>
#include <tchar.h>
#elif defined(__LINUX__)
#include <assert.h>
#endif

#include "./path_utility.h"
#include "./file_utility.h"
#include "./dir_utility.h"
#include "kb_lib/error.h"

KB_NAMESPACE_BEGIN(filesystem)

/*******************************************************************************
* 函数名称	: path_utility::get_file_name
* 功能描述	: 从文件路径参数中获取文件名
* 参　　数	: const tchar* file_path		文件路径参数
* 返 回 值	: tstring						获取的文件名
*******************************************************************************/
tstring path_utility::get_file_name(const tchar* file_path)
{
    if(NULL == file_path || '\0' == file_path[0])
    {
        return TSTRING("");
    }	

#if defined(WIN32)
    const wchar_t* pos = _tcsrchr(file_path, '\\');
#elif defined(__LINUX__)
    tchar* pos = strrchr(file_path, '/');
#endif

    if(NULL == pos)
    {
        return (tstring)file_path;
    }

    return (tstring)(pos + 1);	
}

/*******************************************************************************
* 函数名称	: path_utility::get_file_main_name
* 功能描述	: 获得文件名（不包括文件名后缀）
* 参　　数	: const tchar* file_path	文件路径
* 返 回 值	: tstring					不包括文件名后缀的文件名
*******************************************************************************/
tstring	path_utility::get_file_main_name(const tchar* file_path)
{
    tstring ret = get_file_name( file_path );
    tstring::size_type pos = ret.rfind('.');
    if( pos == tstring::npos )
    {
        return ret;
    }
    return ret.substr( 0, pos );
}

/*******************************************************************************
* 函数名称	: path_utility::get_file_ext
* 功能描述	: 获得文件名后缀
* 参　　数	: const tchar* file_path	文件路径
* 返 回 值	: tstring					文件名后缀
*******************************************************************************/
tstring	path_utility::get_file_ext(const tchar* file_path)
{
    tstring ret = get_file_name( file_path );
    tstring::size_type pos = ret.rfind('.');
    if( pos == tstring::npos )
    {
        return TSTRING("");
    }
    
    return ret.substr( pos + 1);
}

/*******************************************************************************
* 函数名称	: path_utility::get_path_dir
* 功能描述	: 获得文件所在的目录，用户保证输入参数一定是文件
* 参　　数	: const tchar* file_path	文件路径
* 返 回 值	: tstring					文件对应的上一级目录
*******************************************************************************/
tstring	path_utility::get_path_dir(const tchar* file_path)
{
    if(NULL == file_path || '\0' == file_path[0])
    {
        return TSTRING("");
    }	

#if defined(WIN32)
    const wchar_t* pos = _tcsrchr(file_path, '\\');
#elif defined(__LINUX__)
    tchar* pos = strrchr(file_path, '/');
#endif

    if(NULL == pos)
    {
		set_last_error( ERR_WRONG_PARA );
        return TSTRING("");
    }

    return tstring( file_path, pos - file_path + 1 );
}

/*******************************************************************************
* 函数名称  : path_utility::get_path_root
* 功能描述  : 获得文件所在的盘符，如 c:\，用户保证输入参数一定是文件
* 参　　数  : const tchar* file_path
* 返 回 值  : tstring
*******************************************************************************/
tstring path_utility::get_path_root(const tchar* file_path)
{
    if(NULL == file_path || '\0' == file_path[0])
    {
        return TSTRING("");
    }	

#if defined(WIN32)
    const wchar_t* pos = _tcschr(file_path, '\\');
#elif defined(__LINUX__)
    tchar* pos = strchr(file_path, '/');
#endif

    if(NULL == pos)
    {
        set_last_error( ERR_WRONG_PARA );
        return TSTRING("");
    }

    return tstring( file_path, pos - file_path + 1 );
}

/*******************************************************************************
* 函数名称	: path_utility::get_upper_dir
* 功能描述	: 获得传入目录的对应上一级目录，用户保证输入参数一定是目录
* 参　　数	: const tchar* dir_path		传入目录
* 返 回 值	: tstring					传入目录对应的上一级目录
*******************************************************************************/
tstring path_utility::get_upper_dir(const tchar* dir_path)
{
    if(NULL == dir_path || '\0' == dir_path[0] )
    {
        return TSTRING("");
    }	

#if defined(WIN32)
    tchar sperator = '\\';
#elif defined(__LINUX__)
    tchar sperator = '/';
#endif

    tstring dir = dir_path;
    if ( dir[dir.size() - 1] == sperator )
    {
        // 末尾就是 '\\'，处理之前先去掉末尾的 '\\'
        dir.resize(dir.size() - 1);
    }

    tstring::size_type pos = dir.rfind( sperator );
    if ( tstring::npos == pos )
    {        
        set_last_error( ERR_WRONG_PARA );
        // 字符串中不包含 '\\'
        return TSTRING("");
    }
    else
    {
        return dir.substr( 0, pos + 1 );
    }
}

/*******************************************************************************
* 函数名称	: path_utility::get_current_exe_path
* 功能描述	: 返回当前进程 exe 全路径（仅提供给WIndows平台使用）
* 返 回 值	: tstring
*******************************************************************************/
tstring path_utility::get_current_exe_path()
{
#if defined(WIN32)
    tchar modulePath[MAX_PATH];
    if ( GetModuleFileName( NULL, modulePath, MAX_PATH ) ) // 获取所在EXE名称
    {
        return modulePath;
    }
    else
    {
        return TSTRING("");
    }
#elif defined(__LINUX__)

	assert(0);
	return TSTRING("");

#endif
}

tstring path_utility::get_current_exe_dir()
{
    return get_upper_dir( get_current_exe_path().c_str() );
}

/*******************************************************************************
* 函数名称	: path_utility::get_current_module_path
* 功能描述	: 返回当前模块全路径，如果函数位于 exe 中，则返回 exe 路径，如果函数位于 dll 中，则返回 dll 路径（仅提供给WIndows平台使用）
* 返 回 值	: tstring			当前模块的路径
*******************************************************************************/
tstring path_utility::get_current_module_path()
{
#if defined(WIN32)

    if ( !IsBadCodePtr((FARPROC)get_current_module_path) )
    {
        MEMORY_BASIC_INFORMATION mbi;        
        VirtualQuery(get_current_module_path,&mbi,sizeof(MEMORY_BASIC_INFORMATION)); // 获取函数地址内存信息
        tchar modulePath[MAX_PATH];
        if ( GetModuleFileName((HINSTANCE)mbi.AllocationBase,modulePath,MAX_PATH) ) // 获取所在DLL名称
        {
            return modulePath;
        }
    }
    return TSTRING("");

#elif defined(__LINUX__)

	assert(0);
	return TSTRING("");

#endif
}

tstring path_utility::get_current_module_dir()
{
    return get_upper_dir( get_current_module_path().c_str() );
}

/*******************************************************************************
* 函数名称	: path_utility::get_common_appdata_dir
* 功能描述	: 返回 windows 下KB的全局软件数据(App Data)目录（末尾带'\'） （仅提供给WIndows平台使用）
			  在 XP 下，为 "C:\Documents and Settings\All Users\Application Data\Xunlei\" 
			  在 Vista 下，为 "C:\ProgramData\Xunlei\"（隐藏目录）
			  该目录为主流软件的全局数据公共目录。存储在该目录的文件为所有用户共享。
* 返 回 值	: tstring
*******************************************************************************/
tstring path_utility::get_common_appdata_dir()
{
#if defined(WIN32)

    TCHAR szFolderPath[MAX_PATH] ={0};
    BOOL bRet = SHGetSpecialFolderPath( NULL, szFolderPath, CSIDL_COMMON_APPDATA, TRUE );
    if ( FALSE == bRet )
    {
        return TSTRING("");
    }

    lstrcat( szFolderPath, _T("\\Xunlei") );

    if ( GetFileAttributes(szFolderPath) == -1 )
    {
        // dir not exist
        CreateDirectory( szFolderPath, NULL );
    }

    lstrcat( szFolderPath, _T("\\") );
    return szFolderPath;

#elif defined(__LINUX__)

	assert(0);
	return TSTRING("");

#endif
}

/*******************************************************************************
* 函数名称  : path_utility::get_appdata_dir
* 功能描述  : 返回 windows 下KB的全局软件数据(App Data)目录（末尾带'\'） （仅提供给WIndows平台使用）
              在 XP 下，为 "C:\Documents and Settings\用户名\Application Data\Xunlei\" 
              在 Vista 下，为 "C:\Users\用户名\AppData\Roaming\Xunlei\"（隐藏目录）
              该目录为与当前用户相关的公共文件存储目录。
* 返 回 值  : tstring
*******************************************************************************/
tstring path_utility::get_appdata_dir()
{
#if defined(WIN32)

    TCHAR szFolderPath[MAX_PATH] ={0};
    BOOL bRet = SHGetSpecialFolderPath( NULL, szFolderPath, CSIDL_APPDATA, TRUE );
    if ( FALSE == bRet )
    {
        return TSTRING("");
    }

    lstrcat( szFolderPath, _T("\\Xunlei") );

    if ( GetFileAttributes(szFolderPath) == -1 )
    {
        // dir not exist
        CreateDirectory( szFolderPath, NULL );
    }

    lstrcat( szFolderPath, _T("\\") );
    return szFolderPath;

#elif defined(__LINUX__)

    assert(0);
    return TSTRING("");

#endif
}

/*******************************************************************************
* 函数名称	: path_utility::get_tmpdata_dir
* 功能描述	: 返回KB全局临时数据目录（末尾带'\'），为 "%temp%\Xunlei\"，%temp%一般为c:\windows（仅提供给WIndows平台使用）
* 返 回 值	: tstring			临时性文件路径
*******************************************************************************/
tstring path_utility::get_tmpdata_dir()
{
#if defined(WIN32)

    TCHAR szFolderPath[MAX_PATH] ={0};
    DWORD dwRet = GetTempPath( MAX_PATH, szFolderPath );
    if ( 0 == dwRet )
    {
        return TSTRING("");
    }

    lstrcat( szFolderPath, _T("Xunlei") );
    if ( GetFileAttributes(szFolderPath) == -1 )
    {
        // dir not exist
        CreateDirectory( szFolderPath, NULL );
    }

    lstrcat( szFolderPath, _T("\\") );
    return szFolderPath;

#elif defined(__LINUX__)

	return TSTRING("/tmp/");

#endif

}

KB_NAMESPACE_END(filesystem)



