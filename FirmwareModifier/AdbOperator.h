#if !defined(AFX_ADBOPERATOR_H__5DEB1F22_4BE7_4C25_9541_D4F238676DDC__INCLUDED_)
#define AFX_ADBOPERATOR_H__5DEB1F22_4BE7_4C25_9541_D4F238676DDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdbOperator.h : header file
//

typedef void* ADBAPIHANDLE;

typedef struct _AdbInterfaceInfo {
	/// Inteface's class id (see SP_DEVICE_INTERFACE_DATA for details)
	GUID          class_id;
	
	/// Interface flags (see SP_DEVICE_INTERFACE_DATA for details)
	unsigned long flags;
	
	/// Device name for the interface (see SP_DEVICE_INTERFACE_DETAIL_DATA
	/// for details)
	wchar_t       device_name[1];
} AdbInterfaceInfo;

typedef enum _AdbOpenAccessType {
	/// Opens for read and write access.
	AdbOpenAccessTypeReadWrite,
		
		/// Opens for read only access.
		AdbOpenAccessTypeRead,
		
		/// Opens for write only access.
		AdbOpenAccessTypeWrite,
		
		/// Opens for querying information.
		AdbOpenAccessTypeQueryInfo,
} AdbOpenAccessType;


typedef enum _AdbOpenSharingMode {
	/// Shares read and write.
	AdbOpenSharingModeReadWrite,
		
		/// Shares only read.
		AdbOpenSharingModeRead,
		
		/// Shares only write.
		AdbOpenSharingModeWrite,
		
		/// Opens exclusive.
		AdbOpenSharingModeExclusive,
} AdbOpenSharingMode;

typedef bool (__cdecl *AdbCloseHandle)(ADBAPIHANDLE); 
typedef ADBAPIHANDLE (__cdecl *AdbEnumInterfaces)(GUID,bool,bool,bool); 
typedef bool (__cdecl *AdbNextInterface)(ADBAPIHANDLE,AdbInterfaceInfo*,unsigned long*); 
typedef ADBAPIHANDLE (__cdecl *AdbCreateInterface)(GUID,unsigned short,unsigned short,unsigned char); 
typedef ADBAPIHANDLE (__cdecl *AdbOpenDefaultBulkReadEndpoint)(ADBAPIHANDLE,AdbOpenAccessType,AdbOpenSharingMode);

/////////////////////////////////////////////////////////////////////////////
// AdbOperator window

class AdbOperator
{
// Construction
public:
	AdbOperator();

// Attributes
public:

// Operations
public:
	HMODULE m_hAdbwinapi;
	AdbCloseHandle m_pAdbCloseHandle;
	AdbEnumInterfaces m_pAdbEnumInterfaces;
	AdbNextInterface m_pAdbNextInterface;
	AdbCreateInterface m_pAdbCreateInterface;
	AdbOpenDefaultBulkReadEndpoint m_pAdbOpenDefaultBulkReadEndpoint;

	BOOL InitAdbApi();
	VOID DeInitAdbApi();
	int  ScanAdbDevice();
    BOOL IsAdbDeviceOnline();
    int ExecPackCmd(long nUserdataSize);
    BOOL PullImage(CString strTargetImagePath);
	virtual ~AdbOperator();
    
	// Generated message map functions
protected:
private:
    CString m_strModulePath;
	CString m_strAdbDllePath;
    int Exec(char** bufRawOutput, const CString adbCmd, int nTimeoutCount);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADBOPERATOR_H__5DEB1F22_4BE7_4C25_9541_D4F238676DDC__INCLUDED_)
