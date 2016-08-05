#if !defined(AFX_NOTEPADBASE_H__1C7A2E62_A2C0_4161_A9E2_4393B36323D9__INCLUDED_)
#define AFX_NOTEPADBASE_H__1C7A2E62_A2C0_4161_A9E2_4393B36323D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotePadBase.h : header file
//
#include <vector>

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CNotePadBase dialog

class CNotePadBase : public CDialog
{
// Construction
public:
	CNotePadBase(CWnd* pParent = NULL, const CString &strFilePath = _T(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNotePadBase)
	enum { IDD = IDD_DIALOG_NOTE_PAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotePadBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
    CString m_strMsgBoxTitle;
    CString m_strFilePath;
	CStringList NotePadList;
    virtual void OnSaveFile() = 0;
	// Generated message map functions
	//{{AFX_MSG(CNotePadBase)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    BOOL GetFileEncode( LPCTSTR lpszFileName, emFileEncode& fileEncode );
    BOOL NotePadReadFile( LPCTSTR pszFilePath);
private:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTEPADBASE_H__1C7A2E62_A2C0_4161_A9E2_4393B36323D9__INCLUDED_)
