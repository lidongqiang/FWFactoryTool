#if !defined(AFX_SYSCFGDLG_H__F03E9C29_4A4D_46C3_9E69_4577A12FD07F__INCLUDED_)
#define AFX_SYSCFGDLG_H__F03E9C29_4A4D_46C3_9E69_4577A12FD07F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysCfgDlg.h : header file
//
#include "NotePadBase.h"
/////////////////////////////////////////////////////////////////////////////
// CSysCfgDlg dialog

class CSysCfgDlg : public CNotePadBase
{
// Construction
public:
	CSysCfgDlg(CWnd* pParent = NULL, const CString& strFilePath = _T(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysCfgDlg)
	enum { IDD = IDD_DIALOG_NOTE_PAD};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    virtual void OnSaveFile();
    
	// Generated message map functions
	//{{AFX_MSG(CSysCfgDlg)
	virtual BOOL OnInitDialog();
    afx_msg void OnBtnClose();
	afx_msg void OnClose();
    virtual void OnOK();
    virtual void OnCancel( );
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSCFGDLG_H__F03E9C29_4A4D_46C3_9E69_4577A12FD07F__INCLUDED_)
