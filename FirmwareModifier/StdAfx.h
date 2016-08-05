// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B05BF234_E4FF_4790_8702_6ADAAF636DDD__INCLUDED_)
#define AFX_STDAFX_H__B05BF234_E4FF_4790_8702_6ADAAF636DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DWORD_PTR DWORD*

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "atlconv.h"
#pragma warning (disable : 4786)

#define ULONG_PTR ULONG*
#define DWORD_PTR DWORD*
#pragma comment(lib, "setupapi.lib")

#include <setupapi.h>
#include <cfgmgr32.h>
#include <afxmt.h>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <shlwapi.h> 


#include "CommonDefine.h"
extern ENUM_LANGUAGE g_emLang;
extern DWORD g_msgHwnd;
extern DWORD g_childHwnd;
extern CString g_strLanguageConfigFilePath;

#include "cmBase.h"
#ifdef _DEBUG
#pragma comment(lib,"cmMfcLib_Debug.lib")
#else
#pragma comment(lib,"cmMfcLib.lib")
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__B05BF234_E4FF_4790_8702_6ADAAF636DDD__INCLUDED_)
