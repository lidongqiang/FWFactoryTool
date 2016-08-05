// TestPluginRangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testDll.h"
#include "TestPluginRangeDlg.h"

// #include "ximage.h"
// #pragma comment(lib, "png.lib")
// #pragma comment(lib, "libdcr.lib")
// #pragma comment(lib, "jpeg.lib")
// #pragma comment(lib, "zlib.lib")
// #pragma comment(lib, "tiff.lib")
// #pragma comment(lib, "jasper.lib")
// #pragma comment(lib, "cximage.lib")
// #pragma comment(lib, "mng.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestPluginRangeDlg dialog


CTestPluginRangeDlg::CTestPluginRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestPluginRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestPluginRangeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTestPluginRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestPluginRangeDlg)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestPluginRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CTestPluginRangeDlg)
	ON_BN_CLICKED(IDC_BUTTON_TOP_LEFT, OnButtonTopLeft)
	ON_BN_CLICKED(IDC_BUTTON_TOP_RIGHT, OnButtonTopRight)
	ON_BN_CLICKED(IDC_BUTTON_BOTTOM_LEFT, OnButtonBottomLeft)
	ON_BN_CLICKED(IDC_BUTTON_BOTTOM_RIGHT, OnButtonBottomRight)
	ON_BN_CLICKED(IDC_BUTTON_GET_DIR, OnButtonGetDir)
	ON_MESSAGE(WM_PACK_FINISH,OnPackFinishMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPluginRangeDlg message handlers

void CTestPluginRangeDlg::OnButtonTopLeft() 
{
	// TODO: Add your control notification handler code here
	CString strTitle;
	GetDlgItemText(IDC_BUTTON_TOP_LEFT,strTitle);
	MessageBox(strTitle);
// 	CxImage image;
// 	CString strFileName; // 图像文件名
// 	CFileDialog dlg( TRUE, _T("BMP"), NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,_T("Bmp Files(*.bmp)|*.bmp|All Files(*.*)|*.*||"));
// 	if (dlg.DoModal()==IDOK)
// 	{
// 		strFileName = dlg.GetFileName();
// 		// 加载BMP位图
// 		image.Load( (LPCTSTR)strFileName, CXIMAGE_FORMAT_BMP );
// 		// 判断图像是否有效
// 		
// 		if (image.IsValid())
// 		{
// 			// 判断图像颜色数
// 			if (!image.IsGrayScale())
// 				image.IncreaseBpp( 24 );
// 			// 设置Jpeg图像的质量
// 			image.SetJpegQuality( 80 );
// 			// 将位图转换成JPEG格式图像
// 			image.Save(_T("export.jpg"),CXIMAGE_FORMAT_JPG);
// 			MessageBox(_T("bmp to jpeg ok."));
// 		}
// 	}
}

void CTestPluginRangeDlg::OnButtonTopRight() 
{
	// TODO: Add your control notification handler code here
	CString strTitle;
	GetDlgItemText(IDC_BUTTON_TOP_RIGHT,strTitle);
	MessageBox(strTitle);
}

void CTestPluginRangeDlg::OnButtonBottomLeft() 
{
	// TODO: Add your control notification handler code here
	CString strTitle;
	GetDlgItemText(IDC_BUTTON_BOTTOM_LEFT,strTitle);
	MessageBox(strTitle);
}

void CTestPluginRangeDlg::OnButtonBottomRight() 
{
	// TODO: Add your control notification handler code here
	CString strTitle;
	GetDlgItemText(IDC_BUTTON_BOTTOM_RIGHT,strTitle);
	MessageBox(strTitle);
	::SendMessage((HWND)g_msgHwnd,WM_START_PACK,0,0);
}
void CTestPluginRangeDlg::OnPackFinishMsg() 
{
    int x;
	x=1;
	return;
}

void CTestPluginRangeDlg::OnButtonGetDir() 
{
	// TODO: Add your control notification handler code here
	bool bIsRun=true;
	::SendMessage((HWND)g_msgHwnd,WM_TEST_RUNNING,0,(LPARAM)&bIsRun);
	if (!bIsRun)
	{
		char szDir[MAX_PATH];
		memset(szDir,0,MAX_PATH);
		::SendMessage((HWND)g_msgHwnd,WM_GET_DIR,(WPARAM)EmMsg_System_key,(LPARAM)szDir);
		if (strlen(szDir)>0)
		{
			BOOL bRet;
			LPWSTR lpDir;
			int nSize;
			bRet = AnsiToUnicode(lpDir,nSize,szDir);
			if (bRet)
			{
				MessageBox(lpDir);
				delete []lpDir;
				lpDir = NULL;
			}
			
		}
	}
}
BOOL AnsiToUnicode(LPWSTR &lpwzDst,int &nDstSize,LPCSTR lpszSrc)
{
	lpwzDst = NULL;
	if (!lpszSrc)
	{
		return FALSE;
	}
	int nSrcSize;
	nSrcSize = strlen(lpszSrc);
	if (nSrcSize<=0)
	{
		return FALSE;
	}
	nDstSize = MultiByteToWideChar(CP_ACP,0,lpszSrc,-1,NULL,0);
	if (nDstSize<=0)
	{
		return FALSE;
	}
	lpwzDst = new WCHAR[nDstSize];
	if (!lpwzDst)
	{
		return FALSE;
	}
	int nCopySize;
	nCopySize = MultiByteToWideChar(CP_ACP,0,lpszSrc,-1,lpwzDst,nDstSize);
	if (nCopySize<=0)
	{
		delete []lpwzDst;
		lpwzDst = NULL;
		return FALSE;
	}
	return TRUE;
}
BOOL UnicodeToAnsi(LPSTR &lpszDst,int &nDstSize,LPCWSTR lpwzSrc)
{
	lpszDst = NULL;
	if (!lpwzSrc)
	{
		return FALSE;
	}
	int nSrcSize;
	nSrcSize = wcslen(lpwzSrc);
	if (nSrcSize<=0)
	{
		return FALSE;
	}
	nDstSize = WideCharToMultiByte(CP_ACP,0,lpwzSrc,-1,NULL,0,NULL,NULL);
	if (nDstSize<=0)
	{
		return FALSE;
	}
	lpszDst = new CHAR[nDstSize];
	if (!lpszDst)
	{
		return FALSE;
	}
	int nCopySize;
	nCopySize = WideCharToMultiByte(CP_ACP,0,lpwzSrc,-1,lpszDst,nDstSize,NULL,NULL);
	if (nCopySize<=0)
	{
		delete []lpszDst;
		lpszDst = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL CTestPluginRangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tab.InsertItem(0,_T("内核 logo"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
