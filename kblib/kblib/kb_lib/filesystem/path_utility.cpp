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
* ��������	: path_utility::get_file_name
* ��������	: ���ļ�·�������л�ȡ�ļ���
* �Ρ�����	: const tchar* file_path		�ļ�·������
* �� �� ֵ	: tstring						��ȡ���ļ���
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
* ��������	: path_utility::get_file_main_name
* ��������	: ����ļ������������ļ�����׺��
* �Ρ�����	: const tchar* file_path	�ļ�·��
* �� �� ֵ	: tstring					�������ļ�����׺���ļ���
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
* ��������	: path_utility::get_file_ext
* ��������	: ����ļ�����׺
* �Ρ�����	: const tchar* file_path	�ļ�·��
* �� �� ֵ	: tstring					�ļ�����׺
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
* ��������	: path_utility::get_path_dir
* ��������	: ����ļ����ڵ�Ŀ¼���û���֤�������һ�����ļ�
* �Ρ�����	: const tchar* file_path	�ļ�·��
* �� �� ֵ	: tstring					�ļ���Ӧ����һ��Ŀ¼
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
* ��������  : path_utility::get_path_root
* ��������  : ����ļ����ڵ��̷����� c:\���û���֤�������һ�����ļ�
* �Ρ�����  : const tchar* file_path
* �� �� ֵ  : tstring
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
* ��������	: path_utility::get_upper_dir
* ��������	: ��ô���Ŀ¼�Ķ�Ӧ��һ��Ŀ¼���û���֤�������һ����Ŀ¼
* �Ρ�����	: const tchar* dir_path		����Ŀ¼
* �� �� ֵ	: tstring					����Ŀ¼��Ӧ����һ��Ŀ¼
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
        // ĩβ���� '\\'������֮ǰ��ȥ��ĩβ�� '\\'
        dir.resize(dir.size() - 1);
    }

    tstring::size_type pos = dir.rfind( sperator );
    if ( tstring::npos == pos )
    {        
        set_last_error( ERR_WRONG_PARA );
        // �ַ����в����� '\\'
        return TSTRING("");
    }
    else
    {
        return dir.substr( 0, pos + 1 );
    }
}

/*******************************************************************************
* ��������	: path_utility::get_current_exe_path
* ��������	: ���ص�ǰ���� exe ȫ·�������ṩ��WIndowsƽ̨ʹ�ã�
* �� �� ֵ	: tstring
*******************************************************************************/
tstring path_utility::get_current_exe_path()
{
#if defined(WIN32)
    tchar modulePath[MAX_PATH];
    if ( GetModuleFileName( NULL, modulePath, MAX_PATH ) ) // ��ȡ����EXE����
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
* ��������	: path_utility::get_current_module_path
* ��������	: ���ص�ǰģ��ȫ·�����������λ�� exe �У��򷵻� exe ·�����������λ�� dll �У��򷵻� dll ·�������ṩ��WIndowsƽ̨ʹ�ã�
* �� �� ֵ	: tstring			��ǰģ���·��
*******************************************************************************/
tstring path_utility::get_current_module_path()
{
#if defined(WIN32)

    if ( !IsBadCodePtr((FARPROC)get_current_module_path) )
    {
        MEMORY_BASIC_INFORMATION mbi;        
        VirtualQuery(get_current_module_path,&mbi,sizeof(MEMORY_BASIC_INFORMATION)); // ��ȡ������ַ�ڴ���Ϣ
        tchar modulePath[MAX_PATH];
        if ( GetModuleFileName((HINSTANCE)mbi.AllocationBase,modulePath,MAX_PATH) ) // ��ȡ����DLL����
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
* ��������	: path_utility::get_common_appdata_dir
* ��������	: ���� windows ��KB��ȫ���������(App Data)Ŀ¼��ĩβ��'\'�� �����ṩ��WIndowsƽ̨ʹ�ã�
			  �� XP �£�Ϊ "C:\Documents and Settings\All Users\Application Data\Xunlei\" 
			  �� Vista �£�Ϊ "C:\ProgramData\Xunlei\"������Ŀ¼��
			  ��Ŀ¼Ϊ���������ȫ�����ݹ���Ŀ¼���洢�ڸ�Ŀ¼���ļ�Ϊ�����û�����
* �� �� ֵ	: tstring
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
* ��������  : path_utility::get_appdata_dir
* ��������  : ���� windows ��KB��ȫ���������(App Data)Ŀ¼��ĩβ��'\'�� �����ṩ��WIndowsƽ̨ʹ�ã�
              �� XP �£�Ϊ "C:\Documents and Settings\�û���\Application Data\Xunlei\" 
              �� Vista �£�Ϊ "C:\Users\�û���\AppData\Roaming\Xunlei\"������Ŀ¼��
              ��Ŀ¼Ϊ�뵱ǰ�û���صĹ����ļ��洢Ŀ¼��
* �� �� ֵ  : tstring
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
* ��������	: path_utility::get_tmpdata_dir
* ��������	: ����KBȫ����ʱ����Ŀ¼��ĩβ��'\'����Ϊ "%temp%\Xunlei\"��%temp%һ��Ϊc:\windows�����ṩ��WIndowsƽ̨ʹ�ã�
* �� �� ֵ	: tstring			��ʱ���ļ�·��
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



