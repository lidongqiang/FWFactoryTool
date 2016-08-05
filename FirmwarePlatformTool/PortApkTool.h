// PortApkTool.h : main header file for the PORTAPKTOOL application
//

#if !defined(AFX_PORTAPKTOOL_H__C09039C9_D77D_4429_9893_B1C4AB245B7C__INCLUDED_)
#define AFX_PORTAPKTOOL_H__C09039C9_D77D_4429_9893_B1C4AB245B7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPortApkToolApp:
// See PortApkTool.cpp for the implementation of this class
//

class CPortApkToolApp : public CWinApp
{
public:
	CPortApkToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortApkToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPortApkToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTAPKTOOL_H__C09039C9_D77D_4429_9893_B1C4AB245B7C__INCLUDED_)
