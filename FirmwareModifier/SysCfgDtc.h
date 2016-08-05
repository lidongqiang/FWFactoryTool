#if !defined(AFX_SYSCFGDTC_H__B5C54FB3_957E_4B91_A026_F3D69AB78250__INCLUDED_)
#define AFX_SYSCFGDTC_H__B5C54FB3_957E_4B91_A026_F3D69AB78250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysCfgDtc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysCfgDtc dialog

class CSysCfgDtc :  public CNotePadBase
{
// Construction
public:
	CSysCfgDtc(CWnd* pParent = NULL, const CString& strFilePath = _T(""));  // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysCfgDtc)
	enum { IDD = IDD_DIALOG_NOTE_PAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysCfgDtc)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSCFGDTC_H__B5C54FB3_957E_4B91_A026_F3D69AB78250__INCLUDED_)
