// PortApkToolDlg.h : header file
//

#if !defined(AFX_PORTAPKTOOLDLG_H__B97201BF_634E_4C19_976F_3A6C5B079A1C__INCLUDED_)
#define AFX_PORTAPKTOOLDLG_H__B97201BF_634E_4C19_976F_3A6C5B079A1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AES.h"
#include "MySpawnConsumer.h"
#include "KCBusyProgressCtrl.h"
#include "CommonDef.h"
#include "cmIniFile.h"
#include "cmMultiLanguage.h"
#include "cmLog.h"
#include "cmStrCode.h"
using namespace cm;
typedef enum{
	CMD_NONE=0,
	CMD_PACK_UNION,
	CMD_UNPACK_UNION,
	CMD_PACK_ANDROID,
	CMD_UNPACK_ANDROID,
	CMD_PACK_CRAMFS,
	CMD_UNPACK_CRAMFS,
	CMD_UNPACK_BOOT,
	CMD_UNPACK_RAMDISK,
	CMD_REPLACE_BOOT_LOGO,
	CMD_REPLACE_RAMDISK_LOGO,
	CMD_PACK_BOOT,
	CMD_PACK_RAMDISK,
	CMD_UNPACK_RECOVERY,
	CMD_UNPACK_KERNELRECOVERY,
	CMD_PACK_RECOVERY,
	CMD_PACK_KERNELRECOVERY,
	CMD_PACK_EXT
}ENUM_CMD_TYPE;

typedef enum{
	SYSTEM_ANDROID_FOUR_TWO=0,
	SYSTEM_ANDROID_FOUR_FOUR
}ENUM_ANDROID_VERSION;
/////////////////////////////////////////////////////////////////////////////
// CPortApkToolDlg dialog

class CPortApkToolDlg : public CDialog
{
// Construction
public:
	CPortApkToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPortApkToolDlg)
	enum { IDD = IDD_PORTAPKTOOL_DIALOG };
	CKCBusyProgressCtrl	m_progress;
	CStatic	m_labelInfo;
	CTabCtrl	m_tabExtensions;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortApkToolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPortApkToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnButtonImage();
	afx_msg void OnButtonApk();
	afx_msg void OnButtonUnpack();
	afx_msg void OnButtonPack();
	afx_msg LRESULT OnTestRunMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetDirMsg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStartPackMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSelchangeTABExtension(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strModulePath;
	CString m_strPrevApkDir;
	CString m_outputDir;
	CString m_strPortImage;
	CString m_strPortApk;
	CString m_strKey;
	CString m_strBootResFile;
	CString m_strBootFile;
	CString m_strRecoveryFile;
	CString m_strKernelFile;
	int m_androidVersion;
	BOOL m_bIsPortDIr;
	VECTOR_EXTENSION m_vecExtension;
	CWinThread *m_pActionThread;
	cmMultiLanguage *m_pLangObject;
	VOID SwitchCtrl(BOOL bOn=TRUE);
	BOOL GetImageInfo(BOOL &bIsUnionFw,CString &strDeviceType);
	VOID OutputInfo(CString strInfo,BOOL bShow=TRUE);
	BOOL CheckKernel(CString strKernel,CString strKey);
private:
	CString m_strLogPath;
	CString m_strPluginPath;
	CString m_LangPath;
	CString m_strVersion;
	cmIniFile m_iniConfig;
	LANG_PROP_VECTOR m_langProp;
	CHAR m_curLang;	
	BOOL LoadConfig();
	BOOL LoadExtension(STRUCT_EXTENSION &extension);
	VOID SetTabPanel(UINT uiTabIndex);
};
BOOL DeleteDir(CString strDir);
CString BrowseForFolder(HWND wndHandle,CString strInitialDir,CString strTitle);
BOOL OpenFileDialog(CString strFilter,CString &strFileName);

UINT PackProc( LPVOID pParam );
UINT UnpackProc( LPVOID pParam );

BOOL RunOhterExe(CString strCmd,ENUM_STRING_CODE emCode=SC_ANSI);
BOOL WideCharToChar(CString strWideChar,char *pChar,int nCharBufferLen);
BOOL CheckExtBoot(CString strBootPath);
BOOL CheckExtImage(CString strExtImage);
BOOL UnpackKernel(CString strKernelFile,CString strOutputFile);
int ParsePackageFile(CString strFilePath,VECTOR_PACKAGE_SET &packageSet);
int FindItem(VECTOR_PACKAGE_SET &packageSet, const char* name);
BOOL GetAbsolutePath(CString strRelativePath,CString &strAbsolutePath);

CString GetLocalString(CString strKey);
CString GetChipValueFromDword(DWORD dwChipType);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTAPKTOOLDLG_H__B97201BF_634E_4C19_976F_3A6C5B079A1C__INCLUDED_)
