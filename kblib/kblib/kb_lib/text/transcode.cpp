#include <assert.h>
#include "./transcode.h"
#ifdef WIN32
#include <Windows.h>
#include <mbstring.h>
#endif


KB_NAMESPACE_BEGIN(text)

#define CP_GBK      (936)       // ��������code page

#define CHECK_INPUT()  do { if ( NULL == in || 0 == len /*|| '\0' == in[0]�ַ�����0�������*/ ) { out.erase(); return; } } while(0)

/*******************************************************************************
* ��������	: transcode::Unicode_to_UTF8
* ��������	: Unicodeת��ΪUTF8
* �Ρ�����	: const wchar_t* in		Unicode�ַ���
* �Ρ�����	: uint32 len			Unicode�ַ�������
* �Ρ�����	: std::string& out		ת�����UTF8�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::Unicode_to_UTF8(const wchar_t* in, uint32 len, std::string& out)
{
    CHECK_INPUT();    
    size_t out_len = len * 3 + 1;
    char* pBuf = new char[out_len];
    if ( NULL == pBuf )
    {
        return;
    }
    char* pResult = pBuf;
    memset(pBuf, 0, out_len);

#ifdef WIN32
    out_len = ::WideCharToMultiByte(CP_UTF8, 0, in, len, pBuf, len * 3, NULL, NULL);
    out.assign( pResult, out_len );
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("UTF-8","WCHAR_T");
    size_t in_len = len * sizeof(wchar_t);
    iconv(env, (char**)&in, (size_t*)&in_len, (char**)&pBuf, (size_t*)&out_len);     

    iconv_close(env);
    out = pResult;
#endif
    delete [] pResult;
    pResult = NULL;
    return;
}

/*******************************************************************************
* ��������	: transcode::UTF8_to_Unicode
* ��������	: UTF8ת��ΪUnicode
* �Ρ�����	: const char* in		UTF8�ַ���
* �Ρ�����	: uint32 len			UTF8�ַ�������
* �Ρ�����	: std::wstring& out		ת�����Unicode�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::UTF8_to_Unicode(const char* in, uint32 len, std::wstring& out)
{
    CHECK_INPUT();
    wchar_t* pBuf = new wchar_t[len + 1];
    if ( NULL == pBuf )
    {
        return;
    }
    size_t out_len = (len + 1) * sizeof(wchar_t);
    memset(pBuf, 0, (len + 1) * sizeof(wchar_t));
    wchar_t* pResult = pBuf;

#ifdef WIN32
    out_len = ::MultiByteToWideChar(CP_UTF8, 0, in, len, pBuf, len * sizeof(wchar_t));
    out.assign( pResult, out_len );
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("WCHAR_T","UTF-8");    
    iconv(env, (char**)&in, (size_t*)&len, (char**)&pBuf, (size_t*)&out_len);     
    iconv_close(env);
    out = pResult;
#endif

    delete [] pResult;
    pResult = NULL;
}

/*******************************************************************************
* ��������	: transcode::GBK_to_UTF8
* ��������	: GBKת��ΪUTF8
* �Ρ�����	: const char* in		GBK�ַ���
* �Ρ�����	: uint32 len			GBK�ַ�������
* �Ρ�����	: std::string& out		ת�����UTF8�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::GBK_to_UTF8(const char* in, uint32 len, std::string& out)
{
    CHECK_INPUT();
    //char (GBK) to UTF-8
    //GBK->wchar_t

#ifdef WIN32
    int wbufferlen = (int)::MultiByteToWideChar(CP_GBK,MB_PRECOMPOSED,in,(int)len,NULL,0);
    wchar_t* pwbuffer = new wchar_t[wbufferlen+4];
    if ( NULL == pwbuffer )
    {
        return;
    }
    wbufferlen = (int)::MultiByteToWideChar(CP_GBK,MB_PRECOMPOSED,in,(int)len,pwbuffer,wbufferlen+2);
    //wchar_t -> UTF8
    int bufferlen = ::WideCharToMultiByte(CP_UTF8,0,pwbuffer,wbufferlen,NULL,0,NULL,NULL);
    char* pBuffer = new char[bufferlen + 4];
    if ( NULL == pBuffer )
    {
        delete[] pwbuffer;
        return;
    }
    int out_len = ::WideCharToMultiByte(CP_UTF8,0,pwbuffer,wbufferlen,pBuffer,bufferlen+2,NULL,NULL);
    pBuffer[bufferlen] = '\0';
    out.assign( pBuffer, out_len );
    delete[] pwbuffer;
    delete[] pBuffer;
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("UTF-8","GBK");
    size_t out_len = (len * 2);
    char* pbuffer = new char[out_len];
    memset(pbuffer, 0, out_len);
    char* pResult = pbuffer;
    iconv(env, (char**)&in, (size_t*)&len, (char**)&pbuffer, (size_t*)&out_len);     
    iconv_close(env);

    out = pResult;
    delete[] pResult;
#endif

    return;
}

/*******************************************************************************
* ��������	: transcode::UTF8_to_GBK
* ��������	: UTF8ת��ΪGBK
* �Ρ�����	: const char* in		UTF8�ַ���
* �Ρ�����	: uint32 len			UTF8�ַ�������
* �Ρ�����	: std::string& out		ת�����GBK�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::UTF8_to_GBK(const char* in, uint32 len, std::string& out)
{
    CHECK_INPUT();
    //UTF-8 to char (GBK)
#ifdef WIN32
    //1) UTF-8 -> whcar_t
    int wbufferlen = (int)::MultiByteToWideChar(CP_UTF8,0,in,(int)len,NULL,0);    //may cause error
    wchar_t* pwbuffer = new wchar_t[wbufferlen+4];
    if ( NULL == pwbuffer )
    {
        return;
    }
    wbufferlen = (int)::MultiByteToWideChar(CP_UTF8,0,in,(int)len,pwbuffer,wbufferlen+2);
    //2) wchar_t -> GBK
    int bufferlen = (int)::WideCharToMultiByte(CP_GBK,0,pwbuffer,wbufferlen,NULL,0,NULL,NULL);
    char* pBuffer = new char[bufferlen + 4];
    if ( NULL == pBuffer )
    {
        delete[] pwbuffer;
        return;
    }
    int out_len = ::WideCharToMultiByte(CP_GBK,0,pwbuffer,wbufferlen,pBuffer,bufferlen+2,NULL,NULL);   
    pBuffer[bufferlen] = '\0';
    out.assign( pBuffer, out_len );
    delete[] pwbuffer;
    delete[] pBuffer;
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("GBK","UTF-8");
    size_t out_len = len + 1;
    char* pbuffer = new char[out_len];
    memset(pbuffer, 0, out_len);
    char* pResult = pbuffer;
    iconv(env, (char**)&in, (size_t*)&len, (char**)&pbuffer, (size_t*)&out_len);     
    iconv_close(env);

    out = pResult;
    delete[] pResult;
#endif
    return;
}


/*******************************************************************************
* ��������	: transcode::Unicode_to_GBK
* ��������	: Unicodeת��ΪGBK
* �Ρ�����	: const wchar_t* in			Unicode�ַ���
* �Ρ�����	: uint32 len				Unicode�ַ�������
* �Ρ�����	: std::string& out			ת�����GBK�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::Unicode_to_GBK(const wchar_t* in, uint32 len, std::string& out)
{
    CHECK_INPUT();
#ifdef WIN32
    int bufferlen = (int)::WideCharToMultiByte(CP_GBK,0,in,(int)len,NULL,0,NULL,NULL);
    char* pBuffer = new char[bufferlen + 4];
    if ( NULL == pBuffer )
    {
        return;
    }
    int out_len = ::WideCharToMultiByte(CP_GBK,0,in,(int)len,pBuffer,bufferlen+2,NULL,NULL);   
    pBuffer[bufferlen] = '\0';
    out.assign( pBuffer, out_len );
    delete[] pBuffer;
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("GBK","WCHAR_T");
    size_t out_len = len*2 + 1;
    size_t in_len = len * sizeof(wchar_t);
    char* pbuffer = new char[out_len];
    memset(pbuffer, 0, out_len);
    char* pResult = pbuffer;
    iconv(env, (char**)&in, (size_t*)&in_len, (char**)&pbuffer, (size_t*)&out_len);     
    iconv_close(env);
    out = pResult;
    delete[] pResult;
#endif
}

/*******************************************************************************
* ��������	: transcode::GBK_to_Unicode
* ��������	: GBKת��ΪUnicode
* �Ρ�����	: const char* in			GBK�ַ���
* �Ρ�����	: uint32 len				GBK�ַ�������
* �Ρ�����	: std::wstring& out			ת�����Unicode�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::GBK_to_Unicode(const char* in, uint32 len, std::wstring& out)
{
    CHECK_INPUT();
#ifdef WIN32
    int wbufferlen = (int)::MultiByteToWideChar(CP_GBK,MB_PRECOMPOSED,in,(int)len,NULL,0);
    wchar_t* pwbuffer = new wchar_t[wbufferlen+4];
    if ( NULL == pwbuffer )
    {
        return;
    }
    wbufferlen = (int)::MultiByteToWideChar(CP_GBK,MB_PRECOMPOSED,in,(int)len,pwbuffer,wbufferlen+2);
    pwbuffer[wbufferlen] = '\0';
    out.assign( pwbuffer, wbufferlen );
    delete[] pwbuffer;
    return;
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("WCHAR_T","GBK");
    size_t out_len = (len + 1)*sizeof(wchar_t);
    wchar_t* pbuffer = new wchar_t[len + 1];
    memset(pbuffer, 0,out_len);
    wchar_t* pResult = pbuffer;
    iconv(env, (char**)&in, (size_t*)&len, (char**)&pbuffer, (size_t*)&out_len);     
    iconv_close(env);
    out = pResult;
    delete[] pResult;
#endif
}

/*******************************************************************************
* ��������	: transcode::Unicode_to_ANSI
* ��������	: Unicodeת��ΪANSI
* �Ρ�����	: const wchar_t* in			Unicode�ַ����������Ǵ�Ӣ�ģ�
* �Ρ�����	: uint32 len				Unicode�ַ�������
* �Ρ�����	: std::string& out			ת�����ANSI�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::Unicode_to_ANSI(const wchar_t* in, uint32 len, std::string& out)
{
    CHECK_INPUT();
#ifdef WIN32
    int bufferlen = (int)::WideCharToMultiByte(CP_ACP,0,in,(int)len,NULL,0,NULL,NULL);
    char* pBuffer = new char[bufferlen + 4];
    if ( NULL == pBuffer )
    {
        return;
    }
    int out_len = ::WideCharToMultiByte(CP_ACP,0,in,(int)len,pBuffer,bufferlen+2,NULL,NULL);   
    pBuffer[bufferlen] = '\0';
    out.assign( pBuffer, out_len );
    delete[] pBuffer;
    //assert( _mbslen((const unsigned char*)out.c_str()) == strlen(out.c_str()) );
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("MS-ANSI","WCHAR_T");
    size_t out_len = len*2 + 1;
    size_t in_len = len * sizeof(wchar_t);
    char* pbuffer = new char[out_len];
    memset(pbuffer, 0, out_len);
    char* pResult = pbuffer;
    iconv(env, (char**)&in, (size_t*)&in_len, (char**)&pbuffer, (size_t*)&out_len);     
    iconv_close(env);
    out = pResult;
    delete[] pResult;
#endif
}

/*******************************************************************************
* ��������	: transcode::ANSI_to_Unicode
* ��������	: ANSIת��ΪUnicode
* �Ρ�����	: const char* in			ANSI�ַ���
* �Ρ�����	: uint32 len				ANSI�ַ�������
* �Ρ�����	: std::wstring& out			ת�����Unicode�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::ANSI_to_Unicode(const char* in, uint32 len, std::wstring& out)
{
    CHECK_INPUT();
#ifdef WIN32
    //assert( _mbslen((const unsigned char*)in) == strlen(in) ); // ANSI Ҫ������Ǵ�Ӣ��
    int wbufferlen = (int)::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,(int)len,NULL,0);
    wchar_t* pwbuffer = new wchar_t[wbufferlen+4];
    if ( NULL == pwbuffer )
    {
        return;
    }
    wbufferlen = (int)::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,(int)len,pwbuffer,wbufferlen+2);
    pwbuffer[wbufferlen] = '\0';
    out.assign( pwbuffer, wbufferlen );
    delete[] pwbuffer;
    return;
#elif defined __LINUX__
    iconv_t env;
    env = iconv_open("WCHAR_T","MS-ANSI");
    size_t out_len = (len + 1)*sizeof(wchar_t);
    wchar_t* pbuffer = new wchar_t[len + 1];
    memset(pbuffer, 0,out_len);
    wchar_t* pResult = pbuffer;
    iconv(env, (char**)&in, (size_t*)&len, (char**)&pbuffer, (size_t*)&out_len);     
    iconv_close(env);
    out = pResult;
    delete[] pResult;
#endif
}

/*******************************************************************************
* ��������	: transcode::UTF8_to_tstring
* ��������	: UTF8ת��ΪUnicode����GBK�ַ����������Ƿ����˺�UNICODE/_UNICODE��
* �Ρ�����	: const char* in		UTF8�ַ���
* �Ρ�����	: uint32 len			UTF8�ַ�������
* �Ρ�����	: std::basic_string<_char>& out			ת�����Unicode����GBK�ַ���
* �� �� ֵ	: void
*******************************************************************************/
void transcode::UTF8_to_tstring(const char* in, uint32 len, std::string& out)
{
    UTF8_to_GBK(in,len,out);
}

void transcode::UTF8_to_tstring(const char* in, uint32 len, std::basic_string<wchar_t>& out)
{
    UTF8_to_Unicode(in,len,out);
}

void transcode::tstring_to_UTF8(const char* in, uint32 len, std::string& out)
{
    GBK_to_UTF8(in,len,out);
}

void transcode::tstring_to_UTF8(const wchar_t* in, uint32 len, std::string& out)
{
    Unicode_to_UTF8(in,len,out);
}

void transcode::tstring_to_GBK(const char* in, uint32 len,std::string& out)
{
    out.assign( in, len );
}

void transcode::tstring_to_GBK(const wchar_t* in, uint32 len,std::string& out)
{
    Unicode_to_GBK( in, len, out );
}

void transcode::GBK_to_tstring(const char* in, uint32 len, std::string& out)
{
    out.assign( in, len );
}

void transcode::GBK_to_tstring(const char* in, uint32 len, std::wstring& out)
{
    GBK_to_Unicode( in, len, out );
}

void transcode::tstring_to_Unicode(const char* in, uint32 len,std::wstring& out)
{
    ANSI_to_Unicode( in, len, out );
}

void transcode::tstring_to_Unicode(const wchar_t* in, uint32 len,std::wstring& out)
{
    out.assign( in, len );
}

void transcode::Unicode_to_tstring(const wchar_t* in, uint32 len, std::string& out)
{
    Unicode_to_ANSI( in, len, out );
}

void transcode::Unicode_to_tstring(const wchar_t* in, uint32 len, std::wstring& out)
{
    out.assign( in, len );
}

void transcode::tstring_to_ANSI(const char* in, uint32 len,std::string& out)
{
    out.assign( in, len );
}

void transcode::tstring_to_ANSI(const wchar_t* in, uint32 len,std::string& out)
{
    Unicode_to_ANSI( in, len , out );
}

void transcode::ANSI_to_tstring(const char* in, uint32 len, std::string& out)
{
    out.assign( in, len );
}

void transcode::ANSI_to_tstring(const char* in, uint32 len, std::wstring& out)
{
    ANSI_to_Unicode( in, len , out );
}

KB_NAMESPACE_END(text)

