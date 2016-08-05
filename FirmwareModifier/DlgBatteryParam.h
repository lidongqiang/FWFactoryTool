#if !defined(AFX_DLGBATTERYPARAM_H__FF677F67_B779_4335_93B6_A51669385FFB__INCLUDED_)
#define AFX_DLGBATTERYPARAM_H__FF677F67_B779_4335_93B6_A51669385FFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBatteryParam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBatteryParam dialog


typedef struct _STRUCT_BATTERY_PARAM
{
    unsigned char szParamTag[12];
    DWORD dwCell;
    DWORD dwPullUp;
    DWORD dwPullDown;
    DWORD dwDischarge[11];
    DWORD dwCharge[11];
}STRUCT_BATTERY_PARAM,*PSTRUCT_BATTERY_PARAM;

extern BOOL CheckKernel(BYTE *pKernel);
extern int SearchTag(BYTE *pKernel);
extern ULONG CRC_32(PBYTE pData, ULONG ulSize);

class CDlgBatteryParam : public CDialog
{
// Construction
public:
	CDlgBatteryParam(CWnd* pParent = NULL);   // standard constructor
	CStringList ResourceList;
	CString strResourceDtsPath;
	CString strRootPath;
	CString strResourceDtbPath;
	CString strResourcePath;

// Dialog Data
	//{{AFX_DATA(CDlgBatteryParam)
	enum { IDD = IDD_DIALOG_BATTERY_PARAM };
	int		m_batteryCell;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBatteryParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBatteryParam)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonReadBattery();
	afx_msg void OnButtonWriteBattery();
	afx_msg void OnBtnReadBatteryData();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
    int ReadBatteryParam( CString strKernelPath, STRUCT_BATTERY_PARAM* pBatteryParam );
    void UpdateParamToInterface( STRUCT_BATTERY_PARAM* pBatteryParam );
    void ClearParamInterface();
    CString m_strMsgBoxTitle;
private:
	void WirteBatteryParameters();
	void InitUIComponents();
	void ReadBatteryParameters();
    BOOL GetParamFromInterface( STRUCT_BATTERY_PARAM* pBatteryParam );
    int WriteBatteryParam( CString strKernelPath, STRUCT_BATTERY_PARAM* pBatteryParam );
    BOOL SaveKernel( BYTE *pKernel,UINT uiKernelSize,CString strKernelPath );
    char * read_file( CString fn, unsigned *_sz );
	BOOL BatteryParamReadFile( LPCTSTR pszFilePath);
	DWORD GetDtbFileSize(CString mPath); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBATTERYPARAM_H__FF677F67_B779_4335_93B6_A51669385FFB__INCLUDED_)
