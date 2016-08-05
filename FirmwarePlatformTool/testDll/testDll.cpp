// testDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "testDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
ENUM_LANGUAGE g_emLang=EmLang_CHN;
DWORD g_msgHwnd=0;
DWORD g_childHwnd=0;
CTestMfcDllApp theApp;
const char szTabTitle_En[] = "Test Plugin";
const char szTabTitle_Ch[] = "²å¼þ²âÊÔ";
// 
// BOOL APIENTRY DllMain( HANDLE hModule, 
// 					  DWORD  ul_reason_for_call, 
// 					  LPVOID lpReserved
// 					  )
// {
//     
// 	switch (ul_reason_for_call)
// 	{
// 	case DLL_PROCESS_ATTACH:
// 		theApp.m_hInstance = (HINSTANCE)hModule;
// 	case DLL_THREAD_ATTACH:
// 	case DLL_THREAD_DETACH:
// 	case DLL_PROCESS_DETACH:
// 		break;
//     }
//     return TRUE;
// }
void _stdcall SetMsgHandle(DWORD msgHwnd)
{
	if (msgHwnd!=0)
	{
		g_msgHwnd = msgHwnd;
	}
}
void _stdcall SetWindowLanguage(int emLang)
{
	if ((ENUM_LANGUAGE)emLang==EmLang_CHN)
	{
		g_emLang = EmLang_CHN;
	}
	else if ((ENUM_LANGUAGE)emLang==EmLang_ENG)
	{
		g_emLang = EmLang_ENG;
	}
	
}
bool _stdcall GetTabTitle(char *szTitle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!szTitle)
	{
		return false;
	}

	CString strTitle;
	HINSTANCE hInstance ;
	hInstance = AfxGetInstanceHandle();
	if (g_emLang==EmLang_CHN)
	{
		LoadString(hInstance,IDS_TAB_TITLE_CH,strTitle.GetBuffer(MAX_PATH),MAX_PATH);
		strcpy(szTitle,szTabTitle_Ch);
	}
	else if (g_emLang==EmLang_ENG)
	{
		LoadString(hInstance,IDS_TAB_TITLE_EN,strTitle.GetBuffer(MAX_PATH),MAX_PATH);
		strcpy(szTitle,szTabTitle_En);
	}
	else
		return false;
	strTitle.ReleaseBuffer();
	BOOL bRet;
	LPSTR lpTitle;
	int nTitleSize;
	bRet = UnicodeToAnsi(lpTitle,nTitleSize,(LPTSTR)(LPCTSTR)strTitle);
	if (!bRet)
	{
		return false;	
	}
	strcpy(szTitle,lpTitle);
	delete []lpTitle;
	lpTitle = NULL;
	return true;
}
DWORD _stdcall CreateChildWindow(DWORD parentHwnd)
{
	if (parentHwnd==0)
	{
		return 0;
	}

	return theApp.CreateTestDialog(parentHwnd);
}
bool _stdcall DestroyChildWindow(DWORD hwnd)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
 	
	theApp.DestroyTestDialog();
	return true;
}

BEGIN_MESSAGE_MAP(CTestMfcDllApp, CWinApp)
END_MESSAGE_MAP()


CTestMfcDllApp::CTestMfcDllApp()
{
	m_pChildDialog = NULL;	
}
CTestMfcDllApp::~CTestMfcDllApp()
{
	DestroyTestDialog();
}
DWORD CTestMfcDllApp::CreateTestDialog(DWORD dwParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if (!m_pChildDialog)
	{
		m_pChildDialog = new CTestPluginRangeDlg();
		if (m_pChildDialog)
		{
			CWnd *pParentWnd = CWnd::FromHandle((HWND)dwParent);
			BOOL bRet;
			bRet = m_pChildDialog->Create( IDD_DIALOG_TEST_PLATFORM, pParentWnd );
			if (!bRet)
			{
				delete m_pChildDialog;
				m_pChildDialog = NULL;
				return 0;
			}
			else
			{
				TCHAR szTitle[MAX_PATH];
				memset(szTitle,0,sizeof(TCHAR)*MAX_PATH);
				HINSTANCE hModule= AfxGetInstanceHandle();
				if (g_emLang==EmLang_CHN)
				{
					LoadString(hModule,IDS_TOP_LEFT_CH,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_TOP_LEFT,szTitle);
					LoadString(hModule,IDS_TOP_RIGHT_CH,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_TOP_RIGHT,szTitle);
					LoadString(hModule,IDS_BOTTOM_LEFT_CH,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_BOTTOM_LEFT,szTitle);
					LoadString(hModule,IDS_BOTTOM_RIGHT_CH,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_BOTTOM_RIGHT,szTitle);
					LoadString(hModule,IDS_GET_DIR_CH,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_GET_DIR,szTitle);
					
				}
				else if (g_emLang==EmLang_ENG)
				{
					LoadString(hModule,IDS_TOP_LEFT_EN,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_TOP_LEFT,szTitle);
					LoadString(hModule,IDS_TOP_RIGHT_EN,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_TOP_RIGHT,szTitle);
					LoadString(hModule,IDS_BOTTOM_LEFT_EN,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_BOTTOM_LEFT,szTitle);
					LoadString(hModule,IDS_BOTTOM_RIGHT_EN,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_BOTTOM_RIGHT,szTitle);
					LoadString(hModule,IDS_GET_DIR_EN,szTitle,MAX_PATH);
					m_pChildDialog->SetDlgItemText(IDC_BUTTON_GET_DIR,szTitle);
				}
			}
		}	
	}
	return (DWORD)m_pChildDialog->m_hWnd;
}
void CTestMfcDllApp::DestroyTestDialog()
{
	if (m_pChildDialog)
	{
		m_pChildDialog->DestroyWindow();
		delete m_pChildDialog;
		m_pChildDialog = NULL;
	}		
}

