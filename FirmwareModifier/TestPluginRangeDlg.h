#if !defined(AFX_TESTPLUGINRANGEDLG_H__14601DD0_DD7B_4338_85EE_98AD024C6C53__INCLUDED_)
#define AFX_TESTPLUGINRANGEDLG_H__14601DD0_DD7B_4338_85EE_98AD024C6C53__INCLUDED_

#include "SysCfgDlg.h"	// Added by ClassView
#include "SysCfgDtc.h"
#include "ChargeImage.h"
#include "DlgBatteryParam.h"
#include "cmMfcLib/cmLog.h"
#include "cmMfcLib/cmIniFile.h"

#include "EmCharSet.h"
#include "EmAdbScanner.h"
#include "EmProcess.h"

#include "AdbOperator.h"
#include "MySpawnConsumer.h"

using namespace cm;
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestPluginRangeDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTestPluginRangeDlg dialog
typedef struct  
{
	PROCESS_INFORMATION  param1;
	int   param2;
}THREADARG,*PTHREADARG;

/*
typedef struct
{
    CString strPartitionName;
    int length;
    int address;
}PARTITION_INFO;
*/

// Simple CObject-derived class for CObList examples
class CPartitionInfo : public CObject
{
public:
    int length;
    int address;
    CString strPartitionName;
    CPartitionInfo() { length = 0; address = 0; strPartitionName = _T("");}
};

class CTestPluginRangeDlg : public CDialog
{
    // Construction
public:
    CString m_strAdbVid;
    CString m_strAdbPid;
	CString m_strNewImagePath;
	CString strSystemPath;
	CString strTempPath;
	CString strRootPath;
    CString m_strMsgBoxTitle;
	CString strRecoverInitRcPath; 
	CString strBootInitRcPath;
	CString strBootInitBoardRcPath;
	CString strParameterPath;
    CString strPackageFilePath;
	CString strFactoryPath;
	CString strBuildPropPath;
	CString strDefaultPropPath;
	CString strSystemConfigPath;
	CString strCovertToBMPPath;
	CString strCovertToPPMPath;
	CString strCovertToPressionBMPPath;
    CString strKernelPath;
	CString strMiscPath;
	CString strResourcePath;
    CString strResourceUbootLogoPath;
	CString strResourceBatteryPath;
	CString strResourceKernelLogoPath;
    CString strBootAnimationBasePath;
    CString strShutdownAnimationBasePath;
    CString strDefaultWallpaperPath;
    CString strAnimationUnZipPath;
	CString strInitSystemConfiginiPath;
	CString strInitConfiginiPath;
	CString TemporaryFolderPath;
	CString strMoudlePath;
	CString strConfigPath;
	CString strBootRingtonePath;
	CString strShutdownRingtonePath;
	CString strResourceDtsPath;
	CString strResourceDtbPath;
	CString strFsBootTabPath;
	CString strFsRamTabPath;
	CString strFsPackageFileTabPath;
	
	CString strCmdline;
	CString strInitModifyTime;
	CString strAdcChannel;
	CString strPmicType;
	CString strTpName;
	CString strTpCmdline; 
    CString m_strModulePath;

    CStringList m_AnimationPathList;
    POSITION m_AnimationPos;

    int RKChipType;
    WORD wKernelPictureWidth;
    WORD wKernelPictureHeight;
	WORD wKernelMaxPictureWidth;
    WORD wKernelMaxPictureHeight;
    int KernelPictureType;
    unsigned int nLanguagesCount;
    ULONGLONG mDatabkImageSize;
    BOOL needDisplayBootanimation;
    BOOL needDisplayDefaultWallpaper;
    BOOL isExistAnroidAnimationZip;
    BOOL isReadubootLogo;
	BOOL isReadkernelLogo;
	BOOL isReadkernelDtb;
	BOOL isOpenedSysconfigIni;
	BOOL isSystemConfigIniWrited;
    AdbOperator m_AdbOperator;
    CObList mPartitionList;

	CString strInitParameterTime;
	CString strInitResourceDtbTime;

	int m_androidVersion;
	int m_kernelVersion;

	int filecount_list;
    
    CTestPluginRangeDlg(CWnd* pParent = NULL);   // standard constructor
    ~CTestPluginRangeDlg();
    DWORD GetDirSize(CString strFullPath);
	int ReadParameterFile();
	int ReadBuildpropFile();
	int ReadRecoverInitRcFile();
	int ReadBootInitRcFile();
	int ReadDefaultProp();

    int LoadFirmware(void);
	int LoadResource(void);
	int LoadAnimation();
	int LoadWallopaper(); 
    int CheckDisplayPictrueSize(CString filePath,int imagetype);
	int CheckDisplayPictrueType(CString filePath);
	bool runExternalExe(CString strCmd,int mode,int type);
	bool CreateTheard(PTHREADARG pArg);
	bool CovertPicture(CString srcfilepath,CString decfilepath,int srcimagetype,int decimagetype);
	bool Covert224PNM(CString srcfilepath,CString decfilepath);
	bool CovertPPM(CString srcfilepath,CString decfilepath);
    int DisplayPictrue(CString filePath,bool isFullScreen,int isbootlogo);
    int SelectPlatformForInitRc();
    void ImportAnimation(CString AnimationPath, EMANIMATIONS animType); 
    CString GetFileTitleFromFileName(CString FileName,CString& ExtendName);
    CString SetFileTitleFromFileName(CString FileName,CString ExtendName);

	void GetComlineParameter();
    void GetRecoveryInitRcParameter();
	void GetBuildPropParameter();

    CString ExtractCmdlinePartitionsInfo();
    DWORD GetProperPartitionSectorSize(CString strImagePath);
	DWORD GetProperPartitionSectorSize(DWORD dwFileSize);
    // ReplaceCStringListByKeywords(comline_list, "mtdparts", "xxxxxxxx")
    // it will find "mtdparts" in parameter, and then replace it with "xxxxx"
    BOOL ReplaceCStringListByKeywords(CStringList& strList, CString keywords, CString content);
    BOOL ParsePartitionsInfo(CString strParitions);
    void ModifyPartitionSize(CString strPartitionName, int nSectorSize);
    void ModifyPartitionSizeInternal(CString strPartitionName, int nSectorSize);
    BOOL ParseOnePartition(CString strOnePartition, CPartitionInfo* info);
    BOOL PartitionExist(CString strPartitionName);
    void AddPartition(CString strPartitionName, int nSectorSize);
    CString GeneratePartitionsInfo();
    BOOL FlushPartitionsToParameterList();
    int ReadPackageFile();
    int ConvertCStringoHex(CString Data);
    void ModifyPackageFile();
    int GetPartitionSizeInBytes(CString strPartitionName);
	void PartitionListFree();
	BOOL UpdateFstab();


	void ReadDvfsToSystemConfigIni(CString strSrc); 
	void ReadPmicToSystemConfigIni(CString strSrc); 
	void ReadRtcToSystemConfigIni(CString strSrc);
	//void ReadTpToSystemConfigIni(); 
	void ReadSystemConfigIni(CString strSrc);
	void ReadTpToSystemConfigIni(CString strSrc); 
	void ReadLcdToSystemConfigIni(CString strSrc);
	void ReadKeyToSystemConfigIni(CString strSrc);
	void ReadCodecToSystemConfigIni(CString strSrc);
	void ReadPwmToSystemConfigIni(CString strSrc);
	void ReadBLToSystemConfigIni(CString strSrc);
	void ReadSensorToSystemConfigIni(CString strSrc,int sensortype);
	void ReadCameraToSystemConfigIni(CString strSrc);
	void ReadUsbToSystemConfigIni(CString strSrc);
	void ReadSDToSystemConfigIni(CString strSrc);
	void ReadWifiToSystemConfigIni(CString strSrc);
	void ReadChargeToSystemConfigIni(CString strSrc);
	void ReadIonToSystemConfigIni(CString strSrc); 
	
	void WriteSystemConfig(CString strSysConfigName,CString strKey,CString strSrc,int mode);
	CString CovertToUserMode(CString strSysConfigName,int mode);
	void WriteSystemConfigIniFile(CString strSysConfigName,CString strSrc,int mode); 
	void ReadParamToSystemConfigIni();


    CString ConvetToCmdMode(CString strSrc,int mode);
	void WriteParamerFile(CString strSrc,CString strKey,int mode);
	void WriteComlinetoParamerFile();
	void WriteSystemConfigToRecoveryInitRc();
	void WriteSystemConfigToBootInitRc();
	void WriteSystemConfigToParameter();
	void WriteSystemConfigToBuildProp();
	void WriteTpSystemConfigToBootInitRc();
	void WriteSystemConfigToDefaultProp();
	void WriteMiscCommand();
	void CopyKoToSystemMoudle(CString strName);
	void RemoveCmdlineConfig();

	void ModifyParameterFile();
    BOOL ModifyPartitionsParameter();


    void EnableUIControls();
	void DisableUIControls();
	void RemoveTempFolder();
	void RemoveTempFolderFile();

	void InitSystemPath();
	void InitVariable();

	int CovertCompressionBmp(CString srcPath,CString desPath); 
	DWORD GetCompressionFileSize(CString mPath);

	DWORD GetDtbFileSize(CString mPath); 

    // Dialog Data
    //{{AFX_DATA(CTestPluginRangeDlg)
	enum { IDD = IDD_DIALOG_TEST_PLATFORM };
	CProgressCtrl	m_progressClone;
    CListCtrl	m_sysApksList;
    CListCtrl	m_preInstApksList;
    CTabCtrl	m_tabApks;
    CTabCtrl	m_tabLogo;
	BOOL	m_chkBlueTooth;
	//}}AFX_DATA
    
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTestPluginRangeDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL
    
    // Implementation
protected:
    int m_nCurrenListIndex;
    int m_nCurrentSize;
    CString m_strAppsPath;
	CString m_priAppsPath;
	CString m_strAppsPrePath;
    //CImageList m_ImageList; //this define for list control
    // Generated message map functions
    //{{AFX_MSG(CTestPluginRangeDlg)
	afx_msg void OnUnPackFinishMsg(WPARAM wParam, LPARAM lParam); 
    afx_msg void OnPackFinishMsg(WPARAM wParam, LPARAM lParam); 
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnPaint();
    afx_msg void OnReplaceBootLogo();
    afx_msg void OnReplaceBootAnimation();
    afx_msg void OnSelchangeTabLogo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReplaceDefaultWallpaper();
    afx_msg void OnSelchangeTabApk(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRclickListPreinstApks(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRclickListSysApks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnModifySysConfig();
	afx_msg void OnReplaceShutdownAnimation();
	afx_msg void OnBtnConfirmModify();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnModifyBuildProp();
	afx_msg void OnBtnModifyKeyMap();
	afx_msg void OnReplaceChargeLogo();
	afx_msg void OnBtnModifyBatteryParam();
	afx_msg void OnBtnClone();
	afx_msg void OnReplaceBootRing();
	afx_msg void OnBtnModifyKernelParameter();
	afx_msg void OnDestroy();
    afx_msg LRESULT OnUpdateCloneBtnMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReplaceKernelLogo();
	afx_msg void OnCheckEnableUserFormat();
	afx_msg void OnReplaceShutdownRing();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CBrush m_brBackBrush;
    CString m_strLogPath;
    cmIniFile m_iniConfig;
    CNotePadBase* m_dlgNotePad;
    CChargeImage* m_dlgChargeImage;
    CDlgBatteryParam* m_dlgBatteryParam;

    BOOL LoadConfig();
	void InitApkConfigGroupBox();
	void InitLogoConfigGroupBox();
	void InitSysStatusCloneGroupBox();
	void InitTimeZone();
	void InitSystemSettingsGroupBox();
	void InitUsbConfigGroupBox();
	void InitSystemInfoGroupBox();
	void InitLanguagesList();
	void InitUIComponents();
	void LocalizeMenuCaptions(CMenu* pPopupMenu);
    void ParseApps();
	void BrowseCurrentAllFile(CString strDir,CListCtrl &ApksList);
    int FindMenuItem(CMenu* Menu, LPCTSTR MenuString);
    CString DetermineBootInitRcPath();
    CString GetBootChargeImagePath();
};
typedef struct
{
    CString key;
    CString value;
}KEY_WORD;

BOOL AnsiToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc);
BOOL UnicodeToAnsi(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc);
BOOL U8ToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc);
BOOL UnicodeToU8(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc);
BOOL RunOhterExe(CString strCmd,CString *strResult,ENUM_STRING_CODE emCode=SC_ANSI);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPLUGINRANGEDLG_H__14601DD0_DD7B_4338_85EE_98AD024C6C53__INCLUDED_)
