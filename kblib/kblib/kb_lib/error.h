#ifndef __ERROR_H_KB__
#define __ERROR_H_KB__

#include "./type.h"
#include "./tchar.h"

NAMESPACE_BEGIN(kb)

/* ���������Ͷ��� */
typedef         uint32              error_code;


/* KB�ڲ������뷶Χ���� */
#define         NO_ERR              (0)
#define         ERR_KB_MIN          (20000000)
#define         ERR_KB_MAX          (30000000)

/* KB������ */
#define         ERR_KB_LIB_MIN      (ERR_KB_MIN + 100000)
#define         ERR_KB_DATA_MIN     (ERR_KB_MIN + 5000000)
/* kb_client */
#define         ERR_KB_CLIENT_MIN   (ERR_KB_MIN + 6000000) 

#define         ERR_WRONG_PARA		(ERR_KB_LIB_MIN + 1) // �����������
#define         ERR_WRONG_FORMAT    (ERR_KB_LIB_MIN + 2) // ��ʽ����
#define         ERR_NONE_SYMLINK    (ERR_KB_LIB_MIN + 3) // �Ƿ������Ӵ���
#define         ERR_FILE_INVALID    (ERR_KB_LIB_MIN + 4) // �ļ���ʧ��
#define         ERR_FILE_IO_ERROR   (ERR_KB_LIB_MIN + 5) // �ļ���дʧ��
#define         ERR_INSUFF_BUFFER   (ERR_KB_LIB_MIN + 6) // ���뻺����̫С
#define         ERR_CHAR_INVALID    (ERR_KB_LIB_MIN + 7) // �Ƿ��ַ�
#define         ERR_LOGIC_ERROR     (ERR_KB_LIB_MIN + 7) // ��������߼�����
#define         ERR_NO_MEMORY       (ERR_KB_LIB_MIN + 8) // �ڴ����ʧ��

// ����"��ʽ�����������ģ��"�еĴ���
#define         ERR_PROTOBUF_MIN    (ERR_KB_LIB_MIN + 1000)
#define         ERR_INVALID_VERSION (ERR_PROTOBUF_MIN + 0) // ��Ч��message�汾��
#define         ERR_DUP_VERSION     (ERR_PROTOBUF_MIN + 1) // �ص���message�汾��
#define         ERR_MESSAGE_DEFINE  (ERR_PROTOBUF_MIN + 2) // message�Ķ������
#define         ERR_INVALID_FIELD   (ERR_PROTOBUF_MIN + 3) // ��Ч��message����Ϣ
#define         ERR_NOT_ENOUGH_BUF  (ERR_PROTOBUF_MIN + 4) // ��������Ļ��泤�Ȳ���
#define         ERR_STRING_SIZE     (ERR_PROTOBUF_MIN + 5) // �ַ���̫��
#define         ERR_ARRAY_SIZE      (ERR_PROTOBUF_MIN + 6) // ����̫����

#define         ERR_INVALID_TYPE    (ERR_KB_DATA_MIN + 1)       // kb_data �����ͷǷ�
#define         ERR_MAP_EMPTY       (ERR_KB_DATA_MIN + 2)       // MAP Ϊ��
#define         ERR_MAP_MISMATCH    (ERR_KB_DATA_MIN + 3)       // MAP �Ľṹ�����⣬���ڷ��������У����ǲ��� MAP ��
#define         ERR_INT_INVALID     (ERR_KB_DATA_MIN + 4)       // ����������ֵ���Ϸ���������Ԥ�ڵĴ�С
#define         ERR_IO_FAIL         (ERR_KB_DATA_MIN + 5)       // ��д����Ԥ�ڵ��ֽ���
#define         ERR_DECODE_NOTHING  (ERR_KB_DATA_MIN + 6)       // �����ƽ����з���û��Ԥ�ڵ��ֶ�
#define         ERR_LIST_INVALID    (ERR_KB_DATA_MIN + 7)       // �����ƽ����з���LIST�޷�����Ԥ�ڵ�Ԫ��
#define         ERR_XML_INVALID     (ERR_KB_DATA_MIN + 8)       // XML�����з���XML��ʽ����
#define         ERR_XML_NOT_UTF8    (ERR_KB_DATA_MIN + 9)       // XML����UTF8
#define         ERR_ITEM_NOT_IN_MAP (ERR_KB_DATA_MIN + 10)       // XML��ITEM����MAP��
#define         ERR_ITEM_NO_KEY     (ERR_KB_DATA_MIN + 11)       // XML��ITEMû��key
#define         ERR_UNKNOWN_ELEMENT (ERR_KB_DATA_MIN + 12)       // XML�г���δ֪��element
//#define         ERR_WSTR_IN_BEN     (ERR_KB_DATA_MIN + 13)       // bencoding ���벻֧�ֿ��ַ���
//#define         ERR_UI8_IN_BEN      (ERR_KB_DATA_MIN + 14)       // bencoding ���벻֧�� uint8
//#define         ERR_UI16_IN_BEN     (ERR_KB_DATA_MIN + 15)       // bencoding ���벻֧�� uint16
//#define         ERR_UI32_IN_BEN     (ERR_KB_DATA_MIN + 16)       // bencoding ���벻֧�� uint32
#define         ERR_BEN_INVALID     (ERR_KB_DATA_MIN + 17)       // bencoding �����ʽ����
#define         ERR_ZIP_FAIL        (ERR_KB_DATA_MIN + 18)       // zip ѹ��ʧ��
#define         ERR_UNZIP_FAIL      (ERR_KB_DATA_MIN + 19)       // zip ��ѹ��ʧ��
#define         ERR_HEX_STR_INVALID (ERR_KB_DATA_MIN + 20)       // �Ƿ���ʮ�������ַ���
#define         ERR_INVALID_FORMAT  (ERR_KB_DATA_MIN + 21)       // �Ƿ��ĸ�ʽ

#define         ERR_CONNECTION_CLOSED   (ERR_KB_CLIENT_MIN + 1)  // ���ӱ������ر�

#define         ERR_HTTP_REDIRECT_LOOP  (ERR_KB_CLIENT_MIN + 2)  // HTTP �ظ��ض��򳬹�5��
#define         ERR_HTTP_INVALID_LOCATION (ERR_KB_CLIENT_MIN + 3)  // HTTP �ظ� URL �Ƿ�
#define         ERR_HTTP_HEADER_INVALID (ERR_KB_CLIENT_MIN + 4)  // kb_client�е�HttpWrapper���յ�HTTP Header�в���Content-Length


/*******************************************************************************
* ��������  : set_last_error
* ��������  : �������һ�δ���Ĵ�����
* �Ρ�����  : error_code err �����룬�ô����뱣�����߳�˽�������У����߳�֮�䲻�ụ���ͻ
* �� �� ֵ  : void
*******************************************************************************/
void set_last_error( error_code err );

/*******************************************************************************
* ��������  : get_last_error
* ��������  : ��ȡ���һ�δ���Ĵ����롣
* �� �� ֵ  : error_code ������
*******************************************************************************/
error_code get_last_error();

/*******************************************************************************
* ��������  : get_error_desc
* ��������  : ��ȡָ�������������
* �Ρ�����  : error_code err �����롣Ҳ����ʹ��windowsϵͳ�Ĵ����롣
* �� �� ֵ  : tstring �����ı������û���ҵ����򷵻� ""��
*******************************************************************************/
tstring get_error_desc( error_code err );

/*******************************************************************************
* ��������  : get_error_desc
* ��������  : ��ȡ��ǰ�����������
* �� �� ֵ  : tstring �����ı������û���ҵ����򷵻� ""��
*******************************************************************************/
tstring get_error_desc();

/*********************************** �ඨ�� ***********************************/

NAMESPACE_END(kb)

#endif // end of __ERROR_H_KB__
