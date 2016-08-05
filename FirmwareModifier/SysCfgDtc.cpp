// SysCfgDtc.cpp : implementation file
//

#include "stdafx.h"
#include "firmwaremodifier.h"
#include "SysCfgDtc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysCfgDtc dialog


CSysCfgDtc::CSysCfgDtc(CWnd* pParent /*=NULL*/, const CString& strFilePath)
	: CNotePadBase(pParent, strFilePath)
{
	//{{AFX_DATA_INIT(CSysCfgDtc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	 m_strFilePath = strFilePath;
}


void CSysCfgDtc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysCfgDtc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysCfgDtc, CDialog)
	//{{AFX_MSG_MAP(CSysCfgDtc)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysCfgDtc message handlers

BOOL CSysCfgDtc::OnInitDialog() 
{
	CNotePadBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysCfgDtc::OnBtnClose()
{
    //EndDialog(0);
    CNotePadBase::OnClose();
}

void CSysCfgDtc::OnSaveFile()
{
   
   	int ret=0;
    CString FileType, strTxt;
	FileType=_T("dtb Files|*.dtb||");
    
    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,FileType);
	
    GetPrivateProfileStringW(_T("Common"),_T("OPEN"),_T("OPEN"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);	
    fileopenbox.m_ofn.lpstrTitle = strTxt;
    strTxt.ReleaseBuffer(MAX_LENGTH);
    
    if(fileopenbox.DoModal() == IDOK)
    {
        int n = 0,size=0;
		
		if(!CopyFile(fileopenbox.GetPathName(),strResourceDtbPath,FALSE)) 
        { 
            MessageBox(_T("Copy File Fail!"),MB_OK);
			return;
        }
		
		size=GetDtbFileSize(strResourceDtbPath);
		if(size < 0)
			g_pPluginLogObject->Record(_T("open %s failed"), strResourceDtbPath);
		
		ret=resource_importDts_Dtb(strResourcePath,strResourceDtbPath);
		if(ret<0)
		{
			g_pPluginLogObject->Record(_T("Import Logo failed :%d"), ret);
			return;
		}
		
		GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);	
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
	}
}


void CSysCfgDtc::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CNotePadBase::OnClose();
}

void CSysCfgDtc::OnOK()
{
    CNotePadBase::OnClose();
}

void CSysCfgDtc::OnCancel()
{
    CNotePadBase::OnClose(); 
}
