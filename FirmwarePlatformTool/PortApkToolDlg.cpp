// PortApkToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PortApkTool.h"
#include "PortApkToolDlg.h"
#include <comutil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
cmLog *g_pLogObject=NULL;
CString g_strModulePath = _T("");
CString g_strBinDir = _T("");
CString g_strRKImageMaker= _T("");
CString g_strAfptool = _T("");
CString g_strMKcramfs = _T("");
CString g_strCramfsCK = _T("");

CString g_strReplaceKernelExe = _T("");
CString g_strMKkrnlimg = _T("");
CString g_strUnpackBootExe = _T("");
CString g_strUnpackExtExe = _T("");
CString g_strPackExtBat = _T("");

CString g_strUnpackBootImgBat = _T("");
CString g_strUnpackRamdiskBat = _T("");
CString g_strReplaceBootLogoBat = _T("");
CString g_strReplaceRamdiskLogoBat = _T("");
CString g_strPackBootImgBat = _T("");
CString g_strPackRamdiskBat = _T("");
CString g_strMkbootimg = _T("");
CString g_strUnpackRecoveryBat = _T("");
CString g_strUnpackKernelRecoveryBat = _T("");
CString g_strPackRecoveryBat = _T("");
CString g_strPackKernelRecoveryBat = _T("");
BOOL g_bCheckKernel=FALSE;
CPortApkToolDlg *g_pMainDlg = NULL;

CString m_strUpdateFile;


/////////////////////////////////////////////////////////////////////////////
// CPortApkToolDlg dialog

CPortApkToolDlg::CPortApkToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortApkToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortApkToolDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPortApkToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortApkToolDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_LABEL_INFO, m_labelInfo);
	DDX_Control(pDX, IDC_TAB_Extension, m_tabExtensions);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPortApkToolDlg, CDialog)
	//{{AFX_MSG_MAP(CPortApkToolDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, OnButtonImage)
	ON_BN_CLICKED(IDC_BUTTON_APK, OnButtonApk)
	ON_BN_CLICKED(IDC_BUTTON_UNPACK, OnButtonUnpack)
	ON_BN_CLICKED(IDC_BUTTON_PACK, OnButtonPack)
	ON_MESSAGE(WM_TEST_RUNNING,OnTestRunMsg)
	ON_MESSAGE(WM_GET_DIR,OnGetDirMsg)
	ON_MESSAGE(WM_START_PACK,OnStartPackMsg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_Extension, OnSelchangeTABExtension)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortApkToolDlg message handlers
CString GetChipValueFromDword( DWORD dwChipType )
{
    CString strChipType;
    DWORD dwTemp = 0;
    for (int i=0; i<4; i++)
    {
        dwTemp = dwChipType >> (3-i)*8;
        dwTemp &= 0x000000FF;
        TCHAR ch = (TCHAR)dwTemp;
        strChipType += ch;
    }
    return strChipType;
}

BOOL CPortApkToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	if (__argc>1)
	{
		CString strCmdLine;
		strCmdLine = GetCommandLine();
		strCmdLine.MakeLower();
		if (strCmdLine.Find(_T("-nocheck"))!=-1)
		{
			g_bCheckKernel = FALSE;
		}
	}

	g_pMainDlg = this;
	//设置路径变量
	GetModuleFileName(NULL, m_strModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	m_strModulePath.ReleaseBuffer();
	m_strModulePath = m_strModulePath.Left(m_strModulePath.ReverseFind(_T('\\'))+1);
	
	g_strModulePath = m_strModulePath;
	m_strPrevApkDir = m_strModulePath;
	g_strBinDir = m_strModulePath + _T("bin\\");
	g_strRKImageMaker = m_strModulePath +_T("bin\\RKImageMaker.exe");
	g_strAfptool = m_strModulePath +_T("bin\\AFPTool.exe");
	g_strMKcramfs = m_strModulePath +_T("bin\\mkcramfs.exe");
	g_strCramfsCK = m_strModulePath +_T("bin\\cramfsck.exe");
	g_strReplaceKernelExe = m_strModulePath + _T("bin\\logo.exe");
	g_strMKkrnlimg = m_strModulePath + _T("bin\\mkkrnlimg.exe");
	g_strUnpackBootExe = m_strModulePath + _T("bin\\unBootImg.exe");
	g_strUnpackExtExe = m_strModulePath + _T("bin\\ext2fsckft.exe");
	g_strPackExtBat = m_strModulePath + _T("bin\\PackExtImageft.bat");	
	g_strUnpackBootImgBat = m_strModulePath + _T("bin\\UnpackBootImg.bat");
	g_strUnpackRamdiskBat = m_strModulePath + _T("bin\\UnpackRamdisk.bat");
	g_strReplaceBootLogoBat = m_strModulePath + _T("bin\\ReplaceBootLogo.bat");
	g_strReplaceRamdiskLogoBat = m_strModulePath + _T("bin\\ReplaceRamdiskLogo.bat");
	g_strPackBootImgBat = m_strModulePath + _T("bin\\PackBootImg.bat");
	g_strPackRamdiskBat = m_strModulePath + _T("bin\\PackRamdisk.bat");

	g_strUnpackRecoveryBat = m_strModulePath + _T("bin\\UnpackRecovery.bat");
	g_strUnpackKernelRecoveryBat = m_strModulePath + _T("bin\\UnpackKernelRecovery.bat");
	g_strPackRecoveryBat = m_strModulePath + _T("bin\\PackRecovery.bat");
	g_strPackKernelRecoveryBat = m_strModulePath + _T("bin\\PackKernelRecovery.bat");

	g_strMkbootimg = m_strModulePath + _T("bin\\mkbootimg.exe");

	m_strBootFile = m_strModulePath + _T("Temp\\Android\\Image\\boot.img");
	m_strRecoveryFile = m_strModulePath + _T("Temp\\Android\\Image\\recovery.img");
	m_strKernelFile = m_strModulePath + _T("Temp\\Android\\Image\\kernel.img");

	m_strLogPath = m_strModulePath + _T("Log\\");
	m_outputDir = m_strModulePath + _T("Output\\");
	m_strPluginPath = m_strModulePath + _T("Plugin\\");
	if ( !PathFileExists(m_outputDir) )
	{
		CreateDirectory(m_outputDir,NULL);
	}
	if ( !PathFileExists(m_strPluginPath) )
	{
		CreateDirectory(m_strPluginPath,NULL);
	}
	//创建日志对象
	if ( !PathFileExists(m_strLogPath) )
	{
		CreateDirectory(m_strLogPath,NULL);
	}

	BOOL bRet;
	g_pLogObject = NULL;
	g_pLogObject = new cmLog(m_strLogPath,bRet);
	if (bRet)
	{
		g_pLogObject->LogFileName = _T("Log");
	}


	m_pActionThread = NULL;
	m_pLangObject = NULL;
	m_vecExtension.clear();

	if (LoadConfig())
	{
		m_pLangObject = new cmMultiLanguage(m_LangPath,m_langProp,g_pLogObject,bRet);
		if (bRet)
		{
			m_pLangObject->CurrentLang = m_curLang;
			STRUCT_DIALOG_PROPERTY prop;
			prop.hwnd = m_hWnd;
			prop.id = IDD_PORTAPKTOOL_DIALOG;
			m_pLangObject->AddDialogProp(prop);
			bRet = m_pLangObject->SetLanguageString();

			RECT	rcTabCtrl;
			m_tabExtensions.GetClientRect(&rcTabCtrl);
			rcTabCtrl.top		+= 22;
 			rcTabCtrl.left		+= 2;
 			rcTabCtrl.bottom	-= 5;
 			rcTabCtrl.right		-= 5;

			char szTabTitle[MAX_PATH];
			int i;
			for (i=0;i<m_vecExtension.size();i++)
			{
				m_vecExtension[i].pSetMsgHandle((DWORD)m_hWnd);
				m_vecExtension[i].pSetWindowLanguage(m_curLang+1);
				if (m_vecExtension[i].pGetTabTitle(szTabTitle))
				{
					LPWSTR lpTabTitle;
					int nTabTitleSize;
					bRet = cmStrCode::AnsiToUnicode(lpTabTitle,nTabTitleSize,szTabTitle);
					if (bRet)
					{
						m_vecExtension[i].strTabTitle = lpTabTitle;
						delete []lpTabTitle;
						m_tabExtensions.InsertItem(i,m_vecExtension[i].strTabTitle);
						m_vecExtension[i].childHwnd = m_vecExtension[i].pCreateChildWindow((DWORD)m_tabExtensions.m_hWnd);
						if (m_vecExtension[i].childHwnd!=0)
						{
							CWnd *pChildWnd = CWnd::FromHandle((HWND)m_vecExtension[i].childHwnd);
							pChildWnd->MoveWindow(&rcTabCtrl,TRUE);
							pChildWnd->ShowWindow(SW_HIDE);
						}
					}
				}
			}
			if (m_tabExtensions.GetItemCount()>0)
			{
				SetTabPanel(0);
			}
		}
		else
		{
			MessageBox(_T("Loading Multi Language failed!"),_T("ERROR"),MB_ICONERROR|MB_OK);
		}
	}
	else
	{
		MessageBox(_T("Loading config file failed!"),_T("ERROR"),MB_ICONERROR|MB_OK);
	}
	m_progress.SetMode();
	m_progress.SetNumSteps(20);
	m_progress.SetSpeed(100);

	CString strTitleText;
	GetWindowText(strTitleText);
	if (m_strVersion.IsEmpty())
	{
		strTitleText += APP_VERSION;
	}
	else
		strTitleText += m_strVersion;
	SetWindowText(strTitleText);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPortApkToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPortApkToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL DeleteDir(CString strDir)
{
	TCHAR tzDir[MAX_PATH];
	memset(tzDir,0,MAX_PATH*sizeof(TCHAR));
	
	if ((strDir.Right(1)==_T("/"))||(strDir.Right(1)==_T("\\")))
	{
		strDir = strDir.Left(strDir.GetLength()-1);
	}
	
	_tcscpy(tzDir,strDir);
	
	
	SHFILEOPSTRUCT FileOP;					//声明文件操作结构体
	FileOP.hwnd = NULL;		//句柄
	FileOP.fFlags = FOF_SILENT | FOF_NOERRORUI |FOF_NOCONFIRMATION;//FOF_SILENT;//FOF_NOCONFIRMATION;
	FileOP.wFunc = FO_DELETE;					//操方式
	FileOP.pFrom = tzDir;				//源地址
	FileOP.pTo = NULL;			//目的地址
	FileOP.fAnyOperationsAborted = FALSE;	//是否允许中断操作
	FileOP.hNameMappings = NULL;
	FileOP.lpszProgressTitle = NULL;
	
	
	
	int MSG = SHFileOperation(&FileOP);		//执行操作
	
	if(0 != MSG)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("DeleteDirAndFile-->SHFileOperation failed,dir(%s),Err(%d),Ret(%d)"),tzDir,GetLastError(),MSG);
		}
		return FALSE;
	}
	return TRUE;
}

void CPortApkToolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pActionThread)
	{
		MessageBox(GetLocalString(_T("MESSAGE_RUNNING_NOW")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (g_pLogObject)
	{
		delete g_pLogObject;
		g_pLogObject = NULL;
	}
	if (m_pLangObject)
	{
		delete m_pLangObject;
		m_pLangObject = NULL;
	}
	int i;
	BOOL bRet;
	for (i=0;i<m_vecExtension.size();i++)
	{
		if (m_vecExtension[i].childHwnd!=0)
		{
			m_vecExtension[i].pDestroyChildWindow(m_vecExtension[i].childHwnd);
		}
		bRet = FreeLibrary(m_vecExtension[i].hDll);
		m_vecExtension[i].hDll = NULL;
	}
	m_vecExtension.clear();
	CDialog::OnClose();
}
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg==BFFM_INITIALIZED)
	{
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
	}
	return 0;
}
CString BrowseForFolder(HWND wndHandle,CString strInitialDir,CString strTitle)
{
	TCHAR  szDisplayName[MAX_PATH] = _T("");
	TCHAR  szPath[MAX_PATH] = _T("");
	BROWSEINFO bi;
	bi.hwndOwner = wndHandle;
	bi.pidlRoot = NULL;
	bi.lpszTitle = (LPTSTR)(LPCTSTR)strTitle;
	bi.pszDisplayName = szDisplayName;
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	bi.lpfn  = BrowseCallbackProc;
	
	bi.lParam = (LPARAM)(LPTSTR)(LPCTSTR)strInitialDir;
	
	LPITEMIDLIST pItemIDList = SHBrowseForFolder( &bi );
	if( pItemIDList )
	{
		SHGetPathFromIDList(pItemIDList,szPath) ;
		
		IMalloc *pMalloc;
		if( SHGetMalloc( &pMalloc ) != NOERROR )
		{
			return szPath;
		}
		pMalloc->Free( pItemIDList );
		if( pMalloc )
			pMalloc->Release();
	}
	return szPath;
}
BOOL OpenFileDialog(CString strFilter,CString &strFileName)
{
	int		structsize = 0;
	
	CFileDialog dlgFileOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY, strFilter);
	
	structsize = sizeof(OPENFILENAME);
	TCHAR lpstrFilename[MAX_PATH] = _T("");
	dlgFileOpen.m_ofn.lpstrFile = lpstrFilename; 
	dlgFileOpen.m_ofn.nMaxFile  = MAX_PATH; 
	dlgFileOpen.m_ofn.lStructSize=structsize;
	
	if(dlgFileOpen.DoModal()==IDOK)
	{
		strFileName = dlgFileOpen.GetPathName();
	}
	else
		return FALSE;
	
	return TRUE;
}

void CPortApkToolDlg::OnButtonImage() 
{
	// TODO: Add your control notification handler code here

	CString strFilter,strImageFile;
	strFilter = _T("Image Files(*.img)|*.img||");
	BOOL bRet;
	bRet = OpenFileDialog(strFilter,strImageFile);
	if (!bRet)
	{
		return;
	}
	SetDlgItemText(IDC_EDIT_IMAGE_PATH,strImageFile);
	
	OnButtonUnpack();

}

void CPortApkToolDlg::OnButtonApk() 
{
	// TODO: Add your control notification handler code here

	CString strApkDir;
	strApkDir = BrowseForFolder(m_hWnd,m_strPrevApkDir,_T(""));
	if (strApkDir.IsEmpty())
	{
		return;
	}
	DWORD dwRet = GetFileAttributes(strApkDir);
	if (dwRet&FILE_ATTRIBUTE_DIRECTORY)
	{
		m_strPrevApkDir = strApkDir;
	}
	else
		m_strPrevApkDir = strApkDir.Left(strApkDir.ReverseFind(_T('\\'))+1);
	// TRACE(_T("%s\r\n"),strApkDir);
	SetDlgItemText(IDC_EDIT_APK_DIR,strApkDir);
}

VOID CPortApkToolDlg::SwitchCtrl(BOOL bOn)
{
	GetDlgItem(IDC_BUTTON_IMAGE)->EnableWindow(bOn);
	//GetDlgItem(IDC_BUTTON_PACK)->EnableWindow(bOn);
	//GetDlgItem(IDC_BUTTON_UNPACK)->EnableWindow(bOn);
}
UINT PackProc( LPVOID pParam )
{
	CPortApkToolDlg *pDlg = (CPortApkToolDlg *)pParam;
	int iRet=0,iPos,nSize;
	LPTSTR lpDst;
	BOOL bIsUnionFW,bRet;	
	CString strDeviceType,strUnionDir,strAndroidDir,strUnionBoot,strUnionFW,strSystemImage,strSystemDir,strCmd;
	CString strOutputFile,strTmpSystemFile,strBootImage,strResouceImage,strKernelImage,strRamdiskImage,strKernelSrc,strBootDir;
	CString strRecoveryImage,strRecoveryDir,strTmpRecovery,strRelativePath;
	VECTOR_PACKAGE_SET packetSet;

	if (!pDlg->GetImageInfo(bIsUnionFW,strDeviceType))
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:PackProc-->GetImageInfo failed"));
		}
		iRet = -1;
		goto EXIT_PACK;
	}
	strUnionDir = pDlg->m_strModulePath + _T("Temp");
	strAndroidDir = strUnionDir + _T("\\Android");
	strSystemDir = strUnionDir + _T("\\System");
	strBootDir = strUnionDir + _T("\\Boot");
	strRecoveryDir = strUnionDir + _T("\\Recovery");
	strUnionBoot = strUnionDir + _T("\\boot.bin");
	strUnionFW = strUnionDir + _T("\\firmware.img");
	strSystemImage = strAndroidDir + _T("\\Image\\system.img");
	strOutputFile = m_strUpdateFile;//pDlg->m_outputDir + _T("update.img");
	strTmpSystemFile = pDlg->m_strModulePath + _T("bin\\system.img");
	strBootImage = strAndroidDir + _T("\\Image\\boot.img");
	strRecoveryImage = strAndroidDir + _T("\\Image\\recovery.img");
	strKernelImage = strAndroidDir + _T("\\Image\\kernel.img");
	strRamdiskImage = strBootDir + _T("\\ramdisk.img");
	strKernelSrc = strAndroidDir + _T("\\kernel_src.img");
	strTmpRecovery = strRecoveryDir +_T("\\recovery.img");
	strResouceImage = strAndroidDir + _T("\\Image\\resource.img");
	
	iRet = ParsePackageFile(strAndroidDir+_T("\\package-file"),packetSet);
	if (iRet==0)
	{
		iPos = FindItem(packetSet,PARTNAME_SYSTEM);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strSystemImage);
			}
		}
		iPos = FindItem(packetSet,PARTNAME_BOOT);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strBootImage);
			}
		}
		iPos = FindItem(packetSet,PARTNAME_RECOVERY);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strRecoveryImage);
			}
		}
		iPos = FindItem(packetSet,PARTNAME_KERNEL);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strKernelImage);
			}
		}
	}
	else
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:PackProc-->ParsePackageFile failed,err=%d"),iRet);
		}
		iRet = 0;//back to initial value
	}

	if (!PathFileExists(strKernelImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_KERNEL")));
		iRet = -1;
		goto EXIT_PACK;
	}

	if (!PathFileExists(strResouceImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_RESOURCE")));
		iRet = -2;
		goto EXIT_PACK;
	}

	if (!PathFileExists(strBootImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_BOOT")));
		iRet = -3;
		goto EXIT_PACK;
	}
	if (!PathFileExists(strRecoveryImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_RECOVERY")));
		iRet = -4;
		goto EXIT_PACK;
	}

	pDlg->OutputInfo(GetLocalString(_T("INFO_PACKING_BOOT")));
	if (CheckExtBoot(strBootImage))
	{
		if (!PathFileExists(strBootDir+_T("\\Ramdisk")))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_RAMDISK_DIR")));
			iRet = -5;
			goto EXIT_PACK;
		}	
		if (PathFileExists(strRamdiskImage))
		{
			DeleteFile(strRamdiskImage);
		}	
		strCmd = CMySpawnConsumer::PackRamdiskCmd(g_strPackRamdiskBat);
		if (!RunOhterExe(strCmd))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_RAMDISK_DOING_WRONG")));
			iRet = -6;
			goto EXIT_PACK;
		}
		if (PathFileExists(strRamdiskImage))
		{
			if (!UnpackKernel(strKernelImage,strKernelSrc))
			{
				pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACK_KERNEL_FAIL")));
				iRet = -7;
				goto EXIT_PACK;
			}
			DeleteFile(strBootImage);
	
			strCmd = CMySpawnConsumer::mkBootImgCmd(g_strMkbootimg,strKernelSrc,strRamdiskImage,strResouceImage,strBootImage);
			
			if (!RunOhterExe(strCmd))
			{
				pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_BOOT_DOING_WRONG")));
				iRet = -8;
				goto EXIT_PACK;
			}
			DeleteFile(strKernelSrc);
			if (!PathFileExists(strBootImage))
			{
				pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_BOOT_FAIL")));
				iRet = -9;
				goto EXIT_PACK;
			}
			
		}
		else
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_RAMDISK_FAIL")));
			iRet = -10;
			goto EXIT_PACK;
		}
	}
	else
	{
		if (!PathFileExists(strBootDir+_T("\\Boot")))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_BOOT_DIR")));
			iRet = -11;
			goto EXIT_PACK;
		}
		if (PathFileExists(strBootDir+_T("\\boot.img")))
		{
			DeleteFile(strBootDir+_T("\\boot.img"));
		}
		strCmd = CMySpawnConsumer::PackBootImgCmd(g_strPackBootImgBat);
		if (!RunOhterExe(strCmd))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_BOOT_DOING_WRONG")));
			iRet = -12;
			goto EXIT_PACK;
		}
		if (PathFileExists(strBootDir+_T("\\boot.img")))
		{
			CopyFile(strBootDir+_T("\\boot.img"),strBootImage,FALSE);
		}
		else
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_BOOT_FAIL")));
			iRet = -13;
			goto EXIT_PACK;
		}

	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_PACK_BOOT_OK")));

	pDlg->OutputInfo(GetLocalString(_T("INFO_PACKING_RECOVERY")));
	if (!PathFileExists(strRecoveryDir+_T("\\Recovery")))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_RECOVERY_DIR")));
		iRet = -14;
		goto EXIT_PACK;
	}
	if (PathFileExists(strTmpRecovery))
	{
		DeleteFile(strTmpRecovery);
	}
	if (CheckExtBoot(strRecoveryImage))
	{
		strCmd = CMySpawnConsumer::PackKernelRecoveryCmd(g_strPackKernelRecoveryBat);
		if (!RunOhterExe(strCmd))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_RAMDISK_RECOVERY_DOING_WRONG")));
			iRet = -15;
			goto EXIT_PACK;
		}
		if (PathFileExists(strTmpRecovery))
		{
			if (!UnpackKernel(strKernelImage,strKernelSrc))
			{
				pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACK_KERNEL_FAIL")));
				iRet = -16;
				goto EXIT_PACK;
			}
			DeleteFile(strRecoveryImage);
			strCmd = CMySpawnConsumer::mkBootImgCmd(g_strMkbootimg,strKernelSrc,strTmpRecovery,strResouceImage,strRecoveryImage);
			
			if (!RunOhterExe(strCmd))
			{
				pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_RECOVERY_DOING_WRONG")));
				iRet = -17;
				goto EXIT_PACK;
			}
			DeleteFile(strKernelSrc);
			if (!PathFileExists(strRecoveryImage))
			{
				pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_RECOVERY_FAIL")));
				iRet = -18;
				goto EXIT_PACK;
			}
			
		}
		else
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_RAMDISK_RECOVERY_FAIL")));
			iRet = -19;
			goto EXIT_PACK;
		}
	}
	else
	{
		strCmd = CMySpawnConsumer::PackRecoveryCmd(g_strPackRecoveryBat);
		if (!RunOhterExe(strCmd))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_RECOVERY_DOING_WRONG")));
			iRet = -20;
			goto EXIT_PACK;
		}
		if (PathFileExists(strTmpRecovery))
		{
			CopyFile(strTmpRecovery,strRecoveryImage,FALSE);
		}
		else
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_RECOVERY_FAIL")));
			iRet = -21;
			goto EXIT_PACK;
		}
		
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_PACK_RECOVERY_OK")));

	if (!PathFileExists(strSystemDir))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_SYSTEM_DIR")));
		iRet = -22;
		goto EXIT_PACK;
	}
	
	pDlg->OutputInfo(GetLocalString(_T("INFO_PACKING_SYSTEM")));
	
	if (CheckExtImage(strSystemImage))
	{
		strCmd = CMySpawnConsumer::PackExtCmd(g_strPackExtBat);
		if (PathFileExists(strTmpSystemFile))
		{
			DeleteFile(strTmpSystemFile);
		}
		bRet = RunOhterExe(strCmd);
		
		if (!bRet)
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_SYSTEM_DOING_WRONG")));
			iRet = -23;
			goto EXIT_PACK;
		}
		
		if (!PathFileExists(strTmpSystemFile))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_SYSTEM_FAIL")));
			iRet = -24;
			goto EXIT_PACK;
		}
		
		if (!CopyFile(strTmpSystemFile,strSystemImage,FALSE))
		{
			if (g_pLogObject)
			{
				g_pLogObject->Record(_T("ERROR:PackProc-->copy new system back failed"));
			}
			iRet = -25;
			goto EXIT_PACK;
		}
		
		DeleteFile(strTmpSystemFile);
		
	}
	else
	{
		strCmd = CMySpawnConsumer::PackCramfs(g_strMKcramfs,strSystemDir,strSystemImage);
		if (!RunOhterExe(strCmd))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_SYSTEM_DOING_WRONG")));
			iRet = -26;
			goto EXIT_PACK;
		}
	}

	if (!PathFileExists(strSystemImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_SYSTEM_NOFOUND_SYSTEM_IMAGE")));
		iRet = -27;
		goto EXIT_PACK;
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_PACK_SYSTEM_OK")));

	if (!PathFileExists(strAndroidDir))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_ANDROID_DIR")));
		iRet = -28;
		goto EXIT_PACK;
	}

	pDlg->OutputInfo(GetLocalString(_T("INFO_PACKING_ANDROID")));
	if (bIsUnionFW)
	{
		strCmd = CMySpawnConsumer::PackAndroidCmd(g_strAfptool,strAndroidDir,strUnionFW);
		if (PathFileExists(strUnionFW))
		{
			DeleteFile(strUnionFW);
		}
	}
	else
	{
		strCmd = CMySpawnConsumer::PackAndroidCmd(g_strAfptool,strAndroidDir,strOutputFile);
		if (PathFileExists(strOutputFile))
		{
			DeleteFile(strOutputFile);
		}
	}

	if (!RunOhterExe(strCmd))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_ANDROID_DOING_WRONG")));
		iRet = -29;
		goto EXIT_PACK;
	}
	if (bIsUnionFW)
	{
		if (!PathFileExists(strUnionFW))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_ANDROID_NOFOUND_FIRMWARE_IMAGE")));
			iRet = -30;
			goto EXIT_PACK;
		}
	}
	else
	{
		if (!PathFileExists(strOutputFile))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_ANDROID_NOFOUND_UPDATE_IMAGE")));
			iRet = -31;
			goto EXIT_PACK;
		}
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_PACK_ANDROID_OK")));
	
	if (bIsUnionFW)
	{
		pDlg->OutputInfo(GetLocalString(_T("INFO_PACKING_UNION")));
		if (PathFileExists(strOutputFile))
		{
			DeleteFile(strOutputFile);
		}
		strCmd = CMySpawnConsumer::PackUnionCmd(g_strRKImageMaker,strDeviceType,strUnionBoot,strUnionFW,strOutputFile);
		if (!RunOhterExe(strCmd))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACKING_UNION_DOING_WRONG")));
			iRet = -32;
			goto EXIT_PACK;
		}
		if (!PathFileExists(strOutputFile))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_PACK_UNION_NOFOUND_UPDATE_IMAGE")));
			iRet = -33;
			goto EXIT_PACK;
		}
		pDlg->OutputInfo(GetLocalString(_T("INFO_PACK_UNION_OK")));
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_PACK_FIRMWARE_OK")));
EXIT_PACK:
	pDlg->m_pActionThread = NULL;
	pDlg->SwitchCtrl();
	pDlg->m_progress.End();
	if (iRet==0)
	{		
		pDlg->MessageBox(GetLocalString(_T("INFO_PACK_FIRMWARE_OK")),GetLocalString(_T("INFO_MESSAGEBOX_INFOTITLE")),MB_ICONINFORMATION|MB_OK);
		SendMessage(((HWND)pDlg->m_vecExtension[0].childHwnd),WM_PACK_FINISH,0,0);
	}
	else
		pDlg->MessageBox(GetLocalString(_T("INFO_PACK_FIRMWARE_FAIL")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
	return iRet;
}

bool tryUnpack(CString strCmd,CString strTmpRamdisk, CString strImg, CString strTmpImg){
		BOOL bSuccess;
		int i = 1;
		FILE *pFile=NULL;
		FILE *pDest=NULL;
		DWORD dwWriteOnce=0,size=0,dwBufferSize=0,dwRet;
		BYTE buffer[16*1024];
		dwBufferSize = 16*1024;
		//char dir[MAX_PATH]= {0};
		//char tmpdir[MAX_PATH] = {0};
		//CString csTmpDir = strRecoveryDir +_T("\\recovery_tmp.img");

		int len = WideCharToMultiByte(CP_ACP, 0, strImg, -1, NULL, 0, NULL, NULL);
		char* dir = new char[len + 1];
		memset(dir, 0, len + 1);
		WideCharToMultiByte (CP_ACP, 0,strImg, -1, dir, len, NULL,NULL);

		len = WideCharToMultiByte(CP_ACP, 0, strTmpImg, -1, NULL, 0, NULL, NULL);
		char* tmpdir = new char[len + 1];
		memset(tmpdir, 0, len + 1);
		WideCharToMultiByte (CP_ACP, 0,strTmpImg, -1, tmpdir, len, NULL,NULL);


		for( ; i <= 3; i++){
			bSuccess = RunOhterExe(strCmd);
			if(bSuccess)
			{
				break;
			}
			CopyFile(strTmpRamdisk,strImg,FALSE);
			pFile = fopen((const char *)dir,"rb");
            pDest = fopen((const char *)tmpdir,"wb+");
			fseek(pFile, SEEK_SET, SEEK_END);
			size = ftell(pFile)-i;  
			fseek(pFile,0,SEEK_SET);
			bool bflag = true;
			while(size>0)
			{
				dwWriteOnce = (size>=dwBufferSize) ? dwBufferSize:size ;
				memset(buffer,0, sizeof(buffer));
				dwRet = fread(buffer,1,dwWriteOnce,pFile);
				if (dwRet!=dwWriteOnce)
				{
					fclose(pDest);
					fclose(pFile);
					printf("Read ramdisk.img data failed.\r\n");
					bflag = false;
					break;
				}
				dwRet = fwrite(buffer,1,dwWriteOnce,pDest);
				if (dwRet!=dwWriteOnce)
				{
					fclose(pDest);
					fclose(pFile);
					printf("Write ramdisk.img data failed.\r\n");
					bflag = false;
					break;
				}
				size -= dwWriteOnce;
			}
			fclose(pDest);
			fclose(pFile);
			if(bflag){
				DeleteFile(strImg);
				CopyFile(strTmpImg,strImg,FALSE);
				DeleteFile(strTmpImg);
			}
			else{
				DeleteFile(strImg);
				DeleteFile(strTmpImg);
			}
		}
		delete[] tmpdir;
		delete[] dir;
		return bSuccess;
}
UINT UnpackProc( LPVOID pParam )
{
	CPortApkToolDlg *pDlg = (CPortApkToolDlg *)pParam;	
	int iRet=0,iPos,nSize;
	LPTSTR lpDst;
	VECTOR_PACKAGE_SET packetSet;
	BOOL bIsUnionFW,bMergeBoot,bMergeRecovery,bSuccess,bRet;
	CString strDeviceType,strUnionDir,strAndroidDir,strBootDir,strUnionBoot,strRamdiskImage,
		    strUnionFW,strSystemImage,strSystemDir,strCmd,strKernelImage,strResourceImage,strBootImage;
	CString strTmpKernel,strTmpResource,strTmpRamdisk,strRecoveryImage,strRecoveryDir,strTmpRecovery,strRelativePath;
	if (!pDlg->GetImageInfo(bIsUnionFW,strDeviceType))
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:UnpackProc-->GetImageInfo failed"));
		}
		iRet = -1;
		goto EXIT_UNPACK;
	}

	strUnionDir = pDlg->m_strModulePath + _T("Temp");
	strAndroidDir = strUnionDir + _T("\\Android");
	strSystemDir = strUnionDir + _T("\\System");
	strBootDir = strUnionDir + _T("\\Boot");
	strRecoveryDir = strUnionDir + _T("\\Recovery");
	strUnionBoot = strUnionDir + _T("\\boot.bin");
	strUnionFW = strUnionDir + _T("\\firmware.img");
	strSystemImage = strAndroidDir + _T("\\Image\\system.img");
	strKernelImage = strAndroidDir + _T("\\Image\\kernel.img");
	strResourceImage = strAndroidDir + _T("\\Image\\resource.img");
	strBootImage = strAndroidDir + _T("\\Image\\boot.img");
	strRecoveryImage = strAndroidDir + _T("\\Image\\recovery.img");
	strRamdiskImage = strBootDir + _T("\\Ramdisk.img");
	strTmpRecovery = strRecoveryDir +_T("\\recovery.img");
	strTmpKernel = pDlg->m_strModulePath + _T("bin\\kernel.img");
	strTmpRamdisk = pDlg->m_strModulePath + _T("bin\\ramdisk.img");
	strTmpResource= pDlg->m_strModulePath + _T("bin\\resource.img");
	if (PathFileExists(strUnionDir))
	{
		DeleteDir(strUnionDir);
	}
	CreateDirectory(strUnionDir,NULL);
	CreateDirectory(strAndroidDir,NULL);
	CreateDirectory(strBootDir,NULL);
	CreateDirectory(strRecoveryDir,NULL);
	CreateDirectory(strAndroidDir+_T("\\Image"),NULL);
	
	if (bIsUnionFW)
	{
		pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACKING_UNION")));
		strCmd = CMySpawnConsumer::UnpackUnionCmd(g_strRKImageMaker,pDlg->m_strPortImage,strUnionDir);
		if (!RunOhterExe(strCmd))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACKING_UNION_DOING_WRONG")));
			iRet = -2;
			goto EXIT_UNPACK;
		}
		if (!PathFileExists(strUnionFW))
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACK_UNION_NOFOUND_FIRMWARE_IMAGE")));
			iRet = -3;
			goto EXIT_UNPACK;
		}
		pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACK_UNION_OK")));
		strCmd = CMySpawnConsumer::UnpackAndroidCmd(g_strAfptool,strUnionFW,strAndroidDir);
	}
	else
	{
		strCmd = CMySpawnConsumer::UnpackAndroidCmd(g_strAfptool,pDlg->m_strPortImage,strAndroidDir);
	}
	
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACKING_ANDROID")));
	if (!RunOhterExe(strCmd))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACKING_ANDROID_DOING_WRONG")));
		iRet = -4;
		goto EXIT_UNPACK;
	}
	iRet = ParsePackageFile(strAndroidDir+_T("\\package-file"),packetSet);
	if (iRet==0)
	{
		iPos = FindItem(packetSet,PARTNAME_SYSTEM);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strSystemImage);
			}
		}
		TRACE(_T("%s\r\n"),strSystemImage);
		iPos = FindItem(packetSet,PARTNAME_BOOT);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strBootImage);
			}
		}
		TRACE(_T("%s\r\n"),strBootImage);
		iPos = FindItem(packetSet,PARTNAME_RECOVERY);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strRecoveryImage);
			}
		}
		TRACE(_T("%s\r\n"),strRecoveryImage);
		iPos = FindItem(packetSet,PARTNAME_KERNEL);
		if (iPos!=-1)
		{
			bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,packetSet[iPos].file);
			if (bRet)
			{
				strRelativePath = strAndroidDir + _T("\\") + lpDst;
				delete []lpDst;
				GetAbsolutePath(strRelativePath,strKernelImage);
			}
		}
		TRACE(_T("%s\r\n"),strKernelImage);
	}
	else
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:PackProc-->ParsePackageFile failed,err=%d"),iRet);
		}
		iRet = 0;//back to initial value
	}
	if (!PathFileExists(strBootImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACK_ANDROID_NOFOUND_BOOT_IMAGE")));
		iRet = -5;
		goto EXIT_UNPACK;
	}
	if (!PathFileExists(strRecoveryImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACK_ANDROID_NOFOUND_RECOVERY_IMAGE")));
		iRet = -6;
		goto EXIT_UNPACK;
	}

	if (!PathFileExists(strSystemImage))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACK_ANDROID_NOFOUND_SYSTEM_IMAGE")));
		iRet = -7;
		goto EXIT_UNPACK;
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACK_ANDROID_OK")));

	if (CheckExtBoot(strBootImage))
	{
		bMergeBoot = TRUE;
	}
	else
		bMergeBoot = FALSE;

	if (CheckExtBoot(strRecoveryImage))
	{
		bMergeRecovery = TRUE;
	}
	else
		bMergeRecovery = FALSE;

	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACKING_KERNEL")));
	if (bMergeBoot)//(!PathFileExists(strKernelImage))
	{
		bSuccess = FALSE;
		if (PathFileExists(strTmpKernel))
		{
			DeleteFile(strTmpKernel);
		}
		if (bMergeBoot)
		{
			strCmd = CMySpawnConsumer::UnpackBootCmd(g_strUnpackBootExe,_T("-k"),strBootImage);
			if (RunOhterExe(strCmd))
			{
				if (PathFileExists(strTmpKernel))
				{
					strCmd = CMySpawnConsumer::mkkrnlimgCmd(g_strMKkrnlimg,strTmpKernel,strKernelImage);
					if (RunOhterExe(strCmd))
					{
						if (PathFileExists(strKernelImage))
						{
							DeleteFile(strTmpKernel);
							bSuccess = TRUE;
						}
					}
					else
						g_pLogObject->Record(_T("Error:mkkrnlimgCmd run failed"));
				}
				else
					g_pLogObject->Record(_T("Error:UnpackBootCmd failed : no found kernel.img"));
			}
			else
				g_pLogObject->Record(_T("Error:UnpackBootCmd run failed"));
		}
		if (!bSuccess)
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_KERNEL")));
			iRet = -8;
			goto EXIT_UNPACK;
		}
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACK_KERNEL_OK")));


	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACKING_RESOURCE")));
	if (!PathFileExists(strResourceImage))
	{
		bSuccess = FALSE;
		if (PathFileExists(strTmpResource))
		{
			DeleteFile(strTmpResource);
		}
		strCmd = CMySpawnConsumer::UnpackBootCmd(g_strUnpackBootExe,_T("-s"),strBootImage);
		if (RunOhterExe(strCmd))
		{
			if (PathFileExists(strTmpResource))
			{
				CopyFile(strTmpResource,strResourceImage,FALSE);
                DeleteFile(strTmpResource);
				bSuccess = TRUE;
			}
			else
				g_pLogObject->Record(_T("Error:UnpackBootCmd failed : no found resource.img"));
		}
		else
			g_pLogObject->Record(_T("Error:UnpackBootCmd run failed"));

		if (!bSuccess)
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_RESOURCE")));
			iRet = -8;
			goto EXIT_UNPACK;
		}
	}	
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACK_RESOURCE_OK")));

	
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACKING_BOOT")));
	if (bMergeBoot)
	{
		bSuccess = FALSE;
		if (PathFileExists(strTmpRamdisk))
		{
			DeleteFile(strTmpRamdisk);
		}
		strCmd = CMySpawnConsumer::UnpackBootCmd(g_strUnpackBootExe,_T("-r"),strBootImage);
		if (RunOhterExe(strCmd))
		{
			if (PathFileExists(strTmpRamdisk))
			{
				CopyFile(strTmpRamdisk,strRamdiskImage,FALSE);
                //DeleteFile(strTmpRamdisk);
				bSuccess = TRUE;
			}
			else
				g_pLogObject->Record(_T("Error:UnpackBootCmd failed : no found ramdisk.img"));
		}
		else
			g_pLogObject->Record(_T("Error:UnpackBootCmd run failed"));
		
		if (!bSuccess)
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_RAMDISK")));
			iRet = -10;
			goto EXIT_UNPACK;
		}
		strCmd = CMySpawnConsumer::UnpackRamdiskCmd(g_strUnpackRamdiskBat);
		//bSuccess = RunOhterExe(strCmd);
		bSuccess = tryUnpack(strCmd, strTmpRamdisk,strRamdiskImage, strBootDir + _T("\\Ramdisk_tmp.img"));
		DeleteFile(strTmpRamdisk);

	}
	else
	{
		CopyFile(strBootImage,strBootDir+_T("\\Boot.img"),FALSE);
		strCmd = CMySpawnConsumer::UnpackBootImgCmd(g_strUnpackBootImgBat);
		//bSuccess = RunOhterExe(strCmd);
		bSuccess = tryUnpack(strCmd, strBootImage,strBootDir+_T("\\Boot.img"), strBootDir + _T("\\Boot_tmp.img"));
	}
	if (!bSuccess)
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACKING_BOOT_DOING_WRONG")));
		iRet = -11;
		goto EXIT_UNPACK;
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACK_BOOT_OK")));

	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACKING_RECOVERY")));
	if (bMergeRecovery)
	{
		bSuccess = FALSE;
		if (PathFileExists(strTmpRamdisk))
		{
			DeleteFile(strTmpRamdisk);
		}
		strCmd = CMySpawnConsumer::UnpackBootCmd(g_strUnpackBootExe,_T("-r"),strRecoveryImage);
		if (RunOhterExe(strCmd))
		{
			if (PathFileExists(strTmpRamdisk))
			{
				CopyFile(strTmpRamdisk,strTmpRecovery,FALSE);
                //DeleteFile(strTmpRamdisk);
				bSuccess = TRUE;
			}
			else
				g_pLogObject->Record(_T("Error:UnpackBootCmd failed : no found ramdisk.img in the recovery.img"));
		}
		else
			g_pLogObject->Record(_T("Error:UnpackBootCmd run failed"));
		
		if (!bSuccess)
		{
			pDlg->OutputInfo(GetLocalString(_T("ERROR_NOFOUND_RAMDISK")));
			iRet = -12;
			goto EXIT_UNPACK;
		}
		strCmd = CMySpawnConsumer::UnpackKernelRecoveryCmd(g_strUnpackKernelRecoveryBat);
		bSuccess = tryUnpack(strCmd,strTmpRamdisk,strTmpRecovery, strRecoveryDir +_T("\\recovery_tmp.img"));
		DeleteFile(strTmpRamdisk);
	}
	else
	{
		CopyFile(strRecoveryImage,strTmpRecovery,FALSE);
		strCmd = CMySpawnConsumer::UnpackRecoveryCmd(g_strUnpackRecoveryBat);
		bSuccess = tryUnpack(strCmd,strRecoveryImage,strTmpRecovery, strRecoveryDir +_T("\\recovery_tmp.img"));//RunOhterExe(strCmd);
	}
	if (!bSuccess)
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACKING_RECOVERY_DOING_WRONG")));
		iRet = -13;
		goto EXIT_UNPACK;
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACK_RECOVERY_OK")));


	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACKING_SYSTEM")));
	if (CheckExtImage(strSystemImage))
	{
		CreateDirectory(strSystemDir,NULL);
		strCmd = CMySpawnConsumer::UnpackExtCmd(g_strUnpackExtExe,strSystemImage,strSystemDir);
	}
	else
		strCmd = CMySpawnConsumer::UnpackCramfsCmd(g_strCramfsCK,strSystemImage,strSystemDir);
	if (!RunOhterExe(strCmd))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACKING_SYSTEM_DOING_WRONG")));
		iRet = -14;
		goto EXIT_UNPACK;
	}
	if (!PathFileExists(strSystemDir+_T("\\app")))
	{
		pDlg->OutputInfo(GetLocalString(_T("ERROR_UNPACK_SYSTEM_FAIL")));
		iRet = -15;
		goto EXIT_UNPACK;
	}
	pDlg->OutputInfo(GetLocalString(_T("INFO_UNPACK_SYSTEM_OK")));

EXIT_UNPACK:
	pDlg->m_pActionThread = NULL;
	pDlg->SwitchCtrl();
	pDlg->m_progress.End();
	if (iRet==0)
	{
		pDlg->MessageBox(GetLocalString(_T("INFO_UNPACK_FIRMWARE_OK")),GetLocalString(_T("INFO_MESSAGEBOX_INFOTITLE")),MB_ICONINFORMATION|MB_OK);
		SendMessage(((HWND)pDlg->m_vecExtension[0].childHwnd),WM_UNPACK_FINISH,0,0);
	}
	else
		pDlg->MessageBox(GetLocalString(_T("INFO_UNPACK_FIRMWARE_FAIL")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
	return iRet;
}

BOOL RunOhterExe(CString strCmd,ENUM_STRING_CODE emCode)
{
	CMySpawnConsumer spawnConsumer;
	cmSpawn spawn;
	CString strText;
	tstring strInfo;
	strText = _T("***")+strCmd+_T("***");
	g_pLogObject->Record(strText);
	BOOL bBat=FALSE;
	strCmd.MakeLower();
	if (strCmd.Find(_T(".bat"))!=-1)
	{
		bBat = TRUE;
	}
	
	try
	{
		SetCurrentDirectory(g_strBinDir);
		spawn.Execute(strCmd);
	}
	catch (LPCTSTR pszError)
	{
		SetCurrentDirectory(g_strModulePath);
		
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:RunOhterExe-->Execute failed,Cmd(%s),Error(%s)"),strCmd,pszError);
		}
		return FALSE;
	}
	catch (...)
	{
		SetCurrentDirectory(g_strModulePath);
		
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:RunOhterExe-->Execute failed,Cmd(%s)"),strCmd);
		}
		return FALSE;
	}
	//4.等待执行线程结束
	while(TRUE)
	{
		if (!spawn.TestThread())
		{
			break;
		}
		Sleep(200);
	}
	SetCurrentDirectory(g_strModulePath);

	spawnConsumer.SetSpawnObject(&spawn);
	spawnConsumer.GetSpawnString(strInfo,emCode);
	g_pLogObject->Write(strInfo.c_str());

	if (bBat)
	{//获取返回结果
		if (!spawnConsumer.GetBatCmdResult(strInfo))
		{
			if (g_pLogObject)
			{
				g_pLogObject->Record(_T("ERROR:RunOhterExe-->GetBatCmdResult failed"));
			}
			return FALSE;
		}
	}
	
	return TRUE;

}
BOOL CPortApkToolDlg::GetImageInfo(BOOL &bIsUnionFw,CString &strDeviceType)
{
	CFile file;
	if (!file.Open(m_strPortImage,CFile::modeRead|CFile::typeBinary))
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:GetImageInfo-->open (%s) failed"),m_strPortImage);
		}
		return FALSE;
	}
	UINT uiTag,uiRead;
	uiRead = file.Read(&uiTag,4);
	if (uiRead!=4)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:GetImageInfo-->read (%s) failed"),m_strPortImage);
		}
		file.Close();
		return FALSE;
	}
	if (uiTag==0x57464B52)
	{//统一固件
		bIsUnionFw = TRUE;
		STRUCT_RKIMAGE_HEAD imageHead;
		file.SeekToBegin();
		uiRead = file.Read(&imageHead,sizeof(STRUCT_RKIMAGE_HEAD));
		if (uiRead!=sizeof(STRUCT_RKIMAGE_HEAD))
		{
			if (g_pLogObject)
			{
				g_pLogObject->Record(_T("ERROR:GetImageInfo-->read union head failed,file(%s)"),m_strPortImage);
			}
			file.Close();
			return FALSE;
		}
		switch (imageHead.emSupportChip)
		{
		case RK28_DEVICE:
			strDeviceType = _T("-RK28");
			break;
		case RK281X_DEVICE:
			strDeviceType = _T("-RK281X");
			break;
		case RK29_DEVICE:
			strDeviceType = _T("-RK29");
			break;
		case RK292X_DEVICE:
			strDeviceType = _T("-RK292X");
			break;
		case RK30_DEVICE:
			strDeviceType = _T("-RK30");
			break;
		case RK31_DEVICE:
			strDeviceType = _T("-RK31");
			break;
		case RK32_DEVICE:
			strDeviceType = _T("-RK32");
			break;
		case RK33_DEVICE:
			strDeviceType = _T("-RK33");
			break;
        case RKNONE_DEVICE:
			if (g_pLogObject)
			{
				g_pLogObject->Record(_T("ERROR:GetImageInfo-->not support device type(%d)"),imageHead.emSupportChip);
			}
			file.Close();
			return FALSE;
        default:
            strDeviceType = _T("-RK") + GetChipValueFromDword(imageHead.emSupportChip);
            break;
		}
	}
	else if (uiTag==0x46414B52)
	{//Android固件
		bIsUnionFw = FALSE;
	}
	else
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("ERROR:GetImageInfo-->invalid image"));
		}
		file.Close();
		return FALSE;
	}
	file.Close();
	return TRUE;
}

VOID CPortApkToolDlg::OutputInfo(CString strInfo,BOOL bShow)
{
	if (bShow)
	{
		m_labelInfo.SetWindowText(strInfo);
	}
	if (g_pLogObject)
	{
		g_pLogObject->Record(_T("INFO:%s"),strInfo);
	}
}

void CPortApkToolDlg::OnButtonUnpack() 
{
	// TODO: Add your control notification handler code here

	TCHAR strtemp[300]={0};
	CString strTxt=CString(strtemp,300);
	CString strConfigPath;
	BOOL isDebug=FALSE;
	strConfigPath = m_strModulePath + _T("config.ini");

	if (m_pActionThread)
	{
		MessageBox(GetLocalString(_T("MESSAGE_RUNNING_NOW")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}

	GetPrivateProfileStringW(_T("Version"),_T("android"),_T("0"),strTxt.GetBuffer(300),300,strConfigPath);
	if((strTxt.CompareNoCase(_T("4.4"))==0)||(strTxt.CompareNoCase(_T("5.0"))==0)||(strTxt.CompareNoCase(_T("6.0"))==0))
	{
		g_strUnpackExtExe = m_strModulePath + _T("bin\\ext2fsckff.exe");
		g_strPackExtBat = m_strModulePath + _T("bin\\PackExtImageff.bat");	
	}
	else
	{
		g_strUnpackExtExe = m_strModulePath + _T("bin\\ext2fsckft.exe");
		g_strPackExtBat = m_strModulePath + _T("bin\\PackExtImageft.bat");		
	}
	strTxt.ReleaseBuffer(300);
	
	if (!PathFileExists(g_strRKImageMaker))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_RKIAMGEMAKER")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (!PathFileExists(g_strAfptool))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_AFPTOOL")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (!PathFileExists(g_strMKcramfs))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_MKCRAMFS")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (!PathFileExists(g_strCramfsCK))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_CRAMFSCK")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	GetDlgItemText(IDC_EDIT_IMAGE_PATH,m_strPortImage);

	if (m_strPortImage.IsEmpty())
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_FIRMWARE")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}

	SwitchCtrl(FALSE);
	m_progress.Start();
	m_pActionThread = AfxBeginThread(UnpackProc,(LPVOID)this);
	m_labelInfo.SetWindowText(GetLocalString(_T("INFO_START_UNPACK")));
}

void CPortApkToolDlg::OnButtonPack() 
{
	// TODO: Add your control notification handler code here
	TCHAR strtemp[300]={0};
	CString strTxt=CString(strtemp,300);
	CString strConfigPath;
	BOOL isDebug=FALSE;
       


	strConfigPath = m_strModulePath + _T("config.ini");
	if (m_pActionThread)
	{
		MessageBox(GetLocalString(_T("MESSAGE_RUNNING_NOW")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (!PathFileExists(g_strRKImageMaker))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_RKIAMGEMAKER")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (!PathFileExists(g_strAfptool))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_AFPTOOL")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (!PathFileExists(g_strMKcramfs))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_MKCRAMFS")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	if (!PathFileExists(g_strCramfsCK))
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_CRAMFSCK")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	GetDlgItemText(IDC_EDIT_IMAGE_PATH,m_strPortImage);

	GetPrivateProfileStringW(_T("Debug"),_T("Debug"),_T("0"),strTxt.GetBuffer(300),300,strConfigPath);
	if(strTxt.CompareNoCase(_T("1"))==0)
	{
		isDebug=TRUE;
	}
	strTxt.ReleaseBuffer(300);
	
	if(isDebug)
	{
		GetPrivateProfileStringW(_T("Debug"),_T("DebugPath"),NULL,strTxt.GetBuffer(300),300,strConfigPath);
		if(!strTxt.IsEmpty())
			m_strPortImage=strTxt;
		strTxt.ReleaseBuffer(300);
	}

	if (m_strPortImage.IsEmpty())
	{
		MessageBox(GetLocalString(_T("MESSAGE_NOFOUND_FIRMWARE")),GetLocalString(_T("INFO_MESSAGEBOX_ERRORTITLE")),MB_ICONERROR|MB_OK);
		return;
	}
	SwitchCtrl(FALSE);
	m_progress.Start();
	m_pActionThread = AfxBeginThread(PackProc,(LPVOID)this);
	m_labelInfo.SetWindowText(GetLocalString(_T("INFO_START_PACK")));
}
BOOL WideCharToChar(CString strWideChar,char *pChar,int nCharBufferLen)
{
	int nRequiredSize;
	nRequiredSize = WideCharToMultiByte(CP_ACP,0,strWideChar,-1,NULL,0,NULL,NULL);
	
	if (nRequiredSize==0)
	{
		return FALSE;
	}
	
	if (!pChar)
	{
		return FALSE;
	}
	if (nCharBufferLen<nRequiredSize)
	{
		return FALSE;
	}
	memset(pChar,0,nCharBufferLen);
	int nTransferSize;
	nTransferSize = WideCharToMultiByte(CP_ACP,0,strWideChar,-1,pChar,nRequiredSize,NULL,NULL);
	
	if (nTransferSize!=nRequiredSize)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CPortApkToolDlg::CheckKernel(CString strKernel,CString strKey)
{
	char plain_key[16];
	unsigned char kernel_key[16];
	CString strLog;
	const unsigned char *encrypt_key;
	const unsigned char aes_key[16] = {
			0x4b, 0x65, 0x79, 0x20,
			0x61, 0x64, 0x64, 0x20,
			0x62, 0x20, 0x43, 0x68,
			0x65, 0x6e, 0x4a, 0x51
	};
	char search_str[] = "RK logo password";
	char *p=search_str;
	memset(plain_key,0,16);
	memset(kernel_key,0,16);
	if (!WideCharToChar(strKey,plain_key,16))
	{
		OutputInfo(_T("CheckKernel-->WideCharToChar key failed"),FALSE);
		return FALSE;
	}

	AES aesObject;
	encrypt_key = aesObject.cipher((const unsigned char *)plain_key,aes_key,16);
	
	FILE *pFile=NULL;
	pFile = _tfopen((LPTSTR)(LPCTSTR)strKernel,_T("rb"));
	if (!pFile)
	{
		strLog.Format(_T("CheckKernel-->Open kernel.img failed,err=%d"),GetLastError());
		OutputInfo(strLog,FALSE);
		return FALSE;
	}
	//匹配搜索串
	BOOL bFound=FALSE;
	int c = fgetc(pFile);
	
	while(c != EOF)
	{
		if (c == *p)
		{
			while(c == *p && *p != '\0')
			{
				c = fgetc(pFile);
				p++;
			}
			if (*p == '\0')
			{
				bFound = TRUE;
			}
			p = search_str;
		}
		
		if (bFound)
		{
			break;
		}
		c = fgetc(pFile);
	}
	if (!bFound)
	{
		OutputInfo(_T("CheckKernel-->No found search string in kernel"),FALSE);
		return FALSE;
	}
	fseek(pFile,-1,SEEK_CUR);
	UINT uiRead;
	uiRead = fread(kernel_key,1,16,pFile);
	if (uiRead!=16)
	{
		OutputInfo(_T("CheckKernel-->read key in kernel failed"),FALSE);
		return FALSE;
	}
	fclose(pFile);
	int i;
	for (i=0;i<16;i++)
	{
		if (kernel_key[i]!=encrypt_key[i])
		{
			return FALSE;
		}
	}
	
	return TRUE;
}


BOOL CheckExtBoot(CString strBootPath)
{
	BOOL bRet;
	CFile file;
	char sztag[] = "ANDROID!";
	char buf[9];
	bRet = file.Open(strBootPath,CFile::typeBinary|CFile::modeRead);
	if (!bRet)
	{
		g_pLogObject->Record(_T("Error:CheckExtBoot-->open file failed,err=%d"),GetLastError());
		return FALSE;
	}
	file.Read(buf,8);
	file.Close();
	buf[8] = 0;
	if (strcmp(buf,sztag)!=0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CheckExtImage(CString strExtImage)
{
	BOOL bRet;
	CFile file;
	USHORT usTag,usMagic;
	usTag = 0xEF53;
	bRet = file.Open(strExtImage,CFile::typeBinary|CFile::modeRead);
	if (!bRet)
	{
		g_pLogObject->Record(_T("Error:CheckExtImage-->open file failed,err=%d"),GetLastError());
		return FALSE;
	}
	file.Seek(1080,CFile::begin);
	file.Read(&usMagic,2);
	file.Close();
	if (usTag!=usMagic)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL UnpackKernel(CString strKernelFile,CString strOutputFile)
{
	BOOL bRet;
	CFile file;
	DWORD dwRead,dwFileSize,dwTag;
	BYTE *pBuf=NULL;
	bRet = file.Open(strKernelFile,CFile::typeBinary|CFile::modeRead);
	if (!bRet)
	{
		g_pLogObject->Record(_T("Error:UnpackKernel-->open file failed,err=%d"),GetLastError());
		return FALSE;
	}
	dwRead = file.Read(&dwTag,4);
	if (dwRead!=4)
	{
		file.Close();
		g_pLogObject->Record(_T("Error:UnpackKernel-->read tag failed"));
		return FALSE;
	}
	if (dwTag!=0x4C4E524B)
	{//不用解包
		file.Close();
		CopyFile(strKernelFile,strOutputFile,FALSE);
		return TRUE;
	}
	dwFileSize = file.GetLength();
	

	dwFileSize = dwFileSize -12;
	pBuf = new BYTE[dwFileSize];
	if (!pBuf)
	{
		
		file.Close();
		g_pLogObject->Record(_T("Error:UnpackKernel-->new memory failed"));
		return FALSE;
	}
	file.Seek(8,CFile::begin);
	dwRead = file.ReadHuge(pBuf,dwFileSize);
	file.Close();

	if (dwRead!=dwFileSize)
	{
		delete []pBuf;
		g_pLogObject->Record(_T("Error:UnpackKernel-->read file failed"));
		return FALSE;
	}
	bRet = file.Open(strOutputFile,CFile::typeBinary|CFile::modeCreate|CFile::modeReadWrite);
	if (!bRet)
	{
		delete []pBuf;
		g_pLogObject->Record(_T("Error:UnpackKernel-->open file for write failed,err=%d"),GetLastError());
		return FALSE;
	}
	file.WriteHuge(pBuf,dwFileSize);
	file.Close();
	delete []pBuf;
	return TRUE;
}

BOOL CPortApkToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE )
	{
		return TRUE;
	}
	if ( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN )
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
BOOL CPortApkToolDlg::LoadConfig()
{
	CString strConfigPath;
	strConfigPath = m_strModulePath + _T("config.ini");
	if (!PathFileExists(strConfigPath))
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadConfig-->PathFileExists failed"));
		}
		return FALSE;
	}
	bool bRet;
	bRet = m_iniConfig.Load((LPTSTR)(LPCTSTR)strConfigPath);
	if (!bRet)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadConfig-->Load file failed"));
		}
		return FALSE;
	}
	cmIniSection *pSection;
	TCHAR tzLangSection[]=_T("Language");
	pSection = m_iniConfig.GetSection(tzLangSection);
	if (!pSection)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadConfig-->GetSection Language failed"));
		}
		return FALSE;
	}
	CString strValue;
	strValue.Format(_T("%s"), pSection->GetKeyValue(_T("Kinds")).c_str() );
	int langKinds;
	langKinds = _ttoi((LPTSTR)(LPCTSTR)strValue);
	if (langKinds<=0)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadConfig-->GetKeyValue kinds failed"));
		}
		return FALSE;
	}

	strValue.Format(_T("%s"),pSection->GetKeyValue(_T("Selected")).c_str() );
	int curLang;
	curLang = _ttoi((LPTSTR)(LPCTSTR)strValue);
	if (curLang<=0)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadConfig-->GetKeyValue Selected failed"));
		}
		return FALSE;
	}
	m_curLang = curLang-1;
	
	strValue.Format(_T("%s"), pSection->GetKeyValue(_T("LangPath")).c_str() );
	m_LangPath = m_strModulePath + strValue;
	if (m_LangPath.Right(1)!=_T("\\"))
	{
		m_LangPath += _T("\\");
	}
	if (!PathFileExists(m_LangPath))
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadConfig-->PathFileExists %s failed"),m_LangPath);
		}
		return FALSE;
	}
	int i;
	CString strKey;
	STRUCT_LANG_PROPERTY langProp;
	for (i=0;i<langKinds;i++)
	{
		strKey.Format(_T("Lang%dFile"),i+1);
		strValue.Format(_T("%s"), pSection->GetKeyValue((LPTSTR)(LPCTSTR)strKey).c_str() );
		if (!PathFileExists(m_LangPath+strValue))
		{
			if (g_pLogObject)
			{
				g_pLogObject->Record(_T("LoadConfig-->PathFileExists %s failed"),strValue);
			}
			return FALSE;
		}
		langProp.strFileName = strValue;
	
		strKey.Format(_T("Lang%dFontName"),i+1);
		strValue.Format(_T("%s"), pSection->GetKeyValue((LPTSTR)(LPCTSTR)strKey).c_str() );
		langProp.strFontName = strValue;

		strKey.Format(_T("Lang%dFontSize"),i+1);
		strValue.Format(_T("%s"), pSection->GetKeyValue((LPTSTR)(LPCTSTR)strKey).c_str() );
		langProp.uiFontSize = _ttoi((LPTSTR)(LPCTSTR)strValue);
		if (langProp.uiFontSize<=0)
		{
			langProp.uiFontSize = 9;
		}
		m_langProp.push_back(langProp);
	}
	TCHAR tzSystemSection[]=_T("Extension");
	pSection = m_iniConfig.GetSection(tzSystemSection);
	if (!pSection)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadConfig-->GetSection Extension failed"));
		}
		return FALSE;
	}

	strValue.Format(_T("%s"),pSection->GetKeyValue(_T("Dll_Count")).c_str() );
	int dllCount;
	dllCount = _ttoi((LPTSTR)(LPCTSTR)strValue);
	if (curLang<=0)
	{
		dllCount = 0;
	}
	STRUCT_EXTENSION extension;
	for (i=1;i<=dllCount;i++)
	{
		strKey.Format(_T("Dll_%d_Name"),i);
		extension.strDllName.Format(_T("%s"), pSection->GetKeyValue((LPTSTR)(LPCTSTR)strKey).c_str() );
		if (LoadExtension(extension))
		{
			m_vecExtension.push_back(extension);
		}
	}
	m_strVersion.Empty();
	m_strVersion.Format(_T("%s"),pSection->GetKeyValue(_T("APP_VERSION")).c_str() );

	return TRUE;
}

BOOL CPortApkToolDlg::LoadExtension(STRUCT_EXTENSION &extension)
{
	CString strDllPath;
	extension.strDllName.MakeLower();
	if (extension.strDllName.Find(_T(".dll"))==-1)
	{
		extension.strDllName += _T(".dll");
	}
	strDllPath = m_strPluginPath  + extension.strDllName;
	if (!PathFileExists(strDllPath))
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadExtension-->%s file is not exist"),extension.strDllName);
		}
		return FALSE;
	}
	extension.hDll = LoadLibrary(strDllPath);
	if (extension.hDll==NULL)
	{
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadExtension-->LoadLibrary %s file failed"),extension.strDllName);
		}
		return FALSE;
	}

	extension.pSetMsgHandle = (SetMsgHandle)GetProcAddress(extension.hDll,"SetMsgHandle");
	if (extension.pSetMsgHandle==NULL)
	{
		FreeLibrary(extension.hDll);
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadExtension-->Get SetMsgHandle Address failed,Dll=%s"),extension.strDllName);
		}
		return FALSE;
	}
	extension.pSetWindowLanguage = (SetWindowLanguage)GetProcAddress(extension.hDll,"SetWindowLanguage");
	if (extension.pSetWindowLanguage==NULL)
	{
		FreeLibrary(extension.hDll);
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadExtension-->Get SetWindowLanguage Address failed,Dll=%s"),extension.strDllName);
		}
		return FALSE;
	}
	extension.pGetTabTitle = (GetTabTitle)GetProcAddress(extension.hDll,"GetTabTitle");
	if (extension.pGetTabTitle==NULL)
	{
		FreeLibrary(extension.hDll);
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadExtension-->Get GetTabTitle Address failed,Dll=%s"),extension.strDllName);
		}
		return FALSE;
	}
	extension.pCreateChildWindow = (CreateChildWindow)GetProcAddress(extension.hDll,"CreateChildWindow");
	if (extension.pCreateChildWindow==NULL)
	{
		FreeLibrary(extension.hDll);
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadExtension-->Get CreateChildWindow Address failed,Dll=%s"),extension.strDllName);
		}
		return FALSE;
	}
	extension.pDestroyChildWindow = (DestroyChildWindow)GetProcAddress(extension.hDll,"DestroyChildWindow");
	if (extension.pDestroyChildWindow==NULL)
	{
		FreeLibrary(extension.hDll);
		if (g_pLogObject)
		{
			g_pLogObject->Record(_T("LoadExtension-->Get DestroyChildWindow Address failed,Dll=%s"),extension.strDllName);
		}
		return FALSE;
	}
	return TRUE;
}
LRESULT CPortApkToolDlg::OnTestRunMsg(WPARAM wParam,LPARAM lParam)
{
	bool *pIsRun = (bool *)lParam;
	if (m_pActionThread)
	{
		*pIsRun = true;
	}
	else
		*pIsRun = false;
	return 0;
}
LRESULT CPortApkToolDlg::OnStartPackMsg(WPARAM wParam,LPARAM lParam)
{	
	if (lParam) {
		TCHAR *pPackParam = new TCHAR[100]; 
		memcpy(pPackParam,(TCHAR*)lParam,100*2);
 		m_strUpdateFile=pPackParam;
		delete pPackParam;
	} else {
         m_strUpdateFile = m_outputDir + _T("update.img");
    }
    OnButtonPack();	
	return 0;
}

LRESULT CPortApkToolDlg::OnGetDirMsg(WPARAM wParam,LPARAM lParam)
{
	ENUM_MSG_KEY emMsgKey = (ENUM_MSG_KEY)wParam;
	char *pszDst = (char *)lParam;
	CString strDir;
	strDir = m_strModulePath + _T("Temp");
	
	switch (emMsgKey)
	{
	case EmMsg_Boot_Key:
		strDir = strDir + _T("\\Boot");
		break;
	case EmMsg_Recovery_Key:
		strDir = strDir + _T("\\Recovery");
		break;
	case EmMsg_System_key:
		strDir = strDir + _T("\\System");
		break;
	case EmMsg_Root_key:
		strDir = strDir + _T("\\Android");
		break;
	default:
		return 0;
	}
	char *pszBuf;
	int bufSize;
	BOOL bRet;
	bRet = cmStrCode::UnicodeToAnsi(pszBuf,bufSize,(LPTSTR)(LPCTSTR)strDir);
	if (bRet)
	{
		strcpy(pszDst,pszBuf);
		delete []pszBuf;
		pszBuf = NULL;
	}
	return 0;
}
VOID CPortApkToolDlg::SetTabPanel(UINT uiTabIndex)
{
	NMHDR nmhdr;
	HRESULT hr;
	nmhdr.code=TCN_SELCHANGE;
	nmhdr.hwndFrom=m_tabExtensions.GetSafeHwnd();
	nmhdr.idFrom=m_tabExtensions.GetDlgCtrlID();
	m_tabExtensions.SetCurSel(uiTabIndex);
	hr = ::SendMessage(this->GetSafeHwnd(),WM_NOTIFY,MAKELONG(TCN_SELCHANGE,0),(LPARAM)(&nmhdr));
}

void CPortApkToolDlg::OnSelchangeTABExtension(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int index;
	index = m_tabExtensions.GetCurSel();
	if (index!=-1)
	{
		int i;
		for (i=0;i<m_vecExtension.size();i++)
		{
			CWnd *pChildWnd = CWnd::FromHandle((HWND)m_vecExtension[i].childHwnd);
			if (i==index)
			{
				pChildWnd->ShowWindow(SW_SHOW);
			}
			else
				pChildWnd->ShowWindow(SW_HIDE);
		}
	}
	*pResult = 0;
}
CString GetLocalString(CString strKey)
{
	BOOL bRet;
	CString strValue ;
	bRet = g_pMainDlg->m_pLangObject->GetString(_T("Prompt"),strKey,strValue);
	if (!bRet)
	{
		strValue = _T("");
	}
	return strValue;
}
int ParsePackageFile(CString strFilePath,VECTOR_PACKAGE_SET &packageSet)
{
	PBYTE pFileBuffer=NULL;
	STRUCT_PACKAGE_ITEM packageItem;
	CFileStatus fs;
	CFile file;
	BOOL bRet;
	UINT uiRead,uiFileSize;
	bRet = CFile::GetStatus(strFilePath,fs);
	if (!bRet)
	{
		return -1;
	}
	uiFileSize = fs.m_size;
	pFileBuffer = new BYTE[uiFileSize+1];
	if (!pFileBuffer)
	{
		return -2;
	}
	memset(pFileBuffer,0,uiFileSize+1);
	bRet = file.Open(strFilePath,CFile::typeBinary|CFile::modeRead);
	if (!bRet)
	{
		delete []pFileBuffer;
		return -3;
	}
	uiRead = file.Read(pFileBuffer,uiFileSize);
	if (uiRead!=uiFileSize)
	{
		delete[] pFileBuffer;
		file.Close();
		return -4;
	}
	string strPackFile=(PCHAR)pFileBuffer;
	stringstream packStream(strPackFile);
	delete []pFileBuffer;
	file.Close();
	
	packageSet.clear();
	string strLine,strName,strFile;
	string::size_type line_size,posSpace,posTab,pos;
	
	while (!packStream.eof())
	{
		getline(packStream,strLine);
		line_size = strLine.size();
		if (strLine[line_size-1]=='\r')
		{
			strLine = strLine.substr(0,line_size-1);
		}
		strLine.erase(0,strLine.find_first_not_of(' '));
		strLine.erase(0,strLine.find_first_not_of('\t'));
		if (strLine[0]=='#')
		{// 注释行
			continue;
		}
		posSpace = strLine.find(' ');
		posTab = strLine.find('\t');
		if ((posSpace==string::npos)&&(posTab==string::npos))
		{
			continue;
		}
		if (posSpace>posTab)
		{
			pos = posTab;
		}
		else
			pos = posSpace;
		strName = strLine.substr(0,pos);
		strFile = strLine.substr(pos+1);
		strFile.erase(0,strFile.find_first_not_of(' '));
		strFile.erase(0,strFile.find_first_not_of('\t'));
		strFile.erase(strFile.find_last_not_of(' ')+1);
		strFile.erase(strFile.find_last_not_of('\t')+1);

		strcpy(packageItem.name,strName.c_str());
		strcpy(packageItem.file,strFile.c_str());
		packageSet.push_back(packageItem);

	}
	return 0;
}
int FindItem(VECTOR_PACKAGE_SET &packageSet, const char* name)
{
	int i;
	
	for(i=0; i<packageSet.size(); i++)
		if( !stricmp(packageSet[i].name, name) )
			return i;
		
	return -1;
}
BOOL GetAbsolutePath(CString strRelativePath,CString &strAbsolutePath)
{
	strRelativePath.TrimLeft();
	strRelativePath.TrimRight();
	strRelativePath.MakeUpper();
	strRelativePath.Replace(_T('/'),_T('\\'));
	STRING_SET listPath;
	listPath.clear();
	
	CString strCurrentDir;
	DWORD dwRet;
	dwRet = GetModuleFileName(NULL, strCurrentDir.GetBuffer(MAX_PATH), MAX_PATH);
	if (dwRet==0)
	{
		strCurrentDir.ReleaseBuffer();
		return FALSE;
	}
	strCurrentDir.ReleaseBuffer();
	strCurrentDir = strCurrentDir.Left(strCurrentDir.ReverseFind(_T('\\'))+1);
	
	if (PathIsRelative(strRelativePath))
	{
		strRelativePath = strCurrentDir + strRelativePath;
	}
	
	CString strValue;
	int pos;
	pos = strRelativePath.Find(_T('\\'));

	
	while(pos!=-1)
	{
		strValue = strRelativePath.Left(pos);
		listPath.push_back(strValue);
		strRelativePath.Delete(0,pos+1);
		pos = strRelativePath.Find(_T('\\'));
	}
	if (!strRelativePath.IsEmpty())
	{
		listPath.push_back(strRelativePath);
	}
	string_list_iter iter;
	for (iter= listPath.begin();iter!=listPath.end();)
	{
		if ((*iter)==_T("."))
		{
			iter = listPath.erase(iter);
			continue;
		}
		else if ((*iter)==_T(".."))
		{
			iter = listPath.erase(iter);
			iter--;
			if (iter==listPath.begin())
			{//error to root
				return FALSE;
			}
			iter = listPath.erase(iter);
			continue;
		}
		else
			iter++;
	}
	if (listPath.size()<=0)
	{
		return FALSE;
	}
	strAbsolutePath = _T("");
	for (iter= listPath.begin();iter!=listPath.end();iter++)
	{
		strAbsolutePath = strAbsolutePath + (*iter) + _T("\\");
	}
	strAbsolutePath = strAbsolutePath.Left(strAbsolutePath.GetLength()-1);
	return TRUE;
	
}

BOOL CPortApkToolDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	int menuID = LOWORD(wParam);
	/*
	if(menuID == POPMENU_DEL)
	{
	}
	else
	{

	}
	*/
	return CDialog::OnCommand(wParam, lParam);
}

int CPortApkToolDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 设置寻找标记
    ::SetProp(m_hWnd, _T("rkFactory"), (HANDLE)1);
	return 0;
}

void CPortApkToolDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	// 删除寻找标记
    ::RemoveProp(m_hWnd,  _T("rkFactory"));

}
