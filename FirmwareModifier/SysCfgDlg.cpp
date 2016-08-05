// SysCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FirmwareModifier.h"
#include "SysCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysCfgDlg dialog


CSysCfgDlg::CSysCfgDlg(CWnd* pParent /*=NULL*/, const CString& strFilePath)
	: CNotePadBase(pParent, strFilePath)
{
	//{{AFX_DATA_INIT(CSysCfgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSysCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysCfgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CSysCfgDlg)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_WM_CLOSE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysCfgDlg message handlers

BOOL CSysCfgDlg::OnInitDialog() 
{
	CNotePadBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysCfgDlg::OnBtnClose()
{
    //EndDialog(0);
    CNotePadBase::OnClose();
}

void CSysCfgDlg::OnSaveFile()
{
    CFile file;
    CString content;
    CString strTxt;
    //char *pSrcBuf, *pDstBuf;
    int nSrcSize; //nDstSize;
    CString str_find,str_name; 

    UpdateData();
    GetDlgItemText(IDC_EDIT, content);
    if(!content.IsEmpty())
    {
		if(file.Open(m_strFilePath, CFile::modeWrite | CFile::modeCreate|CFile::typeBinary))
		{
			//file.SeekToBegin();
			//file.Write("\xef\xbb\xbf", 3);	
			char *pSrcBuf=NULL;
			content.Replace(_T("\r\n"),_T("\n"));
			UnicodeToU8(pSrcBuf, nSrcSize, (LPTSTR)(LPCTSTR)content);
			file.Write(pSrcBuf, (nSrcSize-1) * sizeof(char));
			if(pSrcBuf!=NULL)
				delete[] pSrcBuf;
			
		}
		else
		{
				GetPrivateProfileStringW(_T("Logo"),_T("ERROR_INITRC_NO_EXIST"),_T("ERROR_INITRC_NO_EXIST"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
				MessageBox(strTxt + m_strFilePath + _T(".tmp"), m_strMsgBoxTitle, MB_OK);
				strTxt.ReleaseBuffer(MAX_LENGTH);
        }
		/*
        UnicodeToAnsi(pSrcBuf, nSrcSize, (LPTSTR)(LPCTSTR)content);
        pDstBuf = new char[nSrcSize];
        ConvertDos2Unix(pSrcBuf, pDstBuf, nDstSize);
        if (file.Open(m_strFilePath + _T(".tmp"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
        {
            file.Write(pDstBuf, nDstSize);
            file.Close();
            
            CFile::Remove(m_strFilePath);
            CFile::Rename(m_strFilePath + _T(".tmp"), m_strFilePath);
            
            if (pSrcBuf != NULL)
            {
                delete[] pSrcBuf;
                pSrcBuf = NULL;
            }
            
            if (pDstBuf != NULL)
            {
                delete[] pDstBuf;
                pDstBuf = NULL;
            }
        } 
        else
        {
            GetPrivateProfileStringW(_T("Logo"),_T("ERROR_INITRC_NO_EXIST"),_T("ERROR_INITRC_NO_EXIST"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt + m_strFilePath + _T(".tmp"), m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
        }
		*/
    }
}


void CSysCfgDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CNotePadBase::OnClose();
}

void CSysCfgDlg::OnOK()
{
    CNotePadBase::OnClose();
}

void CSysCfgDlg::OnCancel()
{
    CNotePadBase::OnClose(); 
}

BOOL CSysCfgDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	HWND hFocus;//Edit»òRichEditµÄ¾ä±ú
	int LineNum;
	CString strLinedex;
	//((CEdit*)GetDlgItem(IDC_EDIT))->SetFocus();
	hFocus=((CEdit*)GetDlgItem(IDC_EDIT))->m_hWnd;
	
	int LineCount=::SendMessage(hFocus,EM_GETLINECOUNT,0,0);
	int LineIndexNow=::SendMessage(hFocus,EM_LINEINDEX,((WPARAM)-1),0);
	for(int i=0;i<LineCount;i++)
	{
		int LineIndex = ::SendMessage(hFocus,EM_LINEINDEX,((WPARAM)i),0);
		if(LineIndex == LineIndexNow)
		{
			LineNum=i;
			strLinedex.Format(_T("Line:%d"),LineNum+1);
			((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(strLinedex);
			break;
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

