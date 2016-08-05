#if !defined(AFX_CHARGEIMAGE_H__5BC2822B_5C23_4158_84CE_16EF7ADC405D__INCLUDED_)
#define AFX_CHARGEIMAGE_H__5BC2822B_5C23_4158_84CE_16EF7ADC405D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChargeImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChargeImage dialog

class CChargeImage : public CDialog
{
// Construction
public:
	CString strRootPath;
    CString strAndroidPath;
    CString strSrcChargeImagePath;
	CString strRecoveryChargeImagePath;
	CString strErrorImagePath;
	CString strUbootChargeImagePath;
	CString strLanguagePath;
	CString m_strMsgBoxTitle;
	int strAndroidVersion;
	CChargeImage(CWnd* pParent = NULL);   // standard constructor
	int ButtonDisplayPictrue(CString filePath,int index);
	int ConvertToBmp(CString SrcFilePath,CString DesFilePath);
	void OpenBatteryReplaceDialog(CString strBatterySrcpath,CString strBatteryBmppath,int ubootflag,int index);
	void SetLanguagePath(CString LanguagePath);
	void SetAndroidVersion(int version);
	void OnInitUI();

// Dialog Data
	//{{AFX_DATA(CChargeImage)
	enum { IDD = IDD_CHARGE_IMAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChargeImage)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChargeImage)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnBatteryButton1();
	afx_msg void OnBatteryButton2();
	afx_msg void OnBatteryButton3();
	afx_msg void OnBatteryButton4();
	afx_msg void OnBatteryButton5();
	afx_msg void OnBatteryButton6();
	afx_msg void OnBatteryChargeButton();
	afx_msg void OnBatteryFailButton();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    CString GetBootChargeImagePath();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARGEIMAGE_H__5BC2822B_5C23_4158_84CE_16EF7ADC405D__INCLUDED_)
