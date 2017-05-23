#include <string.h>
#include <stdio.h>

#ifdef __LINUX__

#include <errno.h>

#else // WIN32

#include <tchar.h>
#include <windows.h>

#endif // end of __LINUX__


#include "./error.h"

NAMESPACE_BEGIN(kb)

#ifdef __LINUX__

void set_last_error(error_code err)
{
	errno = err;
}

error_code get_last_error()
{
	return errno;
}

#else // WIN32

void set_last_error( error_code err )
{
	SetLastError( err );
}

error_code get_last_error()
{
	return GetLastError();
}

#endif // end of __LINUX__

tstring get_error_desc()
{
    return get_error_desc(get_last_error());
}

tstring get_error_desc(error_code err)
{
    switch(err)
    {
    case NO_ERR:
        {
            return TSTRING("success");
            break;
        }
    case ERR_WRONG_PARA:
        {
            return TSTRING("wrong input parameter");
            break;
        }
    case ERR_WRONG_FORMAT:
        {
            return TSTRING("wrong format");
            break;
        }
    case ERR_NONE_SYMLINK:
        {
            return TSTRING("input path isn't symlink");
            break;
        }
    case ERR_FILE_INVALID:
        {
            return TSTRING("file is invalid");
            break;
        }
    case ERR_FILE_IO_ERROR:
        {
            return TSTRING("file io error");
            break;
        }
    case ERR_INSUFF_BUFFER:
        {
            return TSTRING("buffer is insufficient");
            break;
        }
    case ERR_CHAR_INVALID:
        {
            return TSTRING("character is invalid");
            break;
        }
    case ERR_INVALID_VERSION:
        {
            return TSTRING("invalid message version");
            break;
        }
    case ERR_DUP_VERSION:
        {
            return TSTRING("duplicate message version");
            break;
        }
    case ERR_MESSAGE_DEFINE:
        {
            return TSTRING("wrong message definition");
            break;
        }
    case ERR_INVALID_FIELD:
        {
            return TSTRING("invalid message field");
            break;
        }
    case ERR_NOT_ENOUGH_BUF:
        {
            return TSTRING("not enough buffer size");
            break;
        }
    case ERR_STRING_SIZE:
        {
            return TSTRING("invalid string size");
            break;
        }
    case ERR_ARRAY_SIZE:
        {
            return TSTRING("invalid array size");
            break;
        }
    default:
        {
#if defined(__LINUX__)
            tchar* msg = strerror(err);
            return msg;
#elif defined(WIN32)
            /* 系统错误码描述 */
            TCHAR szMsg[MAX_PATH] = {0};
            FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, szMsg, sizeof(szMsg), NULL );
            /* 去掉尾部的回车换行符 */
            TCHAR* pCr = _tcschr( szMsg, '\r' );
            if ( pCr )
            {
                pCr[0] = '\0';
            }
            return szMsg;
#endif
        }
    }
}


NAMESPACE_END(kb)
