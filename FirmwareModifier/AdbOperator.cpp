// AdbOperator.cpp : implementation file
//

#include "stdafx.h"
#include "FirmwareModifier.h"
#include "AdbOperator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern cmLog *g_pPluginLogObject;
#define ANDROID_USB_CLASS_ID \
{0xf72fe0d4, 0xcbcb, 0x407d, {0x88, 0x14, 0x9e, 0xd6, 0x73, 0xd0, 0xdd, 0x6b}};
static const GUID usb_class_id = ANDROID_USB_CLASS_ID;

/////////////////////////////////////////////////////////////////////////////
// AdbOperator

AdbOperator::AdbOperator()
{
    // Init module path
    GetModuleFileName(NULL, m_strModulePath.GetBuffer(MAX_PATH), MAX_PATH);
    m_strModulePath.ReleaseBuffer();
    m_strModulePath = m_strModulePath.Left(m_strModulePath.ReverseFind(_T('\\'))+1);
	m_strAdbDllePath = m_strModulePath.Left(m_strModulePath.ReverseFind(_T('\\'))+1)+_T("bin\\");
}

AdbOperator::~AdbOperator()
{
}

BOOL AdbOperator::InitAdbApi()
{
	g_pPluginLogObject->Record(_T("%s"),m_strAdbDllePath+ _T("AdbWinApi.dll"));
	if (!PathFileExists(m_strAdbDllePath + _T("AdbWinApi.dll")))
	{
		CString strSysDir=_T("");
		GetWindowsDirectory(strSysDir.GetBuffer(MAX_PATH),MAX_PATH);
		strSysDir.ReleaseBuffer();
		if (strSysDir.IsEmpty())
		{
			strSysDir = _T("c:\\windows\\system32\\");
		}
		else
		{
			strSysDir += _T("\\system32\\");
		}
		if (!PathFileExists(strSysDir + _T("AdbWinApi.dll")))
		{
			if (g_pPluginLogObject)
			{
				g_pPluginLogObject->Record(_T("ERROR:InitAdbApi-->PathFileExists failed"));
			}
			return FALSE;
		}
		m_hAdbwinapi = LoadLibrary(strSysDir + _T("AdbWinApi.dll"));
	}
	else
		m_hAdbwinapi = LoadLibrary(m_strAdbDllePath + _T("AdbWinApi.dll"));

	if (!m_hAdbwinapi)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:InitAdbApi-->LoadLibrary failed"));
		}
		return FALSE;
	}
	m_pAdbCloseHandle = (AdbCloseHandle)GetProcAddress( m_hAdbwinapi, "AdbCloseHandle");
	if (m_pAdbCloseHandle==NULL)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:InitAdbApi-->Get AdbCloseHandle failed"));
		}
		return FALSE;
	}

	m_pAdbEnumInterfaces = (AdbEnumInterfaces)GetProcAddress( m_hAdbwinapi, "AdbEnumInterfaces");
	if (m_pAdbEnumInterfaces==NULL)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:InitAdbApi-->Get AdbEnumInterfaces failed"));
		}
		return FALSE;
	}

	m_pAdbNextInterface = (AdbNextInterface)GetProcAddress( m_hAdbwinapi, "AdbNextInterface");
	if (m_pAdbNextInterface==NULL)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:InitAdbApi-->Get AdbNextInterface failed"));
		}
		return FALSE;
	}

	m_pAdbCreateInterface = (AdbCreateInterface)GetProcAddress( m_hAdbwinapi, "AdbCreateInterface");
	if (m_pAdbCreateInterface==NULL)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:InitAdbApi-->Get AdbCreateInterface failed"));
		}
		return FALSE;
	}

	m_pAdbOpenDefaultBulkReadEndpoint = (AdbOpenDefaultBulkReadEndpoint)GetProcAddress( m_hAdbwinapi, "AdbOpenDefaultBulkReadEndpoint");
	if (m_pAdbOpenDefaultBulkReadEndpoint==NULL)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:InitAdbApi-->Get AdbOpenDefaultBulkReadEndpoint failed"));
		}
		return FALSE;
	}
/*
	ADBAPIHANDLE adbHandle=NULL,pipeHandle=NULL;
	adbHandle = m_pAdbCreateInterface(usb_class_id,0x0bb4,0x0c02,0x1);
	if (adbHandle)
	{
		// 	 HANDLE hDevice = INVALID_HANDLE_VALUE;
		//  	 CString strDevPipe=_T("\\\\?\\usb#vid_0bb4&pid_0c02&mi_01#6&21ed2e15&0&0001#{f72fe0d4-cbcb-407d-8814-9ed673d0dd6b}\\BullkWrite");
		//  	 hDevice = CreateFile(strDevPipe,
		// 	 GENERIC_WRITE,
		//  	 FILE_SHARE_WRITE,
		// 	 NULL,
		//  	 OPEN_EXISTING,
		//  	 FILE_FLAG_OVERLAPPED,
		//  	 NULL);
		// 	 int iErr;
		// 	 iErr = GetLastError();
 		pipeHandle = m_pAdbOpenDefaultBulkReadEndpoint(adbHandle,AdbOpenAccessTypeReadWrite,AdbOpenSharingModeReadWrite);
 		m_pAdbCloseHandle(adbHandle);
 	}
	else
	{
		int iErr;
 		iErr = GetLastError();
	}
*/
	return TRUE;

}


VOID AdbOperator::DeInitAdbApi()
{
	if (m_hAdbwinapi)
	{
		FreeLibrary(m_hAdbwinapi);
		m_hAdbwinapi = NULL;
	}
	if (m_pAdbCloseHandle)
	{
		m_pAdbCloseHandle = NULL;
	}
	if (m_pAdbEnumInterfaces)
	{
		m_pAdbEnumInterfaces = NULL;
	}
	if (m_pAdbNextInterface)
	{
		m_pAdbNextInterface = NULL;
	}
}


int  AdbOperator::ScanAdbDevice()
{
	char entry_buffer[2048];
	char interf_name[2048];
	AdbInterfaceInfo* next_interface = (AdbInterfaceInfo*)(&entry_buffer[0]);
	unsigned long entry_buffer_size = sizeof(entry_buffer);
	char* copy_name;
	int nDeviceCount=0;
	// Enumerate all present and active interfaces.
	ADBAPIHANDLE enum_handle = m_pAdbEnumInterfaces(usb_class_id, true, true, true);

	if (NULL == enum_handle)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:ScanAdbDevice-->AdbEnumInterfaces failed,ErrCode(%d)"),GetLastError());
		}
		return nDeviceCount;
	}

	while (m_pAdbNextInterface(enum_handle, next_interface, &entry_buffer_size))
	{
		// TODO: FIXME - temp hack converting wchar_t into char.
		// It would be better to change AdbNextInterface so it will return
		// interface name as single char string.
		const wchar_t* wchar_name = next_interface->device_name;
		for(copy_name = interf_name;L'\0' != *wchar_name;wchar_name++, copy_name++) 
		{
			*copy_name = (char)(*wchar_name);
		}
		*copy_name = '\0';

		nDeviceCount++;

		entry_buffer_size = sizeof(entry_buffer);
	}
	m_pAdbCloseHandle(enum_handle);
	return nDeviceCount;
}

int AdbOperator::Exec( char** bufRawOutput, const CString adbCmd, int nTimeoutCount )
{
    char cmd[MAX_PATH+256];
    CString strCmd = m_strModulePath + _T("bin\\adb.exe ") + adbCmd;
    em::EmCharSet::Tcs2Mbs(cmd, (LPTSTR)(LPCTSTR)strCmd);
	g_pPluginLogObject->Record(strCmd);
    return em::EmProcess::SendCommand(bufRawOutput, cmd, nTimeoutCount);
}

BOOL AdbOperator::IsAdbDeviceOnline()
{
    char *online = "device";
    char *retBuf = NULL;
    LPTSTR lpStr = NULL;
    int nSize = 0;
    int nRet = 0;
   
    nRet = Exec(&retBuf, _T("devices"), 5);
    if (nRet > 0) {
        char *ptr;
        ptr = strstr(retBuf, online);
        if (ptr) {
            if (retBuf)
            {
                delete retBuf;
                retBuf = NULL;
            }
            return TRUE;
        }
    }

    if (retBuf)
    {
        AnsiToUnicode(lpStr,nSize,retBuf);
        g_pPluginLogObject->Record(_T("IsAdbDeviceOnline faild, return:%d, reson:%s"), nRet, lpStr);
        delete retBuf;
		delete lpStr;
        retBuf = NULL;
    } 
    else
    {
        g_pPluginLogObject->Record(_T("IsAdbDeviceOnline faild, return:%d"), nRet);
    }

    return FALSE;
}

int AdbOperator::ExecPackCmd(long nUserdataSize)
{
    char *ok = "PACK_OK";
    char *error = "PACK_ERROR";
    char *retBuf = NULL;
    LPTSTR lpStr = NULL;
    int nSize = 0;
    int nRet = 0;
	int i=0;

	g_pPluginLogObject->Record(_T("ExecPackCmd nUserdataSize:%ld"), nUserdataSize);
    CString cmd;

    cmd.Format(_T("shell /system/bin/packdata.sh %u"), nUserdataSize);
	nRet = Exec(&retBuf, cmd, 60*5);
	
    if (nRet > 0) {
        char *ptr;
        ptr = strstr(retBuf, ok);
        if (ptr) {
            if (retBuf) {
                delete retBuf;
                retBuf = NULL;
            }
            return 0;
        } else {
            LPWSTR wstrRetBuf;
            int nSize;
            if (AnsiToUnicode(wstrRetBuf,nSize,retBuf)) {
                g_pPluginLogObject->Record(wstrRetBuf);
                delete wstrRetBuf;
                wstrRetBuf = NULL;
            }
            if (retBuf) {
                delete retBuf;
                retBuf = NULL;
            }
            return -1;
        }
        if (retBuf) {
            delete retBuf;
            retBuf = NULL;
        }
        return TRUE;
    }

    if (retBuf)
    {
        AnsiToUnicode(lpStr,nSize,retBuf);
        g_pPluginLogObject->Record(_T("ExecPackCmd faild, return:%d, reson:%s"), nRet, lpStr);
        delete retBuf;
		delete lpStr;
        retBuf = NULL;
    } 
    else
    {
        g_pPluginLogObject->Record(_T("ExecPackCmd faild, return:%d"), nRet);
    }
    return -2;
}

BOOL AdbOperator::PullImage(CString strTargetImagePath)
{
    char *tag = "KB/s";
    char *retBuf = NULL;
    LPTSTR lpStr = NULL;
    int nSize = 0;
    int nRet = 0;

    CString cmd = _T("pull /mnt/sdcard/databk.img ");
    cmd += strTargetImagePath;
    nRet = Exec(&retBuf, cmd, 600);
	//g_pPluginLogObject->Record(_T("PullImage %s\n"),retBuf);
    if (nRet > 0) {
        char *ptr;
        ptr = strstr(retBuf, tag);
        if (ptr) {
            if (retBuf) {
                delete retBuf;
                retBuf = NULL;
            }
            return TRUE;
        }
    }
    if (retBuf)
    {
        AnsiToUnicode(lpStr,nSize,retBuf);
        g_pPluginLogObject->Record(_T("PullImage faild, return:%d, reson:%s"), nRet, lpStr);
        delete retBuf;
		delete lpStr;
        retBuf = NULL;
    } 
    else
    {
        g_pPluginLogObject->Record(_T("PullImage faild, return:%d"), nRet);
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// AdbOperator message handlers
