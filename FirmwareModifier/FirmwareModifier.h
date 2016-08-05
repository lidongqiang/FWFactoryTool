// FirmwareModifier.h : main header file for the TESTDLL DLL
//

#if !defined(AFX_TESTDLL_H__AC027D26_DD27_480D_9783_16F923632561__INCLUDED_)
#define AFX_TESTDLL_H__AC027D26_DD27_480D_9783_16F923632561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "TestPluginRangeDlg.h"
#ifdef FWPLUGIN_EXPORTS
#define FW_PLUGIN_API __declspec(dllexport)
#else
#define FW_PLUGIN_API __declspec(dllimport)
#endif

FW_PLUGIN_API void _stdcall SetMsgHandle(DWORD msgHwnd);
FW_PLUGIN_API void _stdcall SetWindowLanguage(int emLang);
FW_PLUGIN_API bool _stdcall GetTabTitle(char *szTitle);
FW_PLUGIN_API DWORD _stdcall CreateChildWindow(DWORD parentHwnd);
FW_PLUGIN_API bool _stdcall DestroyChildWindow(DWORD hwnd);

class CTestMfcDllApp : public CWinApp
{
public:
	CTestMfcDllApp();
	~CTestMfcDllApp();
	DWORD CreateTestDialog(DWORD dwParent);
	void DestroyTestDialog();
	DECLARE_MESSAGE_MAP()
private:
	CTestPluginRangeDlg *m_pChildDialog;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLL_H__AC027D26_DD27_480D_9783_16F923632561__INCLUDED_)
