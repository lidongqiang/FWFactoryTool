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
    //Ԥת�����õ�����ռ�Ĵ�С
    nDstSize = ::MultiByteToWideChar(CP_UTF8, NULL, lpszSrc, strlen(lpszSrc), NULL, 0);
    //����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
    lpwzDst = new WCHAR[nDstSize + 1];
    //ת��
    ::MultiByteToWideChar(CP_UTF8, NULL, lpszSrc, strlen(lpszSrc), lpwzDst, nDstSize);
    //������'\0'
    lpwzDst[nDstSize] = '\0';

	return TRUE;

}


BOOL UnicodeToU8(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc)
{
    // unicode to UTF8
    //Ԥת�����õ�����ռ�Ĵ�С������õĺ��������������෴
    nDstSize = ::WideCharToMultiByte(CP_UTF8, NULL, lpwzSrc, wcslen(lpwzSrc), NULL, 0, NULL, NULL);
    //ͬ�ϣ�����ռ�Ҫ��'\0'�����ռ�
    //UTF8��Ȼ��Unicode��ѹ����ʽ����Ҳ�Ƕ��ֽ��ַ��������Կ�����char����ʽ����
    lpszDst= new char[nDstSize + 1];
    //ת��
    //unicode���Ӧ��strlen��wcslen
    ::WideCharToMultiByte(CP_UTF8, NULL, lpwzSrc, wcslen(lpwzSrc), lpszDst, nDstSize, NULL, NULL);
    //������'\0'
    lpszDst[nDstSize] = '\0';

	return TRUE;
   	
}
