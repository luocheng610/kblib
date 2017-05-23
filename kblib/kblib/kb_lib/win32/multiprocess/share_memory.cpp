#include <Windows.h>
#include <cassert>
#include ".\share_memory.h"
#include "kb_lib\safe_delete.h"
#include "kb_lib\error.h"

#ifdef _DEBUG
#define SAFE_WAIT_MILLISEC  INFINITE
#else
#define SAFE_WAIT_MILLISEC  (10000) //等待10秒
#endif


KB_NAMESPACE_BEGIN( win32 )
NAMESPACE_BEGIN( multiprocess )

share_memory::share_memory()
{
    m_memory_ptr    = NULL;
    m_memory        = NULL;
    m_mutex         = NULL;
}

share_memory::~share_memory()
{
    close();
}

void share_memory::on_init_memory( kb::byte* mem_ptr, kb::uint32 mem_size )
{
    memset( mem_ptr, 0, mem_size );
}

bool share_memory::open( const char* name, kb::uint32 size, bool open_existing, kb::uint32 read_only )
{
    if ( !open_existing && read_only )
    {
        // 如果有可能创建共享内存，就必须可写，否则长度信息无法保存
        //assert( ("open_existing == false && read_only == true",NULL) );
        kb::set_last_error( ERR_WRONG_PARA );
        return false;
    }

    if ( NULL==m_memory && NULL==m_mutex && NULL!=name && '\0'!=name[0] && (open_existing||0!=size) )
    { 
        std::string mutex_name = name;
        mutex_name += "{0552B021-385F-42ff-9575-B13473D97923}";
        std::string mem_name = name;
        mem_name += "{00D765BD-E15C-49fc-85F3-8D9175531AEF}";

        if ( !open_existing ) // 不强制一定要打开已存在的
        {
            // 可能是创建，也可能是打开已有的
            m_mutex = ::CreateMutexA( NULL, TRUE, mutex_name.c_str() );
            if ( NULL==m_mutex )
            {
                return false;
            }

            // MSDN 上说，通过判断 GetLastError 可以知道这个 mutex 是否是已有的
            bool create_new = (::GetLastError()!=ERROR_ALREADY_EXISTS);
            // 如果是 create_new，那么天生就拥有了 mutex 锁。否则就需要去获取该锁
            if ( create_new || (WAIT_OBJECT_0==::WaitForSingleObject( m_mutex, SAFE_WAIT_MILLISEC )) )
            {
                // 大小增加4字节存储长度
                m_memory = ::CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, 
                    read_only?PAGE_READONLY:PAGE_READWRITE,
                    0, size+sizeof(kb::uint32), mem_name.c_str() );
                if ( NULL==m_memory )
                {
                    ReleaseMutex( m_mutex );
                    close();
                    return false;
                }

                m_memory_ptr = (kb::byte*)::MapViewOfFile( m_memory, read_only?FILE_MAP_READ:(FILE_MAP_WRITE|FILE_MAP_READ), 0, 0, 0 );
                if ( NULL==m_memory_ptr )
                {
                    ReleaseMutex( m_mutex );
                    close();
                    return false;
                }

                // 前四个字节存储长度
                *((kb::uint32*)m_memory_ptr) = size;
                if ( create_new )
                {
                    on_init_memory( m_memory_ptr+sizeof(kb::uint32), get_size() );
                }

                BOOL ret = ReleaseMutex( m_mutex );
                assert( ret );
                return true;
            }
            else
            {
                close();
                return false;
            }
        }
        else
        {
            // 只能打开已有的，否则就失败
            m_mutex = ::OpenMutexA( MUTEX_ALL_ACCESS, TRUE, mutex_name.c_str() );
            if ( (m_mutex!=NULL) && (WAIT_OBJECT_0==::WaitForSingleObject(m_mutex,SAFE_WAIT_MILLISEC)) )
            {
                m_memory = ::OpenFileMappingA( read_only?FILE_MAP_READ:(FILE_MAP_WRITE|FILE_MAP_READ), FALSE, mem_name.c_str() );
                if ( NULL==m_memory )
                {
                    ReleaseMutex( m_mutex );
                    close();
                    return false;
                }

                m_memory_ptr = (kb::byte*)::MapViewOfFile( m_memory, read_only?FILE_MAP_READ:(FILE_MAP_WRITE|FILE_MAP_READ), 0, 0, 0 );
                if ( NULL==m_memory_ptr )
                {
                    ReleaseMutex( m_mutex );
                    close();
                    return false;
                }

                ReleaseMutex( m_mutex );
                return true;
            }
            else
            {
                close();
                return false;
            }
        }
    }
    else
    {
        return false;
    }
}

void share_memory::close()
{
    if ( m_memory_ptr )
    {
        BOOL ret = ::UnmapViewOfFile( m_memory_ptr );
        assert( ret );
        m_memory_ptr = NULL;
    }
    SAFE_CLOSE_HANDLE( m_mutex );
    SAFE_CLOSE_HANDLE( m_memory );
}

kb::byte*  share_memory::get_ptr()
{
    if ( m_memory_ptr )
    {
        return m_memory_ptr+sizeof(kb::uint32);
    }
    else
    {
        return NULL;
    }
}

kb::uint32 share_memory::get_size()
{
    if ( m_memory_ptr )
    {
        kb::uint32 size = *((kb::uint32*)m_memory_ptr);
        return size;
    }
    else
    {
        return 0;
    }
}

kb::byte*  share_memory::lock( kb::uint32 wait_millisec )
{
    if ( m_mutex && m_memory )
    {
        assert( m_memory_ptr );
        if ( WAIT_OBJECT_0==::WaitForSingleObject(m_mutex, wait_millisec) )
        {
            return m_memory_ptr+sizeof(kb::uint32);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
}

void share_memory::unlock()
{
    if ( m_mutex && m_memory )
    {
        assert( m_memory_ptr );
        BOOL ret = ::ReleaseMutex( m_mutex );
        assert( ret );
    }
}

NAMESPACE_END( multiprocess )
KB_NAMESPACE_END( win32 )