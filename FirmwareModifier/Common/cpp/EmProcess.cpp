#include "stdafx.h"
#include "../inc/EmProcess.h"
#include "../inc/EmUtil.h"
#include "../inc/EmTsz.h"
#include "../inc/EmTstr.h"
#include "../inc/EmStlStr.h"
#include "../inc/EmCharSet.h"

#include <afxwin.h>
#include <TLHELP32.H>

using namespace em;

bool EmProcess::Execute(const char* szAppName, const char* szCmd, unsigned short dwShowWindow, bool bWait)
{
	PROCESS_INFORMATION pInfo;
	memset(&pInfo,0,sizeof(pInfo));

	TCHAR szCmdCopy[10240] = {0};
	_tcscpy(szCmdCopy,EmTstr::FromUtf8(szCmd).Sz());

	STARTUPINFO sInfo;
	memset (&sInfo, 0, sizeof(sInfo));
	sInfo.cb = sizeof(sInfo);
	sInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	sInfo.wShowWindow = dwShowWindow;

	bool bResult = ::CreateProcess(EmTstr::FromUtf8(szAppName).Sz()
		,szCmdCopy,NULL,NULL,NULL,NORMAL_PRIORITY_CLASS,NULL,NULL,&sInfo,&pInfo);
	if(bResult){
		if(bWait){
			::CloseHandle(pInfo.hThread);
			::WaitForSingleObject(pInfo.hProcess,INFINITE);
			::CloseHandle(pInfo.hProcess);
		}else{
			::CloseHandle(pInfo.hThread);
			::CloseHandle(pInfo.hProcess);
		}

	}

	return bResult;
}


long EmProcess::FindProcessIdByName(const char* szFind)
{
	long dwProcessId = 0;
	std::string strProcessName;
	std::string csAllProcess;
	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32;
	memset(&pe32,0,sizeof(pe32));
	pe32.dwSize = sizeof(pe32);
	hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(EmUtil::IsInvalidHandle(hProcessSnap))
	{
		return -1;
	}
	
	if(!::Process32First(hProcessSnap,&pe32))
	{
		return -2;
	}
	else
	{
		do 
		{
			strProcessName = EmStlStr::UnicodeToUtf8( pe32.szExeFile );
			if(strProcessName.compare( szFind ) == 0)
			{
				dwProcessId = pe32.th32ProcessID;
				break;
			}
		} while (::Process32Next(hProcessSnap,&pe32));
	}
	
	return dwProcessId;
}

bool EmProcess::TerminateProcessByName(const char* szProcessName, UINT nExitCode /* = 0 */)
{
	long dwProcessId = FindProcessIdByName(szProcessName);
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS,true,dwProcessId);
	bool bTerminated = false;
	if(hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
	{
		bTerminated = ::TerminateProcess(hProcess,nExitCode);
	}
	
	return bTerminated;
}

int em::EmProcess::CloseWindowByTitle( const char* szTitle, int iAction /* = 0 */)
{
	int iWindowCount = 0;
	EmCloseWindowParam closeParam;
	memset(&closeParam,0,sizeof(closeParam));
	closeParam.szTitle = szTitle;
	closeParam.piWindowCount = &iWindowCount;
	closeParam.iAction = iAction;
	::EnumWindows((WNDENUMPROC)EmProcess::CloseWindowByTitleProc,(LPARAM)&closeParam);
	return iWindowCount;
}

bool em::EmProcess::CloseWindowByTitleProc( HWND hWnd, LPARAM lParam )
{
	std::string strTitle;
	EmCloseWindowParam* pCloseParam = (EmCloseWindowParam*)lParam;
	TCHAR szWindowText[512] = {0};
	::GetWindowText(hWnd,szWindowText,512);
	strTitle = EmStlStr::UnicodeToUtf8(szWindowText);
	if(strTitle.compare(  pCloseParam->szTitle ) == 0)
	{
		(*pCloseParam->piWindowCount)++;
		if(pCloseParam->iAction == 1)
		{
			::PostMessage(hWnd,WM_QUIT,0,0);
		}
		else
		{
			::PostMessage(hWnd,WM_CLOSE,0,0);
		}
	}
	return true;
}

int em::EmProcess::SendCommand(char* bufRawOutput, const char* utf8Cmd)
{
	const char* szPath = NULL;
	int iResult = 0;
	TCHAR tcsCmdCopy[4096] = {0};
	HANDLE hRead;
	HANDLE hWrite;
	SECURITY_ATTRIBUTES xSecInfo;
	STARTUPINFO xStartupInfo; 
	PROCESS_INFORMATION   xProcessInfo; 
	bool bResult = false;

	EmCharSet::Utf8ToTcs(tcsCmdCopy,utf8Cmd);

	memset(&xProcessInfo,0,sizeof(xProcessInfo));
	xSecInfo.nLength  = sizeof(xSecInfo);
	xSecInfo.lpSecurityDescriptor = NULL; 
	xSecInfo.bInheritHandle = true; 
	bResult = ::CreatePipe(&hRead,&hWrite,&xSecInfo,0);
	if(!bResult)
	{
		return EM_ERR_SYS_CREATE_PIPE;
	}
	
	memset(&xStartupInfo,0,sizeof(xStartupInfo));
	xStartupInfo.cb   =   sizeof(xStartupInfo);  
	::GetStartupInfo(&xStartupInfo); 
	xStartupInfo.hStdError = hWrite; 
	xStartupInfo.hStdOutput = hWrite;
	xStartupInfo.wShowWindow = SW_HIDE;
	xStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  
	
	if(szPath == NULL || strlen(szPath) == 0){
		bResult = ::CreateProcess(NULL,tcsCmdCopy ,NULL,NULL,true,NULL,NULL,NULL,&xStartupInfo,&xProcessInfo);
	}
	else
	{
		bResult = ::CreateProcess(EmTstr::FromUtf8(szPath).Sz(),tcsCmdCopy ,NULL,NULL,true,NULL,NULL,NULL,&xStartupInfo,&xProcessInfo);
	}

	if(!bResult)
	{
		return EM_ERR_SYS_CREATE_PROCESS;
	}
	::CloseHandle(xProcessInfo.hThread);
	::CloseHandle(xProcessInfo.hProcess);
	::CloseHandle(hWrite);
	
	char bufTemp[4096] = {0};  
	DWORD iRead = 0;  
	
	
	while(true)
	{
		iRead = 0;
		if(!::ReadFile(hRead,bufTemp,4096,&iRead,NULL))  
		{
			break;  
		}
		
		memcpy(bufRawOutput + iResult,bufTemp,iRead);
        
		iResult += iRead;
		::Sleep(10);  
    }

    bufRawOutput[iResult] = 0;
	::CloseHandle(hRead);
	return iResult;
}


int em::EmProcess::SendCommand(char** bufRawOutput, const char* utf8Cmd,  int nTimeoutCount)
{
    const char* szPath = NULL;
    int iResult = 0;
    TCHAR tcsCmdCopy[4096] = {0};
    HANDLE hRead;
    HANDLE hWrite;
    SECURITY_ATTRIBUTES xSecInfo;
    STARTUPINFO xStartupInfo; 
    PROCESS_INFORMATION   xProcessInfo; 
    bool bResult = false;
    
    EmCharSet::Utf8ToTcs(tcsCmdCopy,utf8Cmd);
    
    memset(&xProcessInfo,0,sizeof(xProcessInfo));
    xSecInfo.nLength  = sizeof(xSecInfo);
    xSecInfo.lpSecurityDescriptor = NULL; 
    xSecInfo.bInheritHandle = true; 
    bResult = ::CreatePipe(&hRead,&hWrite,&xSecInfo,0);
    if(!bResult)
    {
        return EM_ERR_SYS_CREATE_PIPE;
    }
    
    memset(&xStartupInfo,0,sizeof(xStartupInfo));
    xStartupInfo.cb   =   sizeof(xStartupInfo);  
    ::GetStartupInfo(&xStartupInfo); 
    xStartupInfo.hStdError = hWrite; 
    xStartupInfo.hStdOutput = hWrite;
    xStartupInfo.wShowWindow = SW_HIDE;
    xStartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  
    
    if(szPath == NULL || strlen(szPath) == 0){
        bResult = ::CreateProcess(NULL,tcsCmdCopy ,NULL,NULL,true,NULL,NULL,NULL,&xStartupInfo,&xProcessInfo);
    }
    else
    {
        bResult = ::CreateProcess(EmTstr::FromUtf8(szPath).Sz(),tcsCmdCopy ,NULL,NULL,true,NULL,NULL,NULL,&xStartupInfo,&xProcessInfo);
    }
    
    if(!bResult)
    {
        return EM_ERR_SYS_CREATE_PROCESS;
    }
    ::CloseHandle(xProcessInfo.hThread);
    ::CloseHandle(xProcessInfo.hProcess);
    ::CloseHandle(hWrite);

    DWORD iRead = 0;
    int nMultiple = 1;
    int nCurrentBuffer = 0;
    const DWORD nSize = 16384;
    char bufTemp[nSize] = {0}; 
    char* pInternalBuf0 = new char[nSize];
    char* pInternalBuf1;
    char* pInteralBuf = pInternalBuf0;
    
    Sleep(200);
    
    while(true)
    {
        iRead = 0;

        BOOL bRet;
        DWORD dwByteRead;
        DWORD dwTotalBytesAvail;
        DWORD dwBytesLeftThisMessage;
        bRet = PeekNamedPipe(
            hRead,              // handle to pipe
            bufTemp,                // data buffer
            sizeof(bufTemp),              // size of data buffer
            &dwByteRead,            // number of bytes read
            &dwTotalBytesAvail,      // number of bytes available
            &dwBytesLeftThisMessage  // unread bytes
        );
        if(bRet)
        {
            if (dwByteRead)
            {
                if (ReadFile(hRead,bufTemp,sizeof(bufTemp),&iRead,NULL))
                {
                    // Double buffer to ensure we read full message
                    if (iResult+iRead > nSize*nMultiple)
                    {
                        nMultiple++;
                        switch(nCurrentBuffer){
                        case 0:
                            TRACE0("Swap to buffer1\n");
                            nCurrentBuffer = 1;
                            pInternalBuf1 = new char[nSize*nMultiple];
                            memcpy(pInternalBuf1,pInternalBuf0,iResult);
                            pInteralBuf = pInternalBuf1;
                            delete pInternalBuf0;
                            pInternalBuf0 = NULL;
                            break;
                        case 1:
                            TRACE0("Swap to buffer0\n");
                            nCurrentBuffer = 0;
                            pInternalBuf0 = new char[nSize*nMultiple];
                            memcpy(pInternalBuf0,pInternalBuf1,iResult);
                            pInteralBuf = pInternalBuf0;
                            delete pInternalBuf1;
                            pInternalBuf1 = NULL;
                            break;
                        }
                    }
                    memcpy(pInteralBuf+iResult,bufTemp,iRead);
                    iResult += iRead;
                    ::Sleep(10);
                }
            } else {
                if (--nTimeoutCount > 0)
                {
                    Sleep(1000);
                    continue;
                } else {
                    break;
                }
				iResult=-1;
                break;
            }
        } else {
            break;
        }
    }
    
    pInteralBuf[iResult] = 0;
    *bufRawOutput = pInteralBuf;
    ::CloseHandle(hRead);
    return iResult;
}

std::string em::EmProcess::CurrentDir()
{
	TCHAR szDir[1024] = {0};
	::GetCurrentDirectory(1024,szDir);
	EmTsz::Replace(szDir,_T("\\"),_T("/"));
	int iLen = _tcslen(szDir);
	if(szDir[iLen - 1] != _T('/'))
	{
		szDir[iLen] = _T('/');
	}
	std::string strResult = EmTstr::ToUtf8(szDir);
	return strResult;
}

std::string em::EmProcess::CurrentName()
{
	std::string strFullName = CurrentPath();
	return EmStlStr::FileNameFromPath(strFullName.c_str());
}

std::string em::EmProcess::CurrentPath()
{
	std::string strDest;
	TCHAR szAppFileName[MAX_PATH] = {0};
	::GetModuleFileName(::GetModuleHandle(NULL),szAppFileName,MAX_PATH);
	strDest = EmTstr::ToUtf8(szAppFileName);
	EmStlStr::Replace(strDest.c_str(),"\\","/");
	return strDest;
}

std::string em::EmProcess::SendCommandUtf8( const char* szCmd )
{
	const char* szPath = NULL;
	char utf8Result[40960] = {0};
	int iResult = SendCommand(utf8Result,szCmd);
	std::string strResult;
	
	if(iResult > 0){
		strResult = utf8Result;
	}
	
	return strResult;
}

std::string em::EmProcess::SendCommandAnsi( const char* szCmd )
{
	const char* szPath = NULL;
	char ansiResult[40960] = {0};
	int iResult = SendCommand(ansiResult,szCmd);
	std::string strResult;
	
	if(iResult > 0){
		char utf8Result[40960] = {0};
		iResult = EmCharSet::AnsiToUtf8(utf8Result,ansiResult);
		strResult = utf8Result;
	}
	
	return strResult;
}

bool em::EmProcess::EnableDebugPrivilege()
{
	HANDLE hToken = NULL; 
	LUID xUid; 
	memset(&xUid,0,sizeof(xUid));
	TOKEN_PRIVILEGES xToken; 
	memset(&xToken,0,sizeof(xToken));

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return FALSE; 
	} 

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &xUid)) { 
		CloseHandle(hToken);
		return false; 
	} 

	xToken.PrivilegeCount = 1; 
	xToken.Privileges[0].Luid = xUid; 
	xToken.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if (!AdjustTokenPrivileges(hToken, FALSE, &xToken, sizeof(xToken), NULL, NULL)) { 
		CloseHandle(hToken); 
		return false; 
	} 
	return true;
}

