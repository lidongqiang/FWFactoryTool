#include "stdafx.h"
#include "CommonDefine.h"

BOOL AnsiToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc)
{
    lpwzDst = NULL;
    if (!lpszSrc)
    {
        return FALSE;
    }
    int nSrcSize;
    nSrcSize = strlen(lpszSrc);
    if (nSrcSize<=0)
    {
        return FALSE;
    }
    nDstSize = MultiByteToWideChar(CP_ACP,0,lpszSrc,-1,NULL,0);
    if (nDstSize<=0)
    {
        return FALSE;
    }
    lpwzDst = new WCHAR[nDstSize];
    if (!lpwzDst)
    {
        return FALSE;
    }
    int nCopySize;
    nCopySize = MultiByteToWideChar(CP_ACP,0,lpszSrc,-1,lpwzDst,nDstSize);
    if (nCopySize<=0)
    {
        delete []lpwzDst;
        lpwzDst = NULL;
        return FALSE;
    }
    return TRUE;
}
BOOL UnicodeToAnsi(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc)
{
    lpszDst = NULL;
    if (!lpwzSrc)
    {
        return FALSE;
    }
    int nSrcSize;
    nSrcSize = wcslen(lpwzSrc);
    if (nSrcSize<=0)
    {
        return FALSE;
    }
    nDstSize = WideCharToMultiByte(CP_ACP,0,lpwzSrc,-1,NULL,0,NULL,NULL);
    if (nDstSize<=0)
    {
        return FALSE;
    }
    lpszDst = new CHAR[nDstSize];
    if (!lpszDst)
    {
        return FALSE;
    }
    int nCopySize;
    nCopySize = WideCharToMultiByte(CP_ACP,0,lpwzSrc,-1,lpszDst,nDstSize,NULL,NULL);
    if (nCopySize<=0)
    {
        delete []lpszDst;
        lpszDst = NULL;
        return FALSE;
    }
    return TRUE;
}

LPWSTR GetRootDir()
{
    bool bIsRun=true;
    BOOL bRet = FALSE;
    LPWSTR lpDir = NULL;
    int nSize;
    
    ::SendMessage((HWND)g_msgHwnd,WM_TEST_RUNNING,0,(LPARAM)&bIsRun);
    if (!bIsRun)
    {
        char szDir[MAX_PATH];
        memset(szDir,0,MAX_PATH);
        ::SendMessage((HWND)g_msgHwnd,WM_GET_DIR,(WPARAM)EmMsg_System_key,(LPARAM)szDir);
        if (strlen(szDir)>0)
        {
            bRet = AnsiToUnicode(lpDir,nSize,szDir);
            if (bRet)
            {
                return lpDir;
            }
        }
    }
    
    return 0;
}

VOID ConvertDos2Unix(LPCSTR pSrcBuf, LPSTR pDstBuf, int &dstSize)
{
    dstSize = 0;
    while(*pSrcBuf)
    {
        if (*pSrcBuf == '\r')
        {
            pSrcBuf++;
            continue;
        }
        *pDstBuf++ = *pSrcBuf++;
        dstSize++;
    }
    *pDstBuf = NULL;
}

BOOL U8ToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc)
{
    //UTF8 to Unicode
    //预转换，得到所需空间的大小
    nDstSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpszSrc, strlen(lpszSrc), NULL, 0);
    //分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
    lpwzDst = new WCHAR[nDstSize + 1];
    //转换
    ::MultiByteToWideChar(CP_UTF8, NULL, lpszSrc, strlen(lpszSrc), lpwzDst, nDstSize);
    //最后加上'\0'
    lpwzDst[nDstSize] = '\0';

	return TRUE;

}


BOOL UnicodeToU8(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc)
{
    // unicode to UTF8
    //预转换，得到所需空间的大小，这次用的函数和上面名字相反
    nDstSize = ::WideCharToMultiByte(CP_UTF8, NULL, lpwzSrc, wcslen(lpwzSrc), NULL, 0, NULL, NULL);
    //同上，分配空间要给'\0'留个空间
    //UTF8虽然是Unicode的压缩形式，但也是多字节字符串，所以可以以char的形式保存
    lpszDst= new char[nDstSize + 1];
    //转换
    //unicode版对应的strlen是wcslen
    ::WideCharToMultiByte(CP_UTF8, NULL, lpwzSrc, wcslen(lpwzSrc), lpszDst, nDstSize, NULL, NULL);
    //最后加上'\0'
    lpszDst[nDstSize] = '\0';

	return TRUE;
   	
}
