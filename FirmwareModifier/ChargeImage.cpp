// ChargeImage.cpp : implementation file
//

#include "stdafx.h"
#include "FirmwareModifier.h"
#include "ChargeImage.h"
#include "ximage.h"
#include "KernelLogoTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


enum ENUM_ANDROID_VERTION{
	ANDROID_VERTION_UNKNOWN=0,
		ANDROID_VERTION_FOUR_TWO,
		ANDROID_VERTION_FOUR_FOUR,
		ANDROID_VERTION_FiVE_ZERO,
	    ANDROID_VERTION_SIX_ZERO,
};

enum ENUM_KERNEL_VERTION{
	KERNEL_VERTION_UNKNOWN=0,
		KERNEL_VERTION_THREE_ONE_ZERO,	
};
/////////////////////////////////////////////////////////////////////////////
// CChargeImage dialog
extern BOOL FileExist(CString strFileName);
extern BOOL DeleteFolder(CString str);
extern void DeleteFolderFile(CString str);
extern BOOL CreateFolder(CString strPath);

CChargeImage::CChargeImage(CWnd* pParent /*=NULL*/)
	: CDialog(CChargeImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChargeImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChargeImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChargeImage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChargeImage, CDialog)
	//{{AFX_MSG_MAP(CChargeImage)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON1, OnBatteryButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBatteryButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBatteryButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBatteryButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBatteryButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBatteryButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBatteryChargeButton)
	ON_BN_CLICKED(IDC_BUTTON8, OnBatteryFailButton)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChargeImage message handlers
int CChargeImage::ButtonDisplayPictrue(CString filePath,int index)
{
	HBITMAP   hBitmap;   
	hBitmap = (HBITMAP)::LoadImage(NULL,filePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION); 

    ((CButton *)GetDlgItem(IDC_BUTTON20+index))->SetBitmap(hBitmap);   
	//m_BitmapButton.LoadBitmaps(filePath,filePath);
	//m_BitmapButton.SubclassDlgItem(IDC_BUTTON1,this);
	//m_BitmapButton.SizeToContent();
	//m_BitmapButton.AutoLoad(IDC_BUTTON1,this);
    return 1;
}

int CChargeImage::ConvertToBmp(CString SrcFilePath,CString DesFilePath)
{
    CxImage image,rotateImage;
    int type;
    
    CString ext = SrcFilePath.Right(SrcFilePath.GetLength()-SrcFilePath.ReverseFind('.') - 1);
    
    if (ext == "bmp" || ext == "BMP")
        type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
    else if (ext == "jpg" || ext == "jpeg" || ext == "JPG" || ext == "JPEG")
        type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
    else if (ext == "gif" || ext == "GIF")
        type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
    else if (ext == "png" || ext == "PNG")
        type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
    else if (ext == "mng" || ext == "jng" || ext == "MNG" || ext == "JNG")
        type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
    else if (ext == "ico" || ext == "ICO")
        type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
    else if (ext == "tiff" || ext == "tif" || ext == "TIFF" || ext == "TIF")
        type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
    else if (ext == "tga" || ext == "TGA")
        type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
    else if (ext == "pcx" || ext == "PCX")
        type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
    else if (ext == "wbmp" || ext == "WBMP")
        type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
    else if (ext == "wmf" || ext == "emf" || ext == "WMF" || ext == "EMF")
        type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
    else if (ext == "j2k" || ext == "J2K")
        type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
    else if (ext == "jbg" || ext == "JBG")
        type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
    else if (ext == "jp2" || ext == "JP2")
        type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
    else if (ext == "jpc" || ext == "j2c" || ext == "JPC" || ext == "J2C")
        type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
    else if (ext == "pgx" || ext == "PGX")
        type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
    else if (ext == "ras" || ext == "RAS")
        type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
    else if (ext == "pnm" || ext == "pgm" || ext == "ppm" || ext == "PNM" || ext == "PGM" || ext == "PPM")
        type = CXIMAGE_FORMAT_PNM;
#endif
    else
        type = CXIMAGE_FORMAT_UNKNOWN;
    
    image.Load( (LPCTSTR)SrcFilePath, type);
    // ÅÐ¶ÏÍ¼ÏñÊÇ·ñÓÐÐ§
    
    if (image.IsValid())
    {
		RECT rect;
		CWnd *pWnd=GetDlgItem(IDC_BUTTON1);
		pWnd->GetClientRect(&rect);
   
		image.Resample(rect.right,rect.bottom,2);
		//rect.bottom=0x50;
		//image.Draw(pWnd->GetDC()->GetSafeHdc(),rect);
        image.Save(DesFilePath,CXIMAGE_FORMAT_BMP);
    }
    
    return 1;
}    

void CChargeImage::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CChargeImage::SetLanguagePath(CString LanguagePath)
{
	GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_TITLE"),_T("MESSAGE_TITLE"),
        m_strMsgBoxTitle.GetBuffer(MAX_LENGTH),MAX_LENGTH, LanguagePath);
	
	strLanguagePath=LanguagePath;
	return;
}

void CChargeImage::SetAndroidVersion(int version)
{
	strAndroidVersion=version;
	return;
}

void CChargeImage::OnInitUI() 
{
	int i=0;
	CString strTxt;
	int ubootcheck=FALSE;	
	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();

	for(i=0;i<8;i++){
		((CButton *)GetDlgItem(IDC_BUTTON20+i))->ShowWindow(TRUE);
		((CButton *)GetDlgItem(IDC_BUTTON20+i))->EnableWindow(TRUE);
	}
	
	for(i=0;i<8;i++){
		((CStatic *)GetDlgItem(IDC_STATIC20+i))->ShowWindow(TRUE);
		strTxt.Format(_T("battery_%d"),i);
		((CStatic *)GetDlgItem(IDC_STATIC20+i))->SetWindowText(strTxt);
	}

	((CStatic *)GetDlgItem(IDC_STATIC26))->SetWindowText(_T("battery_fail"));
	((CStatic *)GetDlgItem(IDC_STATIC27))->SetWindowText(_T("battery_charge"));

	if(ubootcheck){
		((CButton *)GetDlgItem(IDC_BUTTON27))->ShowWindow(FALSE);
		((CStatic *)GetDlgItem(IDC_STATIC27))->ShowWindow(FALSE);
	}else{

		if(strAndroidVersion==ANDROID_VERTION_FiVE_ZERO||strAndroidVersion==ANDROID_VERTION_SIX_ZERO){
				for(i=2;i<8;i++)
					((CButton *)GetDlgItem(IDC_BUTTON20+i))->ShowWindow(FALSE);
				
				for(i=2;i<8;i++)
					((CStatic *)GetDlgItem(IDC_STATIC20+i))->ShowWindow(FALSE);
				
				((CStatic *)GetDlgItem(IDC_STATIC20))->SetWindowText(_T("battery_scale"));
				((CStatic *)GetDlgItem(IDC_STATIC21))->SetWindowText(_T("battery_fail"));
	
		}
	}
		
}

BOOL CChargeImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strTxt;
	CString strFilePath;
	CString BatteryImage[8];
	CString BatteryTempImage[8];
    int n,i;
    LPWSTR lpDir;
	lpDir=GetRootDir();
	strFilePath = (CString)lpDir;
	if(lpDir)
	{
		delete lpDir;
	}
	n = strFilePath.ReverseFind('\\');
	strAndroidPath = strFilePath.Left(n);
	n = strAndroidPath.ReverseFind('\\');
	strRootPath = strAndroidPath.Left(n);
  
    strSrcChargeImagePath = GetBootChargeImagePath();
	strUbootChargeImagePath=strRootPath+_T("\\dts\\images");
	strRecoveryChargeImagePath=strRootPath+_T("\\dts\\charge");
	strErrorImagePath=strRootPath+_T("\\bin\\error.bmp");

	
    GetPrivateProfileStringW(_T("Logo"),_T("ONCLICK_IMAGE_BUTTON"),_T("Please onclick Image for update"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
	GetDlgItem(IDC_STATIC_CHARGE)->SetWindowText(strTxt);
	strTxt.ReleaseBuffer(MAX_LENGTH);
	
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);

	OnInitUI();

	if(CreateFolder(strRecoveryChargeImagePath))
	{
		//SrcChangeFilePath=parameter_path+"\\Recovery\\recovery\\res\\images\\charger\\battery_2.png";
	
		if(strAndroidVersion==ANDROID_VERTION_FiVE_ZERO){
			BatteryImage[0]=strSrcChargeImagePath+"battery_scale.png";
			BatteryImage[1]=strSrcChargeImagePath+"battery_fail.png";
			BatteryTempImage[0]=strRecoveryChargeImagePath+"\\battery_scale.bmp";
			BatteryTempImage[1]=strRecoveryChargeImagePath+"\\battery_fail.bmp";

			for(i=0;i<2;i++)
			{
				ConvertToBmp(BatteryImage[i],BatteryTempImage[i]);
				ButtonDisplayPictrue(BatteryTempImage[i],i);
			}

		}else if (strAndroidVersion==ANDROID_VERTION_SIX_ZERO){
			BatteryImage[0]=strSrcChargeImagePath+"battery_scale.png";
			BatteryImage[1]=strSrcChargeImagePath+"battery_fail.png";
			BatteryTempImage[0]=strRecoveryChargeImagePath+"\\battery_scale.bmp";
			BatteryTempImage[1]=strRecoveryChargeImagePath+"\\battery_fail.bmp";
			
			for(i=0;i<2;i++)
			{
				ConvertToBmp(BatteryImage[i],BatteryTempImage[i]);
				ButtonDisplayPictrue(BatteryTempImage[i],i);
			}
		}else{
			BatteryImage[0]=strSrcChargeImagePath+"battery_0.png";
			BatteryImage[1]=strSrcChargeImagePath+"battery_1.png";
			BatteryImage[2]=strSrcChargeImagePath+"battery_2.png";
			BatteryImage[3]=strSrcChargeImagePath+"battery_3.png";
			BatteryImage[4]=strSrcChargeImagePath+"battery_4.png";
			BatteryImage[5]=strSrcChargeImagePath+"battery_5.png";
			BatteryImage[6]=strSrcChargeImagePath+"battery_charge.png";
			BatteryImage[7]=strSrcChargeImagePath+"battery_fail.png";
			
			BatteryTempImage[0]=strRecoveryChargeImagePath+"\\battery_0.bmp";
			BatteryTempImage[1]=strRecoveryChargeImagePath+"\\battery_1.bmp";
			BatteryTempImage[2]=strRecoveryChargeImagePath+"\\battery_2.bmp";
			BatteryTempImage[3]=strRecoveryChargeImagePath+"\\battery_3.bmp";
			BatteryTempImage[4]=strRecoveryChargeImagePath+"\\battery_4.bmp";
			BatteryTempImage[5]=strRecoveryChargeImagePath+"\\battery_5.bmp";
			BatteryTempImage[6]=strRecoveryChargeImagePath+"\\battery_charge.bmp";
			BatteryTempImage[7]=strRecoveryChargeImagePath+"\\battery_fail.bmp";
			
			//MessageBox(SrcChangeFilePath);
			for(i=0;i<8;i++)
			{
				ConvertToBmp(BatteryImage[i],BatteryTempImage[i]);
				ButtonDisplayPictrue(BatteryTempImage[i],i);
			}
		}	
		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChargeImage::OpenBatteryReplaceDialog(CString strBatterySrcpath,CString strBatteryBmppath,int ubootflag,int index)
{
	
	CString strTxt;
	CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("PNG|*.png||"));
	fileopenbox.m_ofn.lpstrTitle = _T ("Open Battery png");
	if(ubootflag){
		fileopenbox.m_ofn.lpstrFilter=_T("BMP\0*.bmp\0");
		fileopenbox.m_ofn.lpstrTitle = _T ("Open Battery bmp");
	}
	
    if(fileopenbox.DoModal() == IDOK)
    {       
        if(FileExist(strBatterySrcpath))
			DeleteFile(strBatterySrcpath);
       
        if(!CopyFile(fileopenbox.GetPathName(),strBatterySrcpath,FALSE)) 
        { 
            MessageBox(_T("Copy File Fail!"),MB_OK); 
        }else
        {
//             GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
//             MessageBox(strTxt, MB_OK);
//             strTxt.ReleaseBuffer(MAX_LENGTH);
			if(!ubootflag)
           		ConvertToBmp(strBatterySrcpath,strBatteryBmppath);

			ButtonDisplayPictrue(strBatteryBmppath,index);
        }
    }	
}

void CChargeImage::OnBatteryButton1() 
{
	// TODO: Add your control notification handler code here

	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;

	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_0.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_0.bmp";
	}else{
		if(strAndroidVersion==ANDROID_VERTION_FiVE_ZERO||strAndroidVersion==ANDROID_VERTION_SIX_ZERO){
			strBatterySrcpath=strSrcChargeImagePath+"battery_scale.png";
			strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_scale.bmp";	
		}else{
			strBatterySrcpath=strSrcChargeImagePath+"battery_0.png";
			strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_0.bmp";	
		}
		
	}

	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,0);
}

void CChargeImage::OnBatteryButton2() 
{
	// TODO: Add your control notification handler code here
	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;
	
	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_1.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_1.bmp";
	}else{
		if(strAndroidVersion==ANDROID_VERTION_FiVE_ZERO){
			strBatterySrcpath=strSrcChargeImagePath+"battery_fail.png";
			strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_fail.bmp";	
		}else{
			strBatterySrcpath=strSrcChargeImagePath+"battery_1.png";
			strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_1.bmp";	
		}	
	}
	
	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,1);
}

void CChargeImage::OnBatteryButton3() 
{
	// TODO: Add your control notification handler code here
	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;

	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_2.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_2.bmp";
	}else{
		strBatterySrcpath=strSrcChargeImagePath+"battery_2.png";
		strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_2.bmp";	
	}

	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,2);
}

void CChargeImage::OnBatteryButton4() 
{
	// TODO: Add your control notification handler code here
	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;
	
	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_3.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_3.bmp";
	}else{
		strBatterySrcpath=strSrcChargeImagePath+"battery_3.png";
		strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_3.bmp";	
	}
	
	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,3);
	
}

void CChargeImage::OnBatteryButton5() 
{
	// TODO: Add your control notification handler code here
	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;
	
	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_4.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_4.bmp";
	}else{
		strBatterySrcpath=strSrcChargeImagePath+"battery_4.png";
		strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_4.bmp";	
	}
	
	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,4);
}

void CChargeImage::OnBatteryButton6() 
{
	// TODO: Add your control notification handler code here
	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;
	
	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_5.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_5.bmp";
	}else{
		strBatterySrcpath=strSrcChargeImagePath+"battery_5.png";
		strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_5.bmp";	
	}
	
	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,5);
}

void CChargeImage::OnBatteryChargeButton() 
{
	// TODO: Add your control notification handler code here
	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;
	
	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_fail.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_fail.bmp";
	}else{
		strBatterySrcpath=strSrcChargeImagePath+"battery_charge.png";
		strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_charge.bmp";	
	}
	
	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,6);
}

void CChargeImage::OnBatteryFailButton() 
{
	// TODO: Add your control notification handler code here
	CString strBatterySrcpath;
	CString strBatteryBmppath;
	int ubootcheck=FALSE;
	
	ubootcheck=((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
	if(ubootcheck){
		strBatterySrcpath=strUbootChargeImagePath+"\\battery_fail.bmp";
		strBatteryBmppath=strUbootChargeImagePath+"\\battery_fail.bmp";
	}else{
		strBatterySrcpath=strSrcChargeImagePath+"battery_fail.png";
		strBatteryBmppath=strRecoveryChargeImagePath+"\\battery_fail.bmp";	
	}
	
	OpenBatteryReplaceDialog(strBatterySrcpath,strBatteryBmppath,ubootcheck,7);
}

BOOL CChargeImage::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString delFolderPath=strRootPath+_T("\\dts\\Charge");
	DeleteFolder(delFolderPath);

	CString strTxt;
	GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_CONFIRM_SAVE"),_T("MESSAGE_CONFIRM_SAVE"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, strLanguagePath);
	int iRes = MessageBox(strTxt, m_strMsgBoxTitle, MB_YESNO | MB_ICONWARNING);
    if(( iRes == IDYES )&&FileExist(strRootPath+_T("\\dts\\charge_anim_desc.txt")))
    {
		CString strCmd;
		int iRet=0;
		CString strResult;
		CString g_strCovert;
		if(FileExist(strRootPath+_T("\\dts\\logo_kernel.bmp")))
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery_kernellogo.bat");
		else
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery.bat");	
		strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
		if (!RunOhterExe(strCmd,&strResult))
		{
			
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_IMPORT_RESOURCE_LOGO"),_T("ERROR_IMPORT_RESOURCE_LOGO"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,strLanguagePath);
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
		} 
    }
	
	return CDialog::DestroyWindow();
}

CString CChargeImage::GetBootChargeImagePath()
{
    int i = 0;
    CString str;

    CString path1, path2;
	LPWSTR lpDir;
	lpDir=GetRootDir();
	path2 = path1 = (CString)lpDir;
	if(lpDir)
	{
		delete lpDir;
	}
    
    path1 = path1.Left(path1.ReverseFind('\\')) + "\\Boot\\Boot\\res\\images\\charger\\";
    path2 =  path2.Left(path2.ReverseFind('\\')) + "\\Boot\\ramdisk\\res\\images\\charger\\";
    if (PathFileExists(path1))
    {
        return path1;
    } else if (PathFileExists(path2))
    {
        return path2;
    }
    return str;
}

void CChargeImage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::EndDialog(0);
}

void CChargeImage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	 delete(this);
}

void CChargeImage::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	int i=0,j=0;
	CString strTxt;
	CString BatteryImage[8];
	CString strDescFilePath;

	OnInitUI();

	strDescFilePath=strRootPath+_T("\\dts\\charge_anim_desc.txt");

	if(FileExist(strDescFilePath)){
		BatteryImage[0]=strUbootChargeImagePath+"\\battery_0.bmp";
		BatteryImage[1]=strUbootChargeImagePath+"\\battery_1.bmp";
		BatteryImage[2]=strUbootChargeImagePath+"\\battery_2.bmp";
		BatteryImage[3]=strUbootChargeImagePath+"\\battery_3.bmp";
		BatteryImage[4]=strUbootChargeImagePath+"\\battery_4.bmp";
		BatteryImage[5]=strUbootChargeImagePath+"\\battery_5.bmp";
		BatteryImage[6]=strUbootChargeImagePath+"\\battery_fail.bmp";
			
		for(i=0;i<7;i++)
		{
			ButtonDisplayPictrue(BatteryImage[i],i);
		}
	}
	else{	
		for(i=0;i<7;i++){
			((CButton *)GetDlgItem(IDC_BUTTON20+i))->EnableWindow(FALSE);
			ButtonDisplayPictrue(strErrorImagePath,i);
		}
		MessageBoxW(_T("No exist charge file!"));
	}


}

void CChargeImage::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	int i=0;
	CString BatteryImage[8];

	OnInitUI();

	if(strAndroidVersion==ANDROID_VERTION_FiVE_ZERO){
		BatteryImage[0]=strRecoveryChargeImagePath+"\\battery_scale.bmp";
		BatteryImage[1]=strRecoveryChargeImagePath+"\\battery_fail.bmp";

		for(i=0;i<2;i++)
		{
			ButtonDisplayPictrue(BatteryImage[i],i);
		}

	}else if (strAndroidVersion==ANDROID_VERTION_SIX_ZERO){
		BatteryImage[0]=strRecoveryChargeImagePath+"\\battery_scale.bmp";
		BatteryImage[1]=strRecoveryChargeImagePath+"\\battery_fail.bmp";
		
		for(i=0;i<2;i++)
		{
			ButtonDisplayPictrue(BatteryImage[i],i);
		}
	}
	else{
		BatteryImage[0]=strRecoveryChargeImagePath+"\\battery_0.bmp";
		BatteryImage[1]=strRecoveryChargeImagePath+"\\battery_1.bmp";
		BatteryImage[2]=strRecoveryChargeImagePath+"\\battery_2.bmp";
		BatteryImage[3]=strRecoveryChargeImagePath+"\\battery_3.bmp";
		BatteryImage[4]=strRecoveryChargeImagePath+"\\battery_4.bmp";
		BatteryImage[5]=strRecoveryChargeImagePath+"\\battery_5.bmp";
		BatteryImage[6]=strRecoveryChargeImagePath+"\\battery_charge.bmp";
		BatteryImage[7]=strRecoveryChargeImagePath+"\\battery_fail.bmp";
		
		
		for(i=0;i<8;i++)
		{
			ButtonDisplayPictrue(BatteryImage[i],i);
		}
	}

}
