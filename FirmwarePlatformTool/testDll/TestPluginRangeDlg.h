#if !defined(AFX_TESTPLUGINRANGEDLG_H__14601DD0_DD7B_4338_85EE_98AD024C6C53__INCLUDED_)
#define AFX_TESTPLUGINRANGEDLG_H__14601DD0_DD7B_4338_85EE_98AD024C6C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestPluginRangeDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTestPluginRangeDlg dialog

class CTestPluginRangeDlg : public CDialog
{
// Construction
public:
	CTestPluginRangeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestPluginRangeDlg)
	enum { IDD = IDD_DIALOG_TEST_PLATFORM };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPluginRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestPluginRangeDlg)
	afx_msg void OnButtonTopLeft();
	afx_msg void OnButtonTopRight();
	afx_msg void OnButtonBottomLeft();
	afx_msg void OnButtonBottomRight();
	afx_msg void OnButtonGetDir();
	afx_msg void OnPackFinishMsg(); 
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
BOOL AnsiToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc);
BOOL UnicodeToAnsi(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPLUGINRANGEDLG_H__14601DD0_DD7B_4338_85EE_98AD024C6C53__INCLUDED_)
