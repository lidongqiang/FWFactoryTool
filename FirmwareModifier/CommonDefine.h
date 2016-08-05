#ifndef _COMMON_DEFINE_HEADER
#define _COMMON_DEFINE_HEADER

enum ENUM_LANGUAGE{EmLang_CHN=1,EmLang_ENG};
enum ENUM_MSG_KEY{EmMsg_Boot_Key=0,EmMsg_Recovery_Key,EmMsg_System_key,EmMsg_Root_key};	
#define WM_TEST_RUNNING WM_USER+88
#define WM_GET_DIR WM_USER+89
#define WM_UNPACK_FINISH WM_USER+90
#define WM_PACK_FINISH WM_USER+91
#define WM_START_PACK  WM_USER+92
#define WM_UPDATE_CLONE_BTN  WM_USER+93

#define MAX_LANGUAGE 48
#define MAX_TIMEZONE 297
#define MAX_LENGTH   300 //出错信息的最大长度
#define MAX_SINGLE_APK_FILE_SIZE 1024*1024*1024
#define MAX_KEY_LENGTH  20
// For select init.rkxxboard.rc
enum EMPLATFORMS
{
    RK29_BOARD,
    RK2926_BOARD,
    RK2928_BOARD,
    RK30_BOARD,
    UNKNOWN_BOARD,
};

enum EMANIMATIONS
{
    BOOT_ANIMATION,
    SHUTDOWN_ANIMATION,
    UNKNOWN_ANIMATION,
};

enum emFileEncode
{
    FILE_ANSI = 0, // ANSI 或 无BOM的UTF8 编码
    FILE_UTF8,     // 有BOM的UTF8
    FILE_UCS2_B,   // ucs-2 big endian
    FILE_UCS2_L,   // ucs-2 little endian
};

extern BOOL AnsiToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc);
extern BOOL UnicodeToAnsi(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc);
extern BOOL U8ToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc);
extern BOOL UnicodeToU8(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc);
extern LPWSTR GetRootDir();
extern VOID ConvertDos2Unix(LPCSTR pSrcBuf, LPSTR pDstBuf, int &dstSize);
extern ULONGLONG GetFileSize(LPCTSTR szFileName);

#endif

