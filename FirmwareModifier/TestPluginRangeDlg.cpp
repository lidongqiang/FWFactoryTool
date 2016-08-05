// TestPluginRangeDlg.cpp : implementation file
//

#include "stdafx.h"
/*#include <vld.h>*/
#include <stdlib.h>
#include "FirmwareModifier.h"
#include "TestPluginRangeDlg.h"
#include "KernelLogoTool.h"


#include "unzip.h"
#include "ximage.h"

#pragma comment(lib, "png.lib")
#pragma comment(lib, "libdcr.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "tiff.lib")
#pragma comment(lib, "jasper.lib")
#pragma comment(lib, "cximage.lib")
#pragma comment(lib, "mng.lib")


#pragma comment( lib, "shlwapi.lib")

#define FirmwareVersion _T("5.3-20160117")
/////////////////////////////////////////////////////////////////////////////////
/*
	1 去除关机动画

	2 修改开机logo的转换

	3 必须关闭隐藏文件后缀名，否则会出现问题

    4 修改加载完成之后才点亮clone按钮

    5 解决内存泄漏问题

	6 解决机型支持中文问题

	7 解决parameter中MACHINE_MODEL修改错误的问题

	8 使用android4.4的打包工具

	9 增加电池参数导入能

    10 增加dts编辑功能

	11 修复增加电池脚本斜杆导致的图片增加失败

	12 增加对6.0的支持

	13 修复对5.0 6.0 预安装支持
	
	14 修复修改内核配置开辟空间不够导致的闪退

    15

 */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// If you add new platform, remember to add it in CommonDefine.h
TCHAR* g_strPlatforms[] = {_T("rk29"), _T("rk2928"), _T("rk2926"), _T("rk30")};

cmLog *g_pPluginLogObject=NULL;
CWinThread *pWinThread=NULL;
TCHAR *pPackParam=NULL;
/////////////////////////////////////////////////////////////////////////////
// CTestPluginRangeDlg dialog
#define  BAT_DEFINE_VALUE	        2500
#define adc_to_voltage(adc_val)    (adc_val * BAT_DEFINE_VALUE)/1024 
#define voltage_to_adc(voltage)    (voltage * 1024)/BAT_DEFINE_VALUE 

#define MAX_CMDLINE 8096

#define BOOT_ANIMATION_TIMER 1 
#define SHUTDOWN_ANIMATION_TIMER 2
#define CLONE_PROGRESS_TIMER 3 
 
struct AnroidAnimationName
{                 
    TCHAR name[MAX_PATH];      // AnroidAnimation within the zip
};
//struct AnroidAnimationName *pAnimationPaths;
struct PlayParameter
{
    
    char PlayChar;
    int PalyCount;
    int DelayTime;
    TCHAR DirName[MAX_PATH];
};
struct AnimationParameter
{
    
    int ScreenWidth;
    int ScreenHeight;
    int FramesPersecond;
    PlayParameter playparam[10];
};
struct AnimationParameter AndroidAnimation;

enum ENUM_SYSTEM_CONFIG_MODE{
SYSTEM_CONFIG_COM = 0,

SYSTEM_CONFIG_CPU=10,
SYSTEM_CONFIG_GPU,
SYSTEM_CONFIG_DDR, 

SYSTEM_CONFIG_PMIC_TYPE=20,
SYSTEM_CONFIG_PMIC_DCDC,
SYSTEM_CONFIG_PMIC_LDO,
SYSTEM_CONFIG_PMIC_DCDC_TPS65910,
SYSTEM_CONFIG_PMIC_LDO_TPS65910,
SYSTEM_CONFIG_PMIC_DCDC_ACT8931,
SYSTEM_CONFIG_PMIC_LDO_ACT8931,

SYSTEM_CONFIG_LCD_RK2928=30,

SYSTEM_CONFIG_GPIO_NORMAL=40,
SYSTEM_CONFIG_GPIO_IRQ,

SYSTEM_CONFIG_ADC=50,
SYSTEM_CONFIG_ADC_CHANNEL,

SYSTEM_CONFIG_TP_COM=60,
SYSTEM_CONFIG_TP_TYPE,
SYSTEM_CONFIG_TP_GPIO_NORMAL,
SYSTEM_CONFIG_TP_GPIO_IRQ,

SYSTEM_CONFIG_GSENSOR=70,
SYSTEM_CONFIG_LSENSOR,
SYSTEM_CONFIG_PSENSOR,

SYSTEM_CONFIG_RUNIMAGE=80,
SYSTEM_CONFIG_RUNTXT,
};

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

BOOL volatile g_bHasUnpacked = FALSE;
BOOL volatile g_bIsCloning = FALSE;
BOOL volatile gThreadIsExist= FALSE;

enum ENUM_CLONE_STAT {
    CLONE_IDLE = 0,
    CLONE_EXECUTE_SCRIPT,
    CLONE_PULL_IMAGE,
}; 
int g_emCloneState = CLONE_IDLE;
CString g_strLanguageConfigFilePath;
CStringList content_list;
CStringList parameter_list;
CStringList volume_list;
CStringList initboadrc_list;
CStringList comline_list;
CStringList defaultprop_list;
CStringList mPackageFileList;
KEY_WORD key_word[MAX_LANGUAGE];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_INDEX_ENTRY_PATH_LEN    256
#define BLOCK_SIZE                  512


struct resource_ptn_header{
	BYTE     magic[4];//tag, "RSCE"
	WORD resource_ptn_version;
	WORD index_tbl_version;
	BYTE  header_size;//blocks, size of ptn header.
	BYTE  tbl_offset;//blocks, offset of index table.
	BYTE  tbl_entry_size;//blocks, size of index table's entry.
	UINT tbl_entry_num;//numbers of index table's entry.
} ;

resource_ptn_header header;

struct index_tbl_entry {
	BYTE     tag[4];//tag, "ENTR"
	BYTE     path[MAX_INDEX_ENTRY_PATH_LEN];
	UINT content_offset;//blocks, offset of resource content.
	UINT content_size;//bytes, size of resource content.
	UINT sizeInfo_offset;
} ;

#define MISC_COMMAND_PAGE   1
#define PAGE_SIZE           (16 * 1024)//16K
#define MISC_COMMAND_OFFSET (MISC_COMMAND_PAGE * PAGE_SIZE)//32

struct bootloader_message {
    char command[32];
    char status[32];
    char recovery[1024];
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern ULONG CRC_32(PBYTE pData, ULONG ulSize);
extern BOOL ParseLogo(CString strLogoFile,struct color* &pLogoData,int &logoWidth,int &logoHeight);
extern BOOL ImportLogo(BYTE *pKernel,int logoIndex,struct color *pLogo,int logoWidth,int logoHeight);
extern BOOL SaveKernel(BYTE *pKernel,UINT uiKernelSize,CString strKernelPath);;
//extern UINT SearchLogo(BYTE *pKernel,CString kernelLogopath);
extern BOOL ExportLogo(BYTE *pKernel,int logoIndex);
extern BOOL CheckKernel(BYTE *pKernel);
extern int resource_importLogo(CString ResourcePath,CString LogoPath);
extern int resource_importDtb(CString ResourcePath,CString dtbPath);
extern int resource_importDts_Dtb(CString ResourcePath,CString dtbPath);
extern int CovertNormalBmp(CString srcPath,CString desPath);
extern int kernel_export_logo(CString KerenlPath,CString FilePath,int ChipType);
extern int resource_export_uboot_logo(CString ResourcePath,CString FilePath);
extern int resource_export_kernel_logo(CString ResourcePath,CString FilePath);
extern int resource_export_dtb(CString ResourcePath,CString FilePath);
extern int convert_nocompression_logo(CString srcLogo,CString desLogo);

extern int JudgeKernelLogoType(CString KerenlPath);
extern BOOL FolderExist(CString strPath);
extern BOOL CreateFolder(CString strPath);
extern BOOL DeleteFolder(CString str);
extern void DeleteFolderFile(CString str);
extern int GetKernelLogoSize(CString KerenlPath,WORD *width,WORD *height,WORD *maxwidth,WORD *maxheight,int index);
extern int GetResourceLogoSize(CString ResourcePath,WORD *width,WORD *height);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ULONGLONG GetFileSize(LPCTSTR szFileName)   
{   
    CFileStatus fs;   
    if (!CFile::GetStatus(szFileName,fs)) return 0;   
    
    return fs.m_size;   
}

CTestPluginRangeDlg::CTestPluginRangeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTestPluginRangeDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTestPluginRangeDlg)
	//}}AFX_DATA_INIT
}


void CTestPluginRangeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTestPluginRangeDlg)
	DDX_Control(pDX, IDC_PROGRESS_CLONE, m_progressClone);
    DDX_Control(pDX, IDC_LIST_SYS_APKS, m_sysApksList);
    DDX_Control(pDX, IDC_LIST_PREINST_APKS, m_preInstApksList);
    DDX_Control(pDX, IDC_TAB_APKS, m_tabApks);
    DDX_Control(pDX, IDC_TAB_LOGO, m_tabLogo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestPluginRangeDlg, CDialog)
//{{AFX_MSG_MAP(CTestPluginRangeDlg)
ON_MESSAGE(WM_UNPACK_FINISH,OnUnPackFinishMsg)
ON_MESSAGE(WM_PACK_FINISH,OnPackFinishMsg)
ON_WM_TIMER()
ON_WM_PAINT()
ON_BN_CLICKED(BTN_REPLACE_BOOT_LOGO, OnReplaceBootLogo)
ON_BN_CLICKED(BTN_REPLACE_BOOT_ANIMATION, OnReplaceBootAnimation)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOGO, OnSelchangeTabLogo)
ON_BN_CLICKED(BTN_REPLACE_DEFAULT_WALLPAPER, OnReplaceDefaultWallpaper)
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_APK, OnSelchangeTabApk)
ON_NOTIFY(NM_RCLICK, IDC_LIST_PREINST_APKS, OnRclickListPreinstApks)
ON_NOTIFY(NM_RCLICK, IDC_LIST_SYS_APKS, OnRclickListSysApks)
	ON_BN_CLICKED(IDC_BTN_CONFIRM_MODIFY, OnBtnConfirmModify)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MODIFY_BUILD_PROP, OnBtnModifyBuildProp)
	ON_BN_CLICKED(IDC_BTN_MODIFY_KEY_MAP, OnBtnModifyKeyMap)
	ON_BN_CLICKED(BTN_REPLACE_CHARGE_LOGO, OnReplaceChargeLogo)
	ON_BN_CLICKED(IDC_BTN_MODIFY_BATTERY_PARAM, OnBtnModifyBatteryParam)
	ON_BN_CLICKED(IDC_BTN_CLONE, OnBtnClone)
	ON_BN_CLICKED(BTN_REPLACE_BOOT_RING, OnReplaceBootRing)
	ON_BN_CLICKED(IDC_BTN_MODIFY_KERNEL_PARAMETER, OnBtnModifyKernelParameter)
	ON_WM_DESTROY()
ON_MESSAGE(WM_UPDATE_CLONE_BTN,OnUpdateCloneBtnMsg)
	ON_BN_CLICKED(BTN_REPLACE_KERNEL_LOGO, OnReplaceKernelLogo)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_BT2, OnCheckEnableUserFormat)
	ON_BN_CLICKED(BTN_REPLACE_SHUTDOWN_RING, OnReplaceShutdownRing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestPluginRangeDlg message handlers


BOOL RunOhterExe(CString strCmd,CString *strResult,ENUM_STRING_CODE emCode)
{
	CMySpawnConsumer spawnConsumer;
	cmSpawn spawn;
	CString strText;
	tstring strInfo;
	strText = _T("***")+strCmd+_T("***");
	g_pPluginLogObject->Record(strText);
	BOOL bBat=FALSE;
	strCmd.MakeLower();
	if (strCmd.Find(_T(".bat"))!=-1)
	{
		bBat = TRUE;
	}
	
	try
	{
		spawn.Execute(strCmd);
	}
	catch (LPCTSTR pszError)
	{		
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:RunOhterExe-->Execute failed,Cmd(%s),Error(%s)"),strCmd,pszError);
		}
		return FALSE;
	}
	catch (...)
	{
		
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("ERROR:RunOhterExe-->Execute failed,Cmd(%s)"),strCmd);
		}
		return FALSE;
	}
	//4.等待执行线程结束
	while(TRUE)
	{
		if (!spawn.TestThread())
		{
			break;
		}
		Sleep(200);
	}

	spawnConsumer.SetSpawnObject(&spawn);
	spawnConsumer.GetSpawnString(strInfo,emCode);
	g_pPluginLogObject->Write(strInfo.c_str());
	strResult->Format(_T("%s"),strInfo.c_str());

	if (bBat)
	{//获取返回结果
		if (!spawnConsumer.GetBatCmdResult(strInfo))
		{
			if (g_pPluginLogObject)
			{
				g_pPluginLogObject->Record(_T("ERROR:RunOhterExe-->GetBatCmdResult failed"));
			}
			return FALSE;
		}
	}
	
	return TRUE;

}

/* reads a file, making sure it is terminated with \n \0 */
char *read_file(CString fn, unsigned *_sz)
{
    char *data;
    int sz;
    int len;
    FILE *fd;
    
    data = 0;
    fd = _tfopen(fn, _T("rb"));
    if(fd == NULL) return 0;
    
    sz = fseek(fd, 0, SEEK_END);
    sz = ftell(fd);
    if(sz < 0) goto oops;
    
    if(fseek(fd, 0, SEEK_SET) != 0) goto oops;
    
    data = (char*) malloc(sz + 2);
    if(data == 0) goto oops;
    
    len = fread(data,sizeof(char), sz,fd);
    if(len != sz) goto oops;
    fclose(fd);
	data[sz] = 0;
    //data[sz] = '\n';
    //data[sz+1] = 0;
    if(_sz) *_sz = sz;
    return data;
    
oops:
    fclose(fd);
    if(data != 0) free(data);
    return 0;
}

BOOL FileExist(CString strFileName)
{
    CFileFind fFind;
    return fFind.FindFile(strFileName); 
}

UINT ModifyChangesThread( LPVOID pParam ) 
{

	CString str_name;
    DWORD current_size = 0;
    DWORD nPartitionSize = 0;
    CTestPluginRangeDlg* pDlg = (CTestPluginRangeDlg*) pParam;

	pDlg->GetDlgItemText(IDC_SYS_EDIT6,str_name);
	if(str_name.GetLength()>11)
	{
		CString strIDName;
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_DISK_VOLUME_ILLEGAL"),_T("ERROR_DISK_VOLUME_ILLEGAL"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(pDlg->m_hWnd, strIDName,pDlg->m_strMsgBoxTitle, MB_ICONERROR);
		strIDName.ReleaseBuffer(MAX_LENGTH);
		return 1;
	}
    
	pDlg->GetDlgItemText(IDC_SYS_EDIT5,str_name);
	if(str_name.GetLength()>29)
	{
		CString strIDName;
		GetPrivateProfileStringW(_T("SystemSeting"),_T("USB_FACTORY_ILLEGAL"),_T("USB_FACTORY_ILLEGAL"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(pDlg->m_hWnd, strIDName,pDlg->m_strMsgBoxTitle, MB_ICONERROR);
		strIDName.ReleaseBuffer(MAX_LENGTH);
		//MessageBox(_T("Usb factory too long "));
		return 1;
	}
	
 	current_size = pDlg->GetDirSize(pDlg->strSystemPath);

 	if(current_size > pDlg->GetPartitionSizeInBytes(_T("system")))
 	{
 		nPartitionSize = pDlg->GetProperPartitionSectorSize(current_size);
		if (nPartitionSize > 0)
		{
			pDlg->ModifyPartitionSize(_T("system"), nPartitionSize);
			if (!pDlg->ReplaceCStringListByKeywords(comline_list, _T("mtdparts"), pDlg->GeneratePartitionsInfo())) {
				CString strIDName;
				GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_REPLACE_CSTRINGLIST_BY_KEYWRODS"),_T("ReplaceCStringListByKeywords failed!"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
				MessageBox(pDlg->m_hWnd,strIDName,pDlg->m_strMsgBoxTitle, MB_ICONERROR);
				strIDName.ReleaseBuffer(MAX_LENGTH);
				return 1;
			}
			if (!pDlg->FlushPartitionsToParameterList()) {
				CString strIDName;
				GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_FLUSH_PARTITION_TO_PARAMETERLIST"),_T("FlushPartitionsToParameterList failed!"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
				MessageBox(pDlg->m_hWnd,strIDName, pDlg->m_strMsgBoxTitle, MB_ICONERROR);
				strIDName.ReleaseBuffer(MAX_LENGTH);
				return 1;
			}
		}
	}
	
	////////////////////////////////////////////////////////////////////////////
	// modify buildprop
	//      
	pDlg->WriteSystemConfigToBuildProp();
	////////////////////////////////////////////////////////////////////////////
	// modify defaultprop
	//
	pDlg->WriteSystemConfigToDefaultProp();
	
	pDlg->ModifyPackageFile();
	
	///////////////////////////////////////////////////////////////////////////
	//modify parameter
	if (!pDlg->ModifyPartitionsParameter()) {
		CString strIDName;
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_MODIFY_PARTITIONS_PARAMETER"),_T("ModifyPartitionsParameter failed!"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(pDlg->m_hWnd,  strIDName, pDlg->m_strMsgBoxTitle, MB_ICONERROR);
		strIDName.ReleaseBuffer(MAX_LENGTH);
		return 1;
	}


	pDlg->WriteSystemConfigToParameter();
	
	//////////////////////////////////////////////////////////////////////////
	// modify recoveryinit.rc
	pDlg->WriteSystemConfigToRecoveryInitRc();

	//////////////////////////////////////////////////////////////////////////
	// modify bootinit.rc
	pDlg->WriteSystemConfigToBootInitRc();
	
	//////////////////////////////////////////////////////////////////////////
	// modify misc
	pDlg->WriteMiscCommand();



	pDlg->DisableUIControls();
	pDlg->PartitionListFree();

	int iLen = pDlg->m_strNewImagePath.GetLength(); 
	pPackParam = new TCHAR[100]; 
	memset(pPackParam,0,2*100);
	lstrcpy(pPackParam, pDlg->m_strNewImagePath.GetBuffer(iLen)); 
	
    ::SendMessage((HWND)g_msgHwnd,WM_START_PACK,0,(LPARAM)pPackParam);
	pDlg->m_strNewImagePath.ReleaseBuffer(); 
	delete pPackParam;
	return 0;
}

UINT AdbDeviceDetectThread( LPVOID pParam )
{   
	int i=0;
	gThreadIsExist = TRUE;
 	CTestPluginRangeDlg* pDlg = (CTestPluginRangeDlg*) pParam;
	if(!pDlg->m_AdbOperator.InitAdbApi())
	{
		return 0;
	}

    do 
    {
        i = pDlg->m_AdbOperator.ScanAdbDevice();
        if (i == 1) {
                pDlg->PostMessage(WM_UPDATE_CLONE_BTN, 1, 0);          
        } else if (i > 1) {
            pDlg->PostMessage(WM_UPDATE_CLONE_BTN, 0, 0);
        } else {
            pDlg->PostMessage(WM_UPDATE_CLONE_BTN, 0, 0);
        }
        Sleep(1000);
    } while (gThreadIsExist);
    
    g_pPluginLogObject->Record(_T("Exit AdbDeviceDetectThread\n"));
    return 0;
}

UINT SystemStatusCloneThread( LPVOID pParam ) {
    g_bIsCloning = TRUE;
	int ret=0;
    CString strTex;
    CString strDatabkImagePath;
    		
    CTestPluginRangeDlg* pDlg = (CTestPluginRangeDlg*) pParam;
    strDatabkImagePath = pDlg->m_strModulePath;
    strDatabkImagePath += _T("Temp\\Android\\Image\\databk.img");
    pDlg->mDatabkImageSize = 0;
    
    SetTimer(pDlg->m_hWnd, CLONE_PROGRESS_TIMER, 1000, NULL);
    g_emCloneState = CLONE_EXECUTE_SCRIPT;
	ret=pDlg->m_AdbOperator.ExecPackCmd(pDlg->GetPartitionSizeInBytes(_T("userdata")));
    if(ret>=0) {
        pDlg->m_progressClone.SetPos(50);

        Sleep(500);
        g_emCloneState = CLONE_PULL_IMAGE;
        if (pDlg->m_AdbOperator.PullImage(strDatabkImagePath)) {
            pDlg->m_progressClone.SetPos(100);
            GetPrivateProfileStringW(_T("Prompt"),_T("INFO_CLONE_OK"),_T("INFO_CLONE_OK"),strTex.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			MessageBox(pDlg->m_hWnd, strTex, pDlg->m_strMsgBoxTitle, MB_OK);
            pDlg->mDatabkImageSize = GetFileSize(strDatabkImagePath);
		
        } else {
            g_emCloneState = CLONE_IDLE;
            GetPrivateProfileStringW(_T("Prompt"),_T("ERROR_PULL_DATABK_IMAGE"),_T("ERROR_PULL_DATABK_IMAGE"),strTex.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            g_pPluginLogObject->Record(strTex);
            MessageBox(pDlg->m_hWnd, strTex, pDlg->m_strMsgBoxTitle, MB_OK | MB_ICONERROR);
        }
    } else {
        g_emCloneState = CLONE_IDLE;
		if(ret==-1){
			GetPrivateProfileStringW(_T("Prompt"),_T("ERROR_PACK_DATA_PARTITION"),_T("ERROR_PACK_DATA_PARTITION"),strTex.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			g_pPluginLogObject->Record(strTex);
			MessageBox(pDlg->m_hWnd, strTex, pDlg->m_strMsgBoxTitle, MB_OK | MB_ICONERROR);
		}else{
			GetPrivateProfileStringW(_T("Prompt"),_T("ERROR_RECONNECT_USB"),_T("ERROR_RECONNECT_USB"),strTex.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			g_pPluginLogObject->Record(strTex);
			MessageBox(pDlg->m_hWnd, strTex, pDlg->m_strMsgBoxTitle, MB_OK | MB_ICONERROR);
		}
    }
    
    KillTimer(pDlg->m_hWnd, CLONE_PROGRESS_TIMER);
    g_bIsCloning = FALSE;
    pDlg->GetDlgItem(IDC_BTN_CLONE)->EnableWindow(TRUE);
    pDlg->m_progressClone.SetPos(0);
    strTex.ReleaseBuffer(MAX_LENGTH);
    return 0;
}

BOOL CTestPluginRangeDlg::UpdateFstab()
{
   CFile mFstabfile;
   int resultBoot=0;
   int resultRam=0;
   DWORD uiFstabSize;
   POSITION pos;
   resultBoot=mFstabfile.Open(strFsBootTabPath,CFile::modeReadWrite|CFile::typeBinary);
   if(resultBoot<=0){
	   g_pPluginLogObject->Record(_T("%s isn't Exsit"),strFsBootTabPath);

	   resultRam=mFstabfile.Open(strFsRamTabPath,CFile::modeReadWrite|CFile::typeBinary);
	   if(resultRam<=0){		
		   g_pPluginLogObject->Record(_T("%s isn't Exsit"),strFsRamTabPath);
		   return FALSE;
		} 
   }

   uiFstabSize = mFstabfile.GetLength();
   char *pFstabBuffer=NULL;
   char *pchar=NULL;
   UINT mStart,count=0;
   char buf[2];
   char strbuf[]="internal_sd";
   char strbuf1[]=":";
   CString strPartitionName="(user)";
   pFstabBuffer = new char[uiFstabSize];

   mFstabfile.Read(pFstabBuffer,uiFstabSize);

   pchar=strstr(pFstabBuffer,strbuf);
   if(pchar==NULL){
	   g_pPluginLogObject->Record(_T("can not find %s"),strbuf);	   
	   return FALSE;		
   }

    mStart=strcspn(pchar,strbuf1);

    for (pos = mPartitionList.GetHeadPosition(); pos; )
    {
        CPartitionInfo *info = (CPartitionInfo*)mPartitionList.GetNext(pos);
        if (!info->strPartitionName.CompareNoCase(strPartitionName))
        {
            break;
        }
		count++;
    }

	itoa(count,buf,10);

	pchar[mStart+1]=buf[0];
	pchar[mStart+2]=buf[1];

	mFstabfile.SeekToBegin();
	mFstabfile.Write(pFstabBuffer,uiFstabSize);

	mFstabfile.Close();

	if(pFstabBuffer!=NULL)
		delete[] pFstabBuffer;

	return TRUE;
}

int CTestPluginRangeDlg::ReadRecoverInitRcFile()
{
	char *pBuffer = NULL;
    char *x = NULL;
    char content_buf[1024*8];
    char loop = 1;
    int  content_len = 0;
	CString str, strTxt;	
    LPWSTR lpDir = NULL;
    int nSize = 0;

	volume_list.RemoveAll();

	memset(content_buf,0,sizeof(content_buf));
    pBuffer = read_file(strRecoverInitRcPath, 0);
    content_len = 0;
    loop = 1;
    if (!pBuffer) 
    {      
        return -1;
    }
    else
    {
        x = pBuffer;
        while(loop)
        {
            switch(*x)
            {
                
            case '\n':
                x++;
                content_buf[content_len] = 0x00;
                
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                
                volume_list.AddTail(lpDir);
				if (lpDir)
				{
					delete[] lpDir;
					lpDir =	 NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            //case '\t':
            case '\r':
                x++;
                continue;
            case  0:                
                loop = 0;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                volume_list.AddTail(lpDir);
				if (lpDir)
				{
					delete[] lpDir;
					lpDir =	 NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            default: content_buf[content_len] = *x++;
                content_len++;
                break;
            }
        }
    }	
    if(pBuffer)
		free(pBuffer);
	return 0;
}

int CTestPluginRangeDlg::ReadBootInitRcFile()
{
	char *pBuffer = NULL;
    char *x = NULL;
    char content_buf[1024*8];
    char loop = 1;
    int  content_len = 0;
	CString str, strTxt;	
    LPWSTR lpDir = NULL;
    int nSize =	0;

	initboadrc_list.RemoveAll();

	memset(content_buf,0,sizeof(content_buf));
    pBuffer = read_file(strBootInitBoardRcPath, 0);
    content_len = 0;
    loop = 1;
    if (!pBuffer) 
    {      
        return -1;
    }
    else
    {
        x = pBuffer;
        while(loop)
        {
            switch(*x)
            {
                
            case '\n':
                x++;
                content_buf[content_len] = 0x00;
                
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                
                initboadrc_list.AddTail(lpDir);
				if (lpDir)
				{
					delete[] lpDir;
					lpDir =	 NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            //case '\t':
            case '\r':
                x++;
                continue;
            case  0:                
                loop = 0;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                initboadrc_list.AddTail(lpDir);
				if (lpDir)
				{
					delete[] lpDir;
					lpDir =	 NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            default: content_buf[content_len] = *x++;
                content_len++;
                break;
            }
        }
    }	
    if(pBuffer)
		free(pBuffer);
	return 0;
}
int CTestPluginRangeDlg::ReadBuildpropFile()
{
	char *pBuffer;
    char *x;
    char content_buf[MAX_CMDLINE];
    char loop = 1;
    int  content_len = 0;
	CString str, strTxt;	
    LPWSTR lpDir = NULL;
    int nSize;
	content_list.RemoveAll();

    memset(content_buf,0,sizeof(content_buf));
    pBuffer = read_file(strBuildPropPath, 0);
    if (!pBuffer) {      
        return -1;
    }
    else{
        ((CButton*)GetDlgItem(IDC_BTN_CONFIRM_MODIFY))->EnableWindow(TRUE); 
        
        x = pBuffer;
        while(loop)
        {
            switch(*x)
            {
            case '\n': 
                x++; 
                content_len++;
                content_buf[content_len] = 0;
                content_len++;
                content_buf[content_len] = 0;
                nSize = strlen(content_buf);
                //AnsiToUnicode(lpDir,nSize,content_buf);
				U8ToUnicode(lpDir,nSize,content_buf);
				strTxt = lpDir;
                content_list.AddTail(strTxt);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            case  0:                
                loop = 0;
                nSize = strlen(content_buf);
                //AnsiToUnicode(lpDir,nSize,content_buf);
				U8ToUnicode(lpDir,nSize,content_buf);
				strTxt = lpDir;
                content_list.AddTail(strTxt);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            default: content_buf[content_len] = *x++;
                content_len++;
                break;
            }
        }
    }
    if(pBuffer)
		free(pBuffer);
	return 0;

}


int CTestPluginRangeDlg::ReadParameterFile()
{
	char *pBuffer = NULL;
    char *x = NULL;
    char content_buf[MAX_CMDLINE];
    char loop = 1;
    int  content_len = 0;
	CString str, strTxt;	
    LPWSTR lpDir = NULL;
    int nSize = 0;
	int n,mStart=10,mEnd=0;
	int count=1000;
    POSITION pos;
    CString str_find;
	char txt1=(char)0x09;
	char txt2=(char)0x02;
	CString strtemp;	

	CString str1=txt1;
	CString str2=txt2;

	parameter_list.RemoveAll();
	mPartitionList.RemoveAll();

	for(pos=mPackageFileList.GetHeadPosition();pos;)
    {
        str_find = mPackageFileList.GetNext(pos);
        n = str_find.Find(_T("parameter"));
		g_pPluginLogObject->Record(_T("file:")+str_find);
        if (n >= 0)
        {
			while(count--){
				strTxt=str_find.Mid(mStart,1);
				if((strTxt.CompareNoCase(str1)==0)||((strTxt.CompareNoCase(str2)==0)))
					continue;
				else
					break;
				mStart++;
			}

			if(count<=0){
				g_pPluginLogObject->Record(_T("in packagefile parameter error"));
				return -1;
			}

			mEnd=str_find.GetLength();

			strtemp=str_find.Right(mEnd-mStart);
			strTxt=strtemp.Mid(0,strtemp.GetLength()-2);
			strTxt.TrimLeft();
			strTxt.TrimRight();
        }
    }
	if(count>=1000){
		g_pPluginLogObject->Record(_T("No Find parameter in packagefile"));	
		return -1;
	}

	strParameterPath.Format(_T("%s%s%s"),strTempPath,_T("\\Android\\"),strTxt);

    memset(content_buf,0,sizeof(content_buf));
    pBuffer = read_file(strParameterPath, 0);
    content_len = 0;
    loop = 1;
    if (!pBuffer){
		g_pPluginLogObject->Record(_T("open parameter failed:")+strParameterPath);
        return -1;
    }
    else
    {		
        x = pBuffer;
        while(loop)
        {
            switch(*x)
            {
            case 0x0D:
                content_buf[content_len] = 0x0D;
                content_len++;					
                x++;
                if(*x == 0x0a) x++;					
                content_buf[content_len] = 0x0A;
                content_len++;
                content_buf[content_len] = 0x00;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                parameter_list.AddTail(lpDir);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            case  0:                
                loop = 0;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                parameter_list.AddTail(lpDir);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            default:
				content_buf[content_len] = *x++;
                content_len++;
                break;
            }
        }
    }	
    if(pBuffer)
		free(pBuffer);
	return 0;
}

int CTestPluginRangeDlg::ReadDefaultProp()
{
	char *pBuffer = NULL;
    char *x = NULL;
    char content_buf[1024];
    char loop = 1;
	LPWSTR lpDir = NULL;
    int nSize,content_len = 0;
	CString strTxt;
    CString str_find,str_name,str_language,str_local;
	
	defaultprop_list.RemoveAll();
	
    memset(content_buf,0,sizeof(content_buf));
    pBuffer = read_file(strDefaultPropPath, 0);
    if (!pBuffer) {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_BUILD_PROP"),_T("Read Build.prop failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
		if(pBuffer)
			free(pBuffer);	
        return -1;
    }
    else{
        ((CButton*)GetDlgItem(IDC_BTN_CONFIRM_MODIFY))->EnableWindow(TRUE); 
        
        x = pBuffer;
        while(loop)
        {
            switch(*x)
            {
            case '\n': 
                x++; 
                content_len++;
                content_buf[content_len] = 0;
                content_len++;
                content_buf[content_len] = 0;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                defaultprop_list.AddTail(lpDir);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            case  0:                
                loop = 0;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                defaultprop_list.AddTail(lpDir);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            default: content_buf[content_len] = *x++;
                content_len++;
                break;
            }
        }
    }
    if(pBuffer)
		free(pBuffer);	
	return 0;
}
void CTestPluginRangeDlg::GetComlineParameter()
{
	POSITION pos;
    CString str_find;
    CString str_name;
    CString str_language;
    CString str_local;
	CString strTemp;
    int n;
	comline_list.RemoveAll();

	for(pos=parameter_list.GetHeadPosition();pos;)
    {
        str_find = parameter_list.GetNext(pos);
        n = str_find.Find(_T("CMDLINE:"));
        if(n>=0) 
        { 
            str_find.Delete(0,8);
			do
			{
				//MessageBox(str_find);
				n=str_find.Find(_T(" "));
				if(n>=0)
				{
					strTemp=str_find.Mid(0,n+1);
					comline_list.AddTail(strTemp);
					str_find.Delete(0,n+1);
				}
				else
				{
					comline_list.AddTail(str_find);
					break;
				}
			
			}
			while(1);
        }
    }
    
}

void CTestPluginRangeDlg::GetRecoveryInitRcParameter()
{
	int n;
	POSITION pos;
    CString str_find;
    CString str_name;
   
	for(pos=volume_list.GetHeadPosition();pos;)
    {
        str_find = volume_list.GetNext(pos);
        n = str_find.Find(_T("setprop UserVolumeLabel"));
        if(n>=0) 
        {
            n = str_find.Find(_T('"'));
            str_name = str_find.Right(str_find.GetLength() - n - 1);
            n = str_name.Find(_T('"'));
            str_name = str_name.Left(n);
            SetDlgItemText(IDC_SYS_EDIT6,str_name);
            ((CEdit*)GetDlgItem(IDC_SYS_EDIT6))->EnableWindow(TRUE); 
            
        }
    }
}


void CTestPluginRangeDlg::GetBuildPropParameter()
{
    POSITION pos;
    CString str_find;
    CString str_name;
    CString str_language;
    CString str_local;
    CString strTemp;
    int n;
    
    for(pos=content_list.GetHeadPosition();pos;)
    {
        str_find = content_list.GetNext(pos);
        n = str_find.Find(_T("ro.sf.lcd_density"));
        if(n>=0) 
        {
            ((CComboBox*)GetDlgItem(IDC_SYS_EDIT7))->EnableWindow(TRUE);
            n = str_find.Find(_T("="));
            str_name = str_find.Right(str_find.GetLength() - n - 1);
            ((CComboBox*)GetDlgItem(IDC_SYS_EDIT7))->SetWindowText(str_name);
        }else 
        {
            n = str_find.Find(_T("ro.build.display.id"));			
            if(n>=0)
            {
                n = str_find.Find(_T("="));
                str_name = str_find.Right(str_find.GetLength() - n - 1);
                SetDlgItemText(IDC_SYS_EDIT3,str_name);
                ((CEdit*)GetDlgItem(IDC_SYS_EDIT3))->EnableWindow(TRUE);
            }else 
            {
                n = str_find.Find(_T("ro.product.model"));
                if(n>=0)
                {
                    n = str_find.Find(_T("="));
                    str_name = str_find.Right(str_find.GetLength() - n - 1);
                    SetDlgItemText(IDC_SYS_EDIT2,str_name);
                    ((CEdit*)GetDlgItem(IDC_SYS_EDIT2))->EnableWindow(TRUE);
                }else
                {
                    n = str_find.Find(_T("ro.rksdk.version"));
                    if(n>=0)
                    {
                        n = str_find.Find(_T("="));
                        str_name = str_find.Right(str_find.GetLength() - n - 1);
                        SetDlgItemText(IDC_SYS_EDIT4,str_name);
                        ((CEdit*)GetDlgItem(IDC_SYS_EDIT4))->EnableWindow(TRUE);
                    }
                    else{
                        n = str_find.Find(_T("ro.product.brand"));
                        if(n>=0)
                        {
                            n = str_find.Find(_T("="));
                            str_name = str_find.Right(str_find.GetLength() - n - 1);
                            SetDlgItemText(IDC_SYS_EDIT1,str_name);
                            ((CEdit*)GetDlgItem(IDC_SYS_EDIT1))->EnableWindow(TRUE);
                        } else {
                            n = str_find.Find(_T(/*"ro.product.manufacturer"*/"ro.product.usbfactory"));
                            if(n>=0)
                            {
                                n = str_find.Find(_T("="));
                                str_name = str_find.Right(str_find.GetLength() - n - 1);
                                SetDlgItemText(IDC_SYS_EDIT5,str_name);
                                ((CEdit*)GetDlgItem(IDC_SYS_EDIT5))->EnableWindow(TRUE);
                            } else {
                                n = str_find.Find(_T("ro.rk.bt_enable"));
                                if(n>=0)
                                {
                                    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->EnableWindow(TRUE);
                                    n = str_find.Find(_T("="));
                                    str_name = str_find.Right(str_find.GetLength() - n - 1);
                                    if (!str_name.CompareNoCase(_T("true")) || !str_name.CompareNoCase(_T("yes"))
                                        || !str_name.CompareNoCase(_T("1"))) {
                                        ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->SetCheck(TRUE);
                                    } else {
                                        ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->SetCheck(FALSE);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
int CTestPluginRangeDlg::LoadFirmware()
{
    char loop = 1;
    int  content_len = 0;
    CString str, strTxt;


    if(FileExist(strBootRingtonePath)) {
        ((CEdit*)GetDlgItem(IDC_EDIT_BOOT_RING_PATH))->SetWindowText(strBootRingtonePath);
    }
	else
	{
		 ((CEdit*)GetDlgItem(IDC_EDIT_BOOT_RING_PATH))->SetWindowText(_T(""));
	}

	if(FileExist(strShutdownRingtonePath)) {
        ((CEdit*)GetDlgItem(IDC_EDIT_SHUTDOWN_RING_PATH))->SetWindowText(strShutdownRingtonePath);
    }
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_SHUTDOWN_RING_PATH))->SetWindowText(_T(""));
	}

	if(!FolderExist(strFactoryPath))
    {
        CreateFolder(strFactoryPath);
    }

	if(ReadBuildpropFile()<0)
	{
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_BUILD_PROP"),_T("Read Build.prop failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
		return -1;
	}
	GetBuildPropParameter();

	if(ReadPackageFile()<0)
    {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_PACKAGE"),_T("Read package-file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
        return -3;
	}


    if(ReadParameterFile()<0)
	{
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_PARAMETER"),_T("Read parameter failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
		return -2;
	}
	GetComlineParameter();

   
    if(ReadRecoverInitRcFile()<0)
	{
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_INIT_RC"),_T("Read init.rc failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
		return -4;
	}
	GetRecoveryInitRcParameter();
	////////////////////////////////////////////////////////////////////////////
    strBootInitBoardRcPath = DetermineBootInitRcPath();
	if(ReadBootInitRcFile()<0)
	{
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_BOOT_INIT_RC"),_T("Read init.rk2928board.rc failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
		return -5;
	}
	////////////////////////////////////////////////////////////////////////////
    if(ReadDefaultProp()<0)
	{
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_DEFAULT_PROP"),_T("Read default.prop failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
		return -6;
	}

	ParsePartitionsInfo(ExtractCmdlinePartitionsInfo());
	////////////////////////////////////////////////////////////////////////////
	//获取parameter的原始时间
	CFileStatus filestatus;
	CFile::GetStatus(strParameterPath,filestatus);
	strInitParameterTime=filestatus.m_mtime.Format("%Y-%m-%d %H:%M:%S");

    return 0;
}

int CTestPluginRangeDlg::LoadAnimation() 
{
	//Android??????
    needDisplayDefaultWallpaper=FALSE;
    strDefaultWallpaperPath=strTempPath+"\\System\\media\\rkfactory\\default_wallpaper.jpg";
    
    if(PathFileExists(strDefaultWallpaperPath))
    {
        needDisplayDefaultWallpaper=TRUE;
    }

	return 0;
}

int CTestPluginRangeDlg::LoadWallopaper() 
{
	int n = 0;
	CString strBootAnimationPath;
    CString strShutdownAnimationPath;

    needDisplayBootanimation =FALSE;
    isExistAnroidAnimationZip=FALSE;
    strBootAnimationPath=strTempPath+"\\System\\media\\bootanimation.zip";
    n = strBootAnimationPath.ReverseFind('\\');
    strBootAnimationBasePath=strBootAnimationPath.Left(n)+"\\bootanimation";
    ImportAnimation(strBootAnimationBasePath, BOOT_ANIMATION);
    
	return 0;
}

int CTestPluginRangeDlg::LoadResource() 
{
    int num=0;
    int ret=0;
    CString strTxt;
    CString FileName,tmp;


    if(!PathFileExists(strResourcePath))
    {
        GetPrivateProfileStringW(_T("Logo"),_T("ERROR_RESOURCE_PATH_NO_EXIST"),_T("ERROR_RESOURCE_PATH_NO_EXIST"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
        return -1;
        
    }

	CString strCmd;
	int iRet=0;
	CString strResult;
	CString g_strCovert = strRootPath +_T("\\bin\\unpackresource.bat");
	strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
	if (!RunOhterExe(strCmd,&strResult))
	{
		
		GetPrivateProfileStringW(_T("Logo"),_T("ERROR_UNPACK_RESOURCE"),_T("ERROR_UNPACK_RESOURCE"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		g_pPluginLogObject->Record(_T("resource unpack failed %s\n"),strResult);
		return -2;
	}

	isReadubootLogo=TRUE;
	isReadkernelLogo = TRUE;

	if(FileExist(strResourceKernelLogoPath)){
	//	((CButton*)GetDlgItem(BTN_REPLACE_KERNEL_LOGO))->ShowWindow(SW_SHOW);
 		((CButton*)GetDlgItem(BTN_REPLACE_KERNEL_LOGO))->EnableWindow(TRUE);
	}
/*
	if(GetResourceLogoSize(strResourcePath,&wKernelPictureWidth,&wKernelPictureHeight)<0)
	{
		GetPrivateProfileStringW(_T("Logo"),_T("ERROR_READ_WIDTH_HEIGHT_FAILED"),_T("ERROR_READ_WIDTH_HEIGHT_FAILED"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		return -2;
	}
    
	wKernelMaxPictureWidth=wKernelPictureWidth;
	wKernelMaxPictureHeight=wKernelPictureHeight;
	
	ret =resource_export_uboot_logo(strResourcePath,strResourceUbootLogoPath);
    if(ret<0){
		GetPrivateProfileStringW(_T("Logo"),_T("ERROR_EXPORT_RESOURCE_LOGO"),_T("ERROR_EXPORT_RESOURCE_LOGO"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		g_pPluginLogObject->Record(_T("resource export logo %d\n"),ret);
		return -3;
	}

    isReadubootLogo=TRUE;

	ret =resource_export_kernel_logo(strResourcePath,strResourceKernelLogoPath);
    if(ret<0){
		GetPrivateProfileStringW(_T("Logo"),_T("ERROR_EXPORT_RESOURCE_LOGO"),_T("ERROR_EXPORT_RESOURCE_LOGO"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		g_pPluginLogObject->Record(_T("resource export logo %d\n"),ret);
		return -4;
	}

	isReadkernelLogo = TRUE;

	ret =resource_export_dtb(strResourcePath,strResourceDtbPath);
    if(ret<0){
		GetPrivateProfileStringW(_T("Logo"),_T("ERROR_EXPORT_RESOURCE_DTB"),_T("ERROR_EXPORT_RESOURCE_DTB"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		g_pPluginLogObject->Record(_T("resource export dtb %d\n"),ret);
		return -5;
	}
*/

	g_strCovert = strRootPath +_T("\\bin\\dtbtodts.bat");
	strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
	if (!RunOhterExe(strCmd,&strResult))
	{
		
		GetPrivateProfileStringW(_T("Logo"),_T("ERROR_EXPORT_RESOURCE_DTB"),_T("ERROR_EXPORT_RESOURCE_DTB"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		g_pPluginLogObject->Record(_T("resource export dtb %s\n"),strResult);
		return -3;
	}

	isReadkernelDtb=TRUE;

    return 0;
}

void CTestPluginRangeDlg::OnUnPackFinishMsg(WPARAM wParam, LPARAM lParam) 
{
	int ret=0;
    if(LoadFirmware()<0)
        return;

	ParseApps();

	if(LoadWallopaper()>=0){
		((CButton*)GetDlgItem(BTN_REPLACE_DEFAULT_WALLPAPER))->EnableWindow(TRUE); 
	}
 
	if(LoadAnimation()>=0){
		((CButton*)GetDlgItem(BTN_REPLACE_BOOT_ANIMATION))->EnableWindow(TRUE); 
	}

	
	if(LoadResource()>=0){
		((CButton*)GetDlgItem(BTN_REPLACE_BOOT_LOGO))->EnableWindow(TRUE);
	}
 
    ((CButton*)GetDlgItem(BTN_REPLACE_BOOT_RING))->EnableWindow(TRUE); 
	((CButton*)GetDlgItem(BTN_REPLACE_SHUTDOWN_RING))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(BTN_REPLACE_CHARGE_LOGO))->EnableWindow(TRUE); 
    
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_BUILD_PROP))->EnableWindow(TRUE); 
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_KEY_MAP))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_BATTERY_PARAM))->EnableWindow(TRUE); 
	((CButton*)GetDlgItem(IDC_BTN_MODIFY_KERNEL_PARAMETER))->EnableWindow(TRUE); 
	((CButton*)GetDlgItem(IDC_CHECK_WAPE_USER))->EnableWindow(TRUE);
	
    GetDlgItem(IDC_BTN_CONFIRM_MODIFY)->EnableWindow(TRUE);
    GetDlgItem(IDC_TAB_LOGO)->EnableWindow(TRUE);
    
    m_tabApks.EnableWindow(TRUE);
    m_tabApks.SetCurSel(0);
	m_tabLogo.SetCurSel(0);
    m_sysApksList.EnableWindow(TRUE);
    m_sysApksList.ShowWindow(SW_SHOW);
    m_preInstApksList.EnableWindow(TRUE);
    m_preInstApksList.ShowWindow(HIDE_WINDOW);

    g_bHasUnpacked = TRUE;

}


BOOL CTestPluginRangeDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

//	_CrtSetBreakAlloc(2505);//查看内存泄漏
    // Init module path
	GetModuleFileName(NULL, m_strModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	m_strModulePath.ReleaseBuffer();
    m_strModulePath = m_strModulePath.Left(m_strModulePath.ReverseFind(_T('\\'))+1);
	strConfigPath = m_strModulePath + _T("config.ini");

    // Init log file
    BOOL bRet;
    g_pPluginLogObject = NULL;
    m_strLogPath = m_strModulePath + _T("Log\\");    
    if ( !PathFileExists(m_strLogPath) )
    {
        CreateDirectory(m_strLogPath,NULL);
	}
    g_pPluginLogObject = new cmLog(m_strLogPath,bRet);
    if (bRet)
    {
        g_pPluginLogObject->LogFileName = _T("PluginLog");
	}

	InitSystemPath();
    GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_TITLE"),_T("MESSAGE_TITLE"),
        m_strMsgBoxTitle.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);

	CString strTxt;
	GetPrivateProfileStringW(_T("Version"),_T("android"),_T("4.2"),
		strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,strInitConfiginiPath);
	if(strTxt.CompareNoCase(_T("4.2"))==0)
		m_androidVersion=ANDROID_VERTION_FOUR_TWO;
	else if(strTxt.CompareNoCase(_T("4.4"))==0)
		m_androidVersion=ANDROID_VERTION_FOUR_FOUR;
	else if(strTxt.CompareNoCase(_T("5.0"))==0)
		m_androidVersion=ANDROID_VERTION_FiVE_ZERO;
	else if(strTxt.CompareNoCase(_T("6.0"))==0)
		m_androidVersion=ANDROID_VERTION_SIX_ZERO;
	else
	     m_androidVersion=ANDROID_VERTION_UNKNOWN;
	strTxt.ReleaseBuffer(MAX_LENGTH);

	GetPrivateProfileStringW(_T("Kernel"),_T("kernel"),_T("3.10"),
		strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,strInitConfiginiPath);
	if(strTxt.CompareNoCase(_T("3.10"))==0)
		m_kernelVersion=KERNEL_VERTION_THREE_ONE_ZERO;
	else
		m_kernelVersion=KERNEL_VERTION_UNKNOWN;

	strTxt.ReleaseBuffer(MAX_LENGTH);


    if (!LoadConfig()) {
        if (g_pPluginLogObject) {
            g_pPluginLogObject->Record(_T("Load adb info faild, use default value"));
        }
        m_strAdbVid = _T("2207");
        m_strAdbPid = _T("0006");
    }

    InitVariable();
	
    InitUIComponents();

	GetPrivateProfileStringW(_T("Debug"),_T("Debug"),_T("0"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, strConfigPath);
	if(strTxt.CompareNoCase(_T("1"))==0)
		OnUnPackFinishMsg(0,0);
	strTxt.ReleaseBuffer(MAX_LENGTH);

    pWinThread = AfxBeginThread(AdbDeviceDetectThread, this);
    TRACE1("AdbDeviceDetectThread: 0x%x\n", pWinThread->m_nThreadID);

	((CStatic*)GetDlgItem(IDC_STATIC_VERSION))->SetWindowTextW(FirmwareVersion);
    return TRUE;
}
DWORD CTestPluginRangeDlg::GetDirSize(CString strFullPath)
{
    //查找指定目录大小的归递函数
    DWORD dwDirSize,dwSubDirSize;
    CFileFind finder;

    dwDirSize=dwSubDirSize=0;
    strFullPath+= "\\*.* ";
    if(finder.FindFile   (strFullPath)) 
    {
        while(1) 
        {
            BOOL bFound; 
            bFound=finder.FindNextFile();
            if(finder.IsDirectory())
            {
                if(!finder.IsDots())
                {
                    dwSubDirSize=GetDirSize(finder.GetFilePath());
                    dwDirSize+=dwSubDirSize;
                }
            }
            else
            {
                CFile file; 
                if(file.Open(finder.GetFilePath(), CFile::modeRead))
                {
                    dwDirSize+=file.GetLength();
                    file.Close();
                }
            }
            if(bFound==FALSE)
                break;
        }
    }
    finder.Close();
    return dwDirSize;
}

void CTestPluginRangeDlg::ImportAnimation(CString AnimationPath, EMANIMATIONS animType) 
{
    int n=0,m=0,k=0,num=0;
    CString FileName, tmp, unzip_path, strTxt;
    CString AnroidAnimationZip;
    FileName=AnimationPath;
    n = FileName.ReverseFind('\\');

    isExistAnroidAnimationZip = FALSE;

    switch(animType)
    {
    case BOOT_ANIMATION:
        AnroidAnimationZip =FileName.Left(n)+"\\bootanimation.zip";
        strAnimationUnZipPath=TemporaryFolderPath+"\\bootanimation";
        break;
    case SHUTDOWN_ANIMATION:
        AnroidAnimationZip =FileName.Left(n)+"\\shutdownanimation.zip";
        strAnimationUnZipPath=TemporaryFolderPath+"\\shutdownanimation";
        break;
    default:
        AnroidAnimationZip =FileName.Left(n)+"\\bootanimation.zip";
        strAnimationUnZipPath=TemporaryFolderPath+"\\bootanimation";
        break;
    }
    
    
    unzip_path=strTempPath;
    n = unzip_path.ReverseFind('\\');
    
    CString AnroidAnimation_DescPath=strAnimationUnZipPath+"\\desc.txt";
    
    if(FolderExist(AnimationPath))
        DeleteFolder(AnimationPath);
    
    if(FolderExist(strAnimationUnZipPath))
        DeleteFolder(strAnimationUnZipPath);
    
    m_AnimationPos = NULL;
    m_AnimationPathList.RemoveAll();
    
    
    if (PathFileExists(AnroidAnimationZip))
    {
        
        HZIP hz = OpenZip(AnroidAnimationZip,0);
        ZIPENTRY ze; 
        GetZipItem(hz,-1,&ze); 
        int numitems=ze.index;// -1 gives overall information about the zipfile
        
        if(!CreateDirectory(strAnimationUnZipPath,NULL))
        {
            GetPrivateProfileStringW(_T("Logo"),_T("ERROR_CREATE_ZIP_DIR_FAILED"),_T("ERROR_CREATE_ZIP_DIR_FAILED"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            return;
        }
        SetUnzipBaseDir(hz,strAnimationUnZipPath);

        for (int zi=0; zi<numitems; zi++)
        { 
            ZIPENTRY ze;
            GetZipItem(hz,zi,&ze); // fetch individual details
            tmp=(CString)ze.name;
            FileName=tmp.Right(3);
            n=FileName.CompareNoCase(_T("JPG"));
	        m=FileName.CompareNoCase(_T("PNG"));
			k=FileName.CompareNoCase(_T("GIF"));
            if((n==0)||(m==0)||(k==0))
            {
                tmp=(CString)ze.name;
                tmp.Replace(_T("/"),_T("\\"));
                //wmemcpy(pAnimationPaths[num++].name,tmp,tmp.GetLength());
                m_AnimationPathList.AddTail(tmp);
                //TRACE1("TMP:%s\n", tmp);
            }
            UnzipItem(hz, zi, ze.name);         // e.g. the item's name.			
        }
        CloseZip(hz);
        //wmemcpy(pAnimationPaths[num++].name,_T("end"),3);
        
        isExistAnroidAnimationZip =TRUE;
    }
    else
    {
        switch(animType)
        {
        case BOOT_ANIMATION:
            GetPrivateProfileStringW(_T("Logo"),_T("ERROR_FIND_BOOTANIMATION"),_T("ERROR_FIND_BOOTANIMATION"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            break;
        case SHUTDOWN_ANIMATION:
            GetPrivateProfileStringW(_T("Logo"),_T("ERROR_FIND_SHUTDOWNANIMATION"),_T("ERROR_FIND_SHUTDOWNANIMATION"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            break;
            }
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
        return;
    }
    
    if(PathFileExists(AnroidAnimation_DescPath))
    {
        char *x;
        char *pBuffer;
        char content_buf[20];
        char loop = 1;
        int  content_len = 0;
        int Index=0;
        
        memset(content_buf,0,sizeof(content_buf));
        pBuffer = read_file(AnroidAnimation_DescPath, 0);
        
        if (!pBuffer) {
            GetPrivateProfileStringW(_T("Logo"),_T("ERROR_READ_DESC_FAILED"),_T("ERROR_READ_DESC_FAILED"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
        }
        else
        {	
            x = pBuffer;
            while(loop)
            {
                switch(*x)
                {
                case 0x0D:
                case ' ':
                    if(content_len==0)
                    {
                        x++;
                        break;
                    }
                    
                    switch(Index)
                    {
                    case 0:
                        AndroidAnimation.ScreenWidth=atoi(content_buf);
                        break;
                    case 1:
                        AndroidAnimation.ScreenHeight=atoi(content_buf);
                        break;
                    case 2:
                        AndroidAnimation.FramesPersecond=atoi(content_buf);
                        break;
                    case 3:
                        AndroidAnimation.playparam[Index/6].PlayChar=content_buf[0];
                        break;
                    case 4:
                        AndroidAnimation.playparam[Index/6].DelayTime=atoi(content_buf);
                        break;	
                    default:
                        break;
                    }	
                    content_len =0;
                    x++;
                    Index++;
                    memset(content_buf,0,20);
                    break;
                    case  0: loop = 0; break;
                    default: content_buf[content_len] = *x++;
                        content_len++;
                        break;
                }
            }
        }
        if(pBuffer)
			free(pBuffer);
    }
    
    
}

int CTestPluginRangeDlg::CheckDisplayPictrueSize(CString filePath,int imagetype)
{
    CxImage image;
     
    image.Load( (LPCTSTR)filePath, imagetype);
    
    if (image.IsValid())
    {
        if((image.GetHeight()>wKernelMaxPictureHeight)||(image.GetWidth()>wKernelMaxPictureWidth))
            return -1;
    }
    return 0;
}

int CTestPluginRangeDlg::CheckDisplayPictrueType(CString filePath)
{
    CxImage image,rotateImage;
    int type;
    
    CString ext = filePath.Right(filePath.GetLength()-filePath.ReverseFind('.') - 1);
    
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
    
    return type;
}


static DWORD ThreadProc(LPVOID lpParameter)
{
	PTHREADARG pArg = (PTHREADARG)lpParameter;
	WaitForSingleObject(pArg->param1.hProcess,pArg->param2); 

	//保存参数

	return 0;
}

bool CTestPluginRangeDlg::CreateTheard(PTHREADARG pArg)
{
	// TODO: Add your control notification handler code here
	HANDLE  hThread = NULL;
	hThread = ::CreateThread(
	NULL,        //系统安全描述，NULL
	0,         //Windows系统一般线程栈大小为1 MB，创建线程的数目与物理内存和栈空间大小有关
	(LPTHREAD_START_ROUTINE)ThreadProc, //线程启动函数
	(LPVOID)pArg,    //线程启动函数接收的参数
	0,         //CREATE_SUSPENDED
	NULL);        //线程ID
 
	if(!hThread )
	{
		return false;
	}
	//WaitForSingleObject(hThread,INFINITE); 
	CloseHandle(hThread);     

	return true;

}
bool CTestPluginRangeDlg::runExternalExe(CString strCmd,int mode,int type)
{

	HANDLE   hStdinRd,   hStdinWr; 
	HANDLE   hStdoutRd,   hStdoutWr; 

	SECURITY_ATTRIBUTES   sa; 
	sa.nLength=sizeof(SECURITY_ATTRIBUTES); 
	sa.bInheritHandle=TRUE; 
	sa.lpSecurityDescriptor=NULL; 

	if (!CreatePipe(&hStdinRd,&hStdinWr,&sa,0)) 
	{ 
		return FALSE; 
	} 
	
	if (!CreatePipe(&hStdoutRd,&hStdoutWr,&sa,0)) 
	{ 
		return FALSE; 
	} 	

	PROCESS_INFORMATION   pi; 
	STARTUPINFO   si; 

	memset(&si,0,sizeof(si));   
	si.cb = sizeof(si); 
	si.dwFlags=STARTF_USESTDHANDLES; 
	si.hStdInput=hStdinRd; 
	si.hStdOutput=hStdoutWr; 
	si.hStdError=hStdoutWr; 
	si.wShowWindow = mode; 
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if(!CreateProcessW(NULL,strCmd.GetBuffer(strCmd.GetLength()),NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) 
	{
		MessageBox(_T("Create Process failed!"));
		return false;
	} 
	if(type==SYSTEM_CONFIG_RUNIMAGE)
		WaitForSingleObject(pi.hProcess,INFINITE);
	/*
	PTHREADARG pArg=new THREADARG;
	pArg->param1=pi;
	pArg->param2=INFINITE;

	if(!CreateTheard(pArg))
	{
		MessageBox(_T("Create Thread failed!"));
		delete pArg;
		return false;
	}
	delete pArg;
	*/
	CloseHandle(hStdinRd); 
	CloseHandle(hStdoutWr); 
	return true; 
}

bool CTestPluginRangeDlg::Covert224PNM(CString srcfilepath,CString decfilepath)
{
	CString strCMD,strExePath;
	strExePath= strRootPath+_T("\\bin\\ppmquant.exe");
	
	if(strExePath.Find(_T(" "))>=0)
		strExePath.Replace(_T(" "),_T("\" \""));

	if(srcfilepath.Find(_T(" "))>=0)
		srcfilepath.Replace(_T(" "),_T("\" \""));

	if(decfilepath.Find(_T(" "))>=0)
		decfilepath.Replace(_T(" "),_T("\" \""));

	strCMD.Format(_T("cmd /C %s 224 %s > %s"),strExePath,srcfilepath,decfilepath);	
	return runExternalExe(strCMD,SW_HIDE,SYSTEM_CONFIG_RUNIMAGE);

}

bool CTestPluginRangeDlg::CovertPPM(CString srcfilepath,CString decfilepath)
{
	CString strCMD,strExePath;
	strExePath= strRootPath+_T("\\bin\\pnmtoplainpnm.exe");
	
	if(strExePath.Find(_T(" "))>=0)
		strExePath.Replace(_T(" "),_T("\" \""));
	
	if(srcfilepath.Find(_T(" "))>=0)
		srcfilepath.Replace(_T(" "),_T("\" \""));
	
	if(decfilepath.Find(_T(" "))>=0)
		decfilepath.Replace(_T(" "),_T("\" \""));
	
	strCMD.Format(_T("cmd /C %s %s > %s"),strExePath,srcfilepath,decfilepath);
	return runExternalExe(strCMD,SW_HIDE,SYSTEM_CONFIG_RUNIMAGE);	
}

bool CTestPluginRangeDlg::CovertPicture(CString srcfilepath,CString decfilepath,int srcimagetype,int decimagetype)
{
	CxImage image;
	image.Load( (LPCTSTR)srcfilepath, srcimagetype);

	if (image.IsValid())
    {
		return (image.Save((LPCTSTR)decfilepath,decimagetype));
	}
	return FALSE;

}
int CTestPluginRangeDlg::DisplayPictrue(CString filePath,bool isFullScreen,int isbootlogo)
{
    CxImage image,rotateImage;
    int type;
	COLORREF crBackground = RGB(0, 0, 0);
    
    CString ext = filePath.Right(filePath.GetLength()-filePath.ReverseFind('.') - 1);
    
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
    
    image.Load( (LPCTSTR)filePath, type);
    // 判断图像是否有效
    
    if (image.IsValid())
    {
        CString strPixelInfo,strPixelInfoMax;
        strPixelInfo.Format(_T("Pixel:%dx%d"), image.GetWidth(), image.GetHeight());
	
		//strPixelInfoMax.Format(_T("MaxPixel:%dx%d"),wKernelMaxPictureWidth,wKernelMaxPictureHeight);
        CWnd *pWnd=GetDlgItem(IDC_STATIC_LOGO);
        CDC *pDC = pWnd->GetDC();
		HDC hdc = pDC->GetSafeHdc();
        if (pWnd)
        {
			CRect TableRect;
            RECT rect;
            pWnd->GetClientRect(&rect);
            if(!isFullScreen)
            {
                if(((LONG)(image.GetHeight()))<rect.bottom)
                {
                    rect.bottom = image.GetHeight();
                }
                
                if(((LONG)(image.GetWidth()))<rect.right)
                {
                    rect.right = image.GetWidth();
                }
				pWnd->GetClientRect(&TableRect);
				(pWnd->GetDC())->FillSolidRect(TableRect, crBackground); 
				CreateSolidBrush(crBackground);
            }
            
            if((image.GetHeight())>(image.GetWidth()))
            {
                image.Rotate(270,&rotateImage);//旋转90，并且保存到smallImage中
                rotateImage.Draw(hdc,rect);
            }
            else
            {
                 image.Draw(hdc,rect);
            }
            pWnd->ReleaseDC(pDC);
			if(isbootlogo)
			{
				GetDlgItem(IDC_STATIC_PIXEL_MAX_INFO)->SetWindowText(strPixelInfo);
				//GetDlgItem(IDC_STATIC_PIXEL_INFO)->SetWindowText(strPixelInfo);
			}
			else
			{
				GetDlgItem(IDC_STATIC_PIXEL_MAX_INFO)->SetWindowText(strPixelInfo);
			}
		}

    }
    
    return 1;
}

void CTestPluginRangeDlg::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default
    CString FileName;
    CString FileNameSign;

    switch(nIDEvent) {
    case BOOT_ANIMATION_TIMER:
    //case SHUTDOWN_ANIMATION_TIMER:
        if(needDisplayBootanimation)
        {
            if (m_AnimationPos == NULL) {
                m_AnimationPos = m_AnimationPathList.GetHeadPosition();
            }
            if (m_AnimationPos) {
                FileName = m_AnimationPathList.GetNext(m_AnimationPos);
                //TRACE1("path:%s\n", FileName);
                DisplayPictrue(strAnimationUnZipPath+_T("\\")+FileName,true,0);
            }
        }
        break;
    case CLONE_PROGRESS_TIMER:
        if (g_emCloneState == CLONE_EXECUTE_SCRIPT)
        {
            if (m_progressClone.GetPos() >= 49) return;
        } 
        else if (g_emCloneState == CLONE_PULL_IMAGE)
        {
            if (m_progressClone.GetPos() >= 99) return;
        }
        m_progressClone.StepIt();
    }

    CDialog::OnTimer(nIDEvent);
}
void CTestPluginRangeDlg::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    CWnd *pWnd=GetDlgItem(IDC_STATIC_LOGO);
    // TODO: Add your message handler code here

    int CurSel = m_tabLogo.GetCurSel();
    switch(CurSel)
    {
    case 0:
        if(PathFileExists(strResourceUbootLogoPath))
            GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
		if(isReadubootLogo)
            DisplayPictrue(strResourceUbootLogoPath,false,1); 
        break;
    case 2:
        if((needDisplayDefaultWallpaper)&&(PathFileExists(strDefaultWallpaperPath)))
            GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
            DisplayPictrue(strDefaultWallpaperPath,true,0); 
        break;
	case 3:
        if(PathFileExists(strResourceKernelLogoPath))
            GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
		if(isReadkernelLogo)
            DisplayPictrue(strResourceKernelLogoPath,false,1); 
        break;
    default:
        break;
    }
    // Do not call CDialog::OnPaint() for painting messages
}

int CTestPluginRangeDlg::SelectPlatformForInitRc()
{
    int i = 0;
    int len = sizeof(g_strPlatforms)/sizeof(TCHAR*);
    for (i = 0; i < len; i++) 
    {
        CString path1, path2;
		LPWSTR lpDir;
		lpDir=GetRootDir();
		path2 = path1 = (CString)lpDir;
		if(lpDir)
		{
			delete lpDir;
		}
        path1 = path1.Left(path1.ReverseFind('\\')) + "\\Boot\\Boot\\init." + g_strPlatforms[i] + "board.rc";
        path2 = path2.Left(path2.ReverseFind('\\')) + "\\Boot\\ramdisk\\init." + g_strPlatforms[i] + "board.rc";
        if (PathFileExists(path1) || PathFileExists(path2))
        {
            return i;
        }
    }
    return UNKNOWN_BOARD;
}

CString CTestPluginRangeDlg::DetermineBootInitRcPath()
{
    int i = 0;
    CString str;
    int len = sizeof(g_strPlatforms)/sizeof(TCHAR*);
    for (i = 0; i < len; i++) 
    {
        CString path1, path2;
        LPWSTR lpDir;
		lpDir=GetRootDir();
		path2 = path1 = (CString)lpDir;
		if(lpDir)
		{
			delete lpDir;
		}

        path1 = path1.Left(path1.ReverseFind('\\')) + "\\Boot\\Boot\\init." + g_strPlatforms[i] + "board.rc";
        path2 = path1.Left(path2.ReverseFind('\\')) + "\\Boot\\ramdisk\\init." + g_strPlatforms[i] + "board.rc";
        if (PathFileExists(path1))
        {
            return path1;
        } else if (PathFileExists(path2))
        {
            return path2;
        }
    }
    return str;
}

int CTestPluginRangeDlg::CovertCompressionBmp(CString srcPath,CString desPath) 
{
	CString strCmd;
	int iRet=0;
	CString strResult;
	CString g_strCovert = strRootPath +_T("\\bin\\convert.exe");
	CString strMidFile;

	strMidFile.Format(_T("%s\\logoback.bmp"),desPath.Left(desPath.ReverseFind('\\')));
	strCmd = CMySpawnConsumer::CovertPressionBmpCmd(g_strCovert,srcPath,strMidFile);
	if (!RunOhterExe(strCmd,&strResult))
	{
		iRet = -5;
	}	
	

	if(CovertNormalBmp(strMidFile,desPath)<0)
		iRet = -6;

	return iRet;
}

DWORD CTestPluginRangeDlg::GetCompressionFileSize(CString mPath) 
{
	CFile cFile;

	if(cFile.Open(mPath, CFile::modeRead|CFile::typeBinary))
    {
		return cFile.GetLength();
	}
	return 0;
}


void CTestPluginRangeDlg::OnReplaceKernelLogo() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int ret=0;
    CString FileType, strTxt;
	FileType=_T("ppm Files|*.ppm|bmp Files|*.bmp|jpg Files|*.jpg|png Files|*.png||");
    
    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,FileType);
	
    GetPrivateProfileStringW(_T("Common"),_T("OPEN"),_T("OPEN"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);	
    fileopenbox.m_ofn.lpstrTitle = strTxt;
    strTxt.ReleaseBuffer(MAX_LENGTH);
    
    if(fileopenbox.DoModal() == IDOK)
    {
        int n = 0;
		int imagetype;
		CString logoPath;
		
		imagetype=CheckDisplayPictrueType(fileopenbox.GetPathName());
		
		if(imagetype==CXIMAGE_FORMAT_UNKNOWN)
		{
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_IMAGE_FILE"),_T("ERROR_IMAGE_FILE"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            return;
		}
		
		CovertPicture(fileopenbox.GetPathName(),strCovertToBMPPath,imagetype,CXIMAGE_FORMAT_BMP);
		logoPath=strCovertToBMPPath;
		imagetype=CXIMAGE_FORMAT_BMP;		
		
		//CString strCovertToPressionBMPTestPath=TemporaryFolderPath+_T("\\logo_kernel.bmp");
		//if(CovertCompressionBmp(logoPath,strResourceKernelLogoPath)<0){
	//		return;
	//	}
		CopyFile(logoPath,strResourceKernelLogoPath,false);
		
		CString strCmd;
		int iRet=0;
		CString strResult;
		CString g_strCovert;

		
		if(FileExist(strResourceBatteryPath)&&(FileExist(strResourceKernelLogoPath))){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery_kernellogo.bat");
		}else if(FileExist(strResourceBatteryPath)){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery.bat");
		}else if(FileExist(strResourceKernelLogoPath)){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_kernellogo.bat");
		}else{
			g_strCovert = strRootPath +_T("\\bin\\packresource.bat");
		}

		//g_strCovert = strRootPath +_T("\\bin\\resourcetool.bat");
		strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
		if (!RunOhterExe(strCmd,&strResult))
		{
			
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_IMPORT_RESOURCE_KERNEL_LOGO"),_T("ERROR_IMPORT_RESOURCE_KERNEL_LOGO"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
			g_pPluginLogObject->Record(_T("resource import logo %s\n"),strResult);
			return;
		}

		GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		//DeleteFile(strResourceUbootLogoPath);
/*		
		//resource_export_uboot_logo(strResourcePath,strResourceUbootLogoPath);	
		g_strCovert = strRootPath +_T("\\bin\\unpackresource.bat");
		strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
		if (!RunOhterExe(strCmd,&strResult))
		{
			
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_UNPACK_RESOURCE"),_T("ERROR_UNPACK_RESOURCE"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
			g_pPluginLogObject->Record(_T("resource unpack failed %s\n"),strResult);
			return;
		}
*/		
		m_tabLogo.SetCurSel(3);
		needDisplayBootanimation =FALSE;
		KillTimer(BOOT_ANIMATION_TIMER);		      
		GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
		DisplayPictrue(strResourceKernelLogoPath,false,1);
	}
}


void CTestPluginRangeDlg::OnReplaceBootLogo() 
{
    // TODO: Add your control notification handler code here
	int ret=0;
    CString FileType, strTxt;
	FileType=_T("ppm Files|*.ppm|bmp Files|*.bmp|jpg Files|*.jpg|png Files|*.png||");
    
    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,FileType);

    GetPrivateProfileStringW(_T("Common"),_T("OPEN"),_T("OPEN"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);	
    fileopenbox.m_ofn.lpstrTitle = strTxt;
    strTxt.ReleaseBuffer(MAX_LENGTH);
    
    if(fileopenbox.DoModal() == IDOK)
    {
        int n = 0;
		int imagetype;
		CString logoPath;

		imagetype=CheckDisplayPictrueType(fileopenbox.GetPathName());

		if(imagetype==CXIMAGE_FORMAT_UNKNOWN)
		{
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_IMAGE_FILE"),_T("ERROR_IMAGE_FILE"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            return;
		}
	
		CovertPicture(fileopenbox.GetPathName(),strCovertToBMPPath,imagetype,CXIMAGE_FORMAT_BMP);
		logoPath=strCovertToBMPPath;
		imagetype=CXIMAGE_FORMAT_BMP;		
		
		//CString strCovertToPressionBMPTestPath=TemporaryFolderPath+_T("\\logo.bmp");
	//	if(CovertCompressionBmp(logoPath,strResourceUbootLogoPath)<0){
	//		return;
	//	}
		CopyFile(logoPath,strResourceUbootLogoPath,false);

		CString strCmd;
		int iRet=0;
		CString strResult;
		CString g_strCovert;
			
		if(FileExist(strResourceBatteryPath)&&(FileExist(strResourceKernelLogoPath))){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery_kernellogo.bat");
		}else if(FileExist(strResourceBatteryPath)){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery.bat");
		}else if(FileExist(strResourceKernelLogoPath)){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_kernellogo.bat");
		}else{
			g_strCovert = strRootPath +_T("\\bin\\packresource.bat");
		}
	
		strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
		if (!RunOhterExe(strCmd,&strResult))
		{
			
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_IMPORT_RESOURCE_LOGO"),_T("ERROR_IMPORT_RESOURCE_LOGO"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
			g_pPluginLogObject->Record(_T("resource import logo %s\n"),strResult);
			return;
		}

		GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
		//DeleteFile(strResourceUbootLogoPath);
/*		
		//resource_export_uboot_logo(strResourcePath,strResourceUbootLogoPath);	
	    g_strCovert = strRootPath +_T("\\bin\\unpackresource.bat");
		strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
		if (!RunOhterExe(strCmd,&strResult))
		{
			
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_UNPACK_RESOURCE"),_T("ERROR_UNPACK_RESOURCE"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
			g_pPluginLogObject->Record(_T("resource unpack failed %s\n"),strResult);
			return;
		}
*/
		m_tabLogo.SetCurSel(0);
		needDisplayBootanimation =FALSE;
		KillTimer(BOOT_ANIMATION_TIMER);		      
		GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
		DisplayPictrue(strResourceUbootLogoPath,false,1);
/*
        ret = resource_importLogo(strResourcePath,strCovertToPressionBMPPath);
        if(ret == 0) 
        {
            GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            DeleteFile(strResourceUbootLogoPath);

            resource_export_uboot_logo(strResourcePath,strResourceUbootLogoPath);	
            m_tabLogo.SetCurSel(0);
            needDisplayBootanimation =FALSE;
            KillTimer(BOOT_ANIMATION_TIMER);		      
			GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
			DisplayPictrue(strResourceUbootLogoPath,false,1);
        }
        else 
        { 		
			GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_REPLACE_PICTURE"),_T("ERROR_REPLACE_PICTURE"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
        }  
*/		
    }
}


void CTestPluginRangeDlg::OnReplaceBootAnimation() 
{
    CString strTxt;

    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("BootAnimation|*.zip||"));
    fileopenbox.m_ofn.lpstrTitle = _T("打开");
    if(fileopenbox.DoModal() == IDOK)
    {
        //copy bootanimation.zip 到指定目录
 
        if(!FolderExist(strFactoryPath))
        {
            CreateFolder(strFactoryPath);
        }
        
        CString strbootanimation_path=strSystemPath+_T("\\media\\bootanimation.zip");
        
        if(!CopyFile(fileopenbox.GetPathName(),strbootanimation_path,FALSE)) 
        { 
            MessageBox(_T("Copy File Fail!"), m_strMsgBoxTitle, MB_OK); 
        }else
        {
            GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            
            DeleteFolder(strBootAnimationBasePath);
            ImportAnimation(strBootAnimationBasePath, BOOT_ANIMATION);
            m_tabLogo.SetCurSel(1);
            needDisplayBootanimation =TRUE;
            int timecount=1000/AndroidAnimation.FramesPersecond;//每一帧需要多少秒
            SetTimer(BOOT_ANIMATION_TIMER, timecount, NULL);
            
        }
    }
}

void CTestPluginRangeDlg::OnSelchangeTabLogo(NMHDR* pNMHDR, LRESULT* pResult) 
{
    int timecount;
    GetDlgItem(IDC_STATIC_PIXEL_INFO)->SetWindowText(NULL);
	GetDlgItem(IDC_STATIC_PIXEL_MAX_INFO)->SetWindowText(NULL);

    int CurSel = m_tabLogo.GetCurSel();
    switch(CurSel)
    {
    case 0:
        needDisplayBootanimation =FALSE;
        KillTimer(BOOT_ANIMATION_TIMER);
        //KillTimer(SHUTDOWN_ANIMATION_TIMER);
        //kernel_display_logo(kernel_path);
        GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
		if(isReadubootLogo)
			DisplayPictrue(strResourceUbootLogoPath,false,1); 

        break;
    case 1:
        //KillTimer(SHUTDOWN_ANIMATION_TIMER);
        ImportAnimation(strBootAnimationBasePath, BOOT_ANIMATION);
        
        if(isExistAnroidAnimationZip)
        {
            needDisplayBootanimation =TRUE;
            timecount=1000/AndroidAnimation.FramesPersecond;//每一帧需要多少秒
            SetTimer(BOOT_ANIMATION_TIMER, timecount, NULL);
        }
        else
        {
            RedrawWindow();
        }
        break;
    case 2:
        needDisplayBootanimation =FALSE;
        KillTimer(BOOT_ANIMATION_TIMER);
        //KillTimer(SHUTDOWN_ANIMATION_TIMER);
        if(needDisplayDefaultWallpaper)
            DisplayPictrue(strDefaultWallpaperPath,true,0); 
        else
            RedrawWindow();
        break;
    case 3:
        needDisplayBootanimation =FALSE;
        KillTimer(BOOT_ANIMATION_TIMER);
        //KillTimer(SHUTDOWN_ANIMATION_TIMER);
        //kernel_display_logo(kernel_path);
        GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
		if(isReadkernelLogo)
			DisplayPictrue(strResourceKernelLogoPath,false,1); 
		
        break;

    default:
        break;
    }    
    *pResult = 0;
}

void CTestPluginRangeDlg::OnReplaceDefaultWallpaper() 
{
    CString strTxt;

    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("default_wallpaper|*.jpg||"));
    fileopenbox.m_ofn.lpstrTitle = _T ("Open default_wallpaper");
    if(fileopenbox.DoModal() == IDOK)
    {
        
        if(!FolderExist(strFactoryPath))
        {
            CreateFolder(strFactoryPath);
        }
        
        
        CString strdefault_wallpaper_path=strSystemPath+ _T("\\media\\rkfactory\\default_wallpaper.jpg");
        
        if(!CopyFile(fileopenbox.GetPathName(),strdefault_wallpaper_path,FALSE)) 
        { 
            MessageBox(_T("Copy File Fail!"),MB_OK); 
        }else
        {
            GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            needDisplayDefaultWallpaper=TRUE;
            needDisplayBootanimation =FALSE;
            KillTimer(BOOT_ANIMATION_TIMER);
			GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();
            DisplayPictrue(strDefaultWallpaperPath,true,0);
            m_tabLogo.SetCurSel(2);
        }
    }	
}


BOOL CTestPluginRangeDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    int menuID = LOWORD(wParam);
    int itemcount;
    CString strTxt;
    CListCtrl *pCurrentList;
	CString strAppPath;
	CString strApkName;
    
    if(menuID == POPMENU_DEL)
    {
        GetPrivateProfileStringW(_T("APKList"),_T("DELETE_FILE"),_T("DELETE_FILE"),
            strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
        if(MessageBox(strTxt, m_strMsgBoxTitle, MB_YESNO) == IDYES)
        {
            if (m_nCurrenListIndex == 1)
            {
                pCurrentList = &m_sysApksList;
            } 
            else
            {
                pCurrentList = &m_preInstApksList;
            }
            
            POSITION  sSelPos = NULL;
            CString delstr;
            while(sSelPos = pCurrentList->GetFirstSelectedItemPosition())
            {
                int nSelItem = -1;
                nSelItem = pCurrentList->GetNextSelectedItem(sSelPos);
                if(nSelItem >= 0 && nSelItem < pCurrentList->GetItemCount())
                {
                	if(m_androidVersion==ANDROID_VERTION_FOUR_TWO)
					{
                    	delstr = m_strAppsPath + pCurrentList->GetItemText(nSelItem,0);
                	}else if(m_androidVersion==ANDROID_VERTION_FOUR_FOUR){
						if (m_nCurrenListIndex == 1){
							delstr = m_strAppsPath + pCurrentList->GetItemText(nSelItem,0);
							if(!DeleteFile(delstr)){
								delstr = m_strAppsPath + pCurrentList->GetItemText(nSelItem,0);
								DeleteFile(delstr);
							}
						}
						else{
							delstr = m_strAppsPrePath + pCurrentList->GetItemText(nSelItem,0);
							DeleteFile(delstr);
						}
			
					}else if(m_androidVersion==ANDROID_VERTION_FiVE_ZERO || m_androidVersion==ANDROID_VERTION_SIX_ZERO ){	
						strApkName=pCurrentList->GetItemText(nSelItem,0);
						if (m_nCurrenListIndex == 1){
							strAppPath.Format(_T("%s%s%s"),m_strAppsPath,strApkName.Left(strApkName.ReverseFind('.')),_T("\\"));
							delstr = strAppPath + pCurrentList->GetItemText(nSelItem,0);
							if(!DeleteFolder(strAppPath)){
								strAppPath.Format(_T("%s%s%s"),m_priAppsPath,strApkName.Left(strApkName.ReverseFind('.')),_T("\\"));
								delstr = strAppPath + pCurrentList->GetItemText(nSelItem,0);
								DeleteFolder(strAppPath);
							}
						}else{						
							strAppPath.Format(_T("%s%s%s"),m_strAppsPrePath,strApkName.Left(strApkName.ReverseFind('.')),_T("\\"));
							delstr = strAppPath + pCurrentList->GetItemText(nSelItem,0);	
							DeleteFolder(strAppPath);
						}
									
						//TRACE1("%s",delstr);
					}
                    pCurrentList->DeleteItem(nSelItem);
                }
                
                CString system_path;
                CString file_path;
                CString parameter_path;
				LPWSTR lpDir;
				lpDir=GetRootDir();
				file_path = (CString)lpDir;
				if(lpDir)
				{
					delete lpDir;
				}            
                int n = 0;
				if(m_androidVersion==ANDROID_VERTION_FOUR_TWO){
	                if(m_nCurrenListIndex == 1)
	                {
	                    int ret=DeleteFile(delstr);
	                    n = file_path.ReverseFind('\\');
	                    parameter_path = file_path.Left(n);
	                    system_path = parameter_path;
	                    system_path += _T("\\System\\");
	                    m_nCurrentSize = GetDirSize(system_path);
	                }
	                else
	                {
	    
	                    delstr = SetFileTitleFromFileName(delstr,_T(".nm"));
	                    DeleteFile(delstr);
	                    
	                    n = file_path.ReverseFind('\\');
	                    parameter_path = file_path.Left(n);
	                    system_path = parameter_path;
	                    system_path += _T("\\System\\");
	                    
	                    m_nCurrentSize = GetDirSize(system_path);
	                } 
				}else if(m_androidVersion==ANDROID_VERTION_FOUR_FOUR){
					 	//int ret=DeleteFile(delstr);
	                    n = file_path.ReverseFind('\\');
	                    parameter_path = file_path.Left(n);
	                    system_path = parameter_path;
	                    system_path += _T("\\System\\");
	                    m_nCurrentSize = GetDirSize(system_path);
				}else if(m_androidVersion==ANDROID_VERTION_FiVE_ZERO || m_androidVersion==ANDROID_VERTION_SIX_ZERO ){
						//DeleteFolder(strAppPath);
						//TRACE1("%s\n",strAppPath);
						n = file_path.ReverseFind('\\');
						parameter_path = file_path.Left(n);
						system_path = parameter_path;
						system_path += _T("\\System\\");
						m_nCurrentSize = GetDirSize(system_path);
				}
            }
        }
        strTxt.ReleaseBuffer(MAX_LENGTH);
    }else if(menuID == POPMENU_ADD)
    {
        CString filename;
		CString fullfilename;
        FILETIME ft;
        CString substr;
        CFileFind file;
        int nSize;
		POSITION pos;
		
        CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLEHOOK,_T("Programs|*.apk|All Files (*.*)|*.*||"));
        GetPrivateProfileStringW(_T("Common"),_T("OPEN"),_T("OPEN"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
        fileopenbox.m_ofn.lpstrTitle = strTxt;
        strTxt.ReleaseBuffer(MAX_LENGTH);
        if(fileopenbox.DoModal() == IDOK)
        {
            SHFILEINFO info;
			pos=fileopenbox.GetStartPosition();
			while(pos){		
				//filename = fileopenbox.GetPathName();
				fullfilename.Empty();
				fullfilename = fileopenbox.GetNextPathName(pos);

				filename = fullfilename.Right(fullfilename.GetLength()-fullfilename.ReverseFind(_T('\\'))-1);

				if(file.FindFile(filename))
				{
					file.FindNextFile(); 
					SHGetFileInfo(fullfilename,0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
					nSize = m_nCurrentSize + file.GetLength64();
					substr.Format(_T("nSize = %d , max = %d"),nSize,MAX_SINGLE_APK_FILE_SIZE);

					if(m_androidVersion==ANDROID_VERTION_FOUR_TWO){
						if(m_nCurrenListIndex == 1)
						{
							if (!FileExist(m_strAppsPath + filename))
								{
									itemcount = m_sysApksList.GetItemCount();
									m_sysApksList.InsertItem(itemcount,info.szDisplayName);
									substr.Format(_T("%d K"),file.GetLength64() / 1024);
									m_sysApksList.SetItemText(itemcount,1,substr);
									file.GetLastWriteTime(&ft);
									CTime FileTime(ft);
									substr = FileTime.Format("%y-%m-%d");
									m_sysApksList.SetItemText(itemcount,2,substr);
								
									CopyFile(fullfilename,m_strAppsPath + filename,false);
									m_nCurrentSize += file.GetLength64();
								} 
								else
								{
									GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_FILE_EXIST"),_T("MESSAGE_FILE_EXIST"),
										strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
									MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
								}                    
						}else{

							if (!FileExist(SetFileTitleFromFileName(m_strAppsPath + filename,_T(".nm"))))
							{
								itemcount = m_preInstApksList.GetItemCount();
								m_preInstApksList.InsertItem(itemcount,info.szDisplayName);
								substr.Format(_T("%d K"),file.GetLength64() / 1024);
								m_preInstApksList.SetItemText(itemcount,1,substr);
								file.GetLastWriteTime(&ft);
								CTime FileTime(ft);
								substr = FileTime.Format("%y-%m-%d");
								m_preInstApksList.SetItemText(itemcount,2,substr);

								CopyFile(filename,SetFileTitleFromFileName(m_strAppsPath + filename,_T(".nm")),false);
								m_nCurrentSize += file.GetLength64();
							}else{
								GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_FILE_EXIST"),_T("MESSAGE_FILE_EXIST"),
									strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
								MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
							}
						}
					}else if(m_androidVersion==ANDROID_VERTION_FOUR_FOUR){
						if(m_nCurrenListIndex == 1)
						{
							TRACE1("%s\n",m_strAppsPath);
							if (!FileExist(m_strAppsPath + filename))
							{
								itemcount = m_sysApksList.GetItemCount();
								m_sysApksList.InsertItem(itemcount,info.szDisplayName);
								substr.Format(_T("%d K"),file.GetLength64() / 1024);
								m_sysApksList.SetItemText(itemcount,1,substr);
								file.GetLastWriteTime(&ft);
								CTime FileTime(ft);
								substr = FileTime.Format("%y-%m-%d");
								m_sysApksList.SetItemText(itemcount,2,substr);

								CopyFile(fullfilename,m_strAppsPath + filename,false);
								m_nCurrentSize += file.GetLength64();
							} 
							else
							{
								GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_FILE_EXIST"),_T("MESSAGE_FILE_EXIST"),
									strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
								MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
							}                    
						}else{
							if (!FileExist(SetFileTitleFromFileName(m_strAppsPrePath + filename,_T(".apk"))))
							{
								itemcount = m_preInstApksList.GetItemCount();
								m_preInstApksList.InsertItem(itemcount,info.szDisplayName);
								substr.Format(_T("%d K"),file.GetLength64() / 1024);
								m_preInstApksList.SetItemText(itemcount,1,substr);
								file.GetLastWriteTime(&ft);
								CTime FileTime(ft);
								substr = FileTime.Format("%y-%m-%d");
								m_preInstApksList.SetItemText(itemcount,2,substr);

								if(!FolderExist(m_strAppsPrePath))
								CreateFolder(m_strAppsPrePath);
								CopyFile(filename,SetFileTitleFromFileName(m_strAppsPrePath + filename,_T(".apk")),false);
								m_nCurrentSize += file.GetLength64();
							} 
							else
							{
								GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_FILE_EXIST"),_T("MESSAGE_FILE_EXIST"),
									strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
								MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
							}
						}
					}else if(m_androidVersion==ANDROID_VERTION_FiVE_ZERO || m_androidVersion==ANDROID_VERTION_SIX_ZERO ){
						if(m_nCurrenListIndex == 1)
						{
							strAppPath.Format(_T("%s%s%s"),m_strAppsPath,filename.Left(filename.ReverseFind('.')),_T("\\"));
							if (!FileExist(strAppPath + filename))
							{
								itemcount = m_sysApksList.GetItemCount();
								m_sysApksList.InsertItem(itemcount,info.szDisplayName);
								substr.Format(_T("%d K"),file.GetLength64() / 1024);
								m_sysApksList.SetItemText(itemcount,1,substr);
								file.GetLastWriteTime(&ft);
								CTime FileTime(ft);
								substr = FileTime.Format("%y-%m-%d");
								m_sysApksList.SetItemText(itemcount,2,substr);

								CreateFolder(strAppPath);
								CopyFile(fullfilename,strAppPath + filename,false);
								m_nCurrentSize += file.GetLength64();
							} 
							else
							{
								GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_FILE_EXIST"),_T("MESSAGE_FILE_EXIST"),
									strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
								MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
							}                    
						}else{
							strAppPath.Format(_T("%s%s%s"),m_strAppsPrePath,filename.Left(filename.ReverseFind('.')),_T("\\"));
							TRACE1("%s\n",strAppPath);
							if (!FileExist(SetFileTitleFromFileName(strAppPath + filename,_T(".apk"))))
							{
								itemcount = m_preInstApksList.GetItemCount();
								m_preInstApksList.InsertItem(itemcount,info.szDisplayName);
								substr.Format(_T("%d K"),file.GetLength64() / 1024);
								m_preInstApksList.SetItemText(itemcount,1,substr);
								file.GetLastWriteTime(&ft);
								CTime FileTime(ft);
								substr = FileTime.Format("%y-%m-%d");
								m_preInstApksList.SetItemText(itemcount,2,substr);
								
								if(!FolderExist(strAppPath))
									CreateFolder(strAppPath);
								CopyFile(filename,SetFileTitleFromFileName(strAppPath + filename,_T(".apk")),false);
								m_nCurrentSize += file.GetLength64();
							} 
							else
							{
								GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_FILE_EXIST"),_T("MESSAGE_FILE_EXIST"),
									strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
								MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
							}
						}
					}			
				}
			}
		}
	}
    return CDialog::OnCommand(wParam, lParam);
}

CString CTestPluginRangeDlg::GetFileTitleFromFileName(CString FileName,CString& ExtendName) //获取文件名和扩展名
{
    int Where;
    Where=FileName.ReverseFind('\\');
    if(Where==-1)
    {
        Where=FileName.ReverseFind('/');
    }
    CString FileTitle=FileName.Right(FileName.GetLength()-1-Where);
    int Which=FileTitle.ReverseFind('.');
    ExtendName=FileTitle.Right(FileTitle.GetLength()-Which-1);
    if (Which!=-1)
    {
        FileTitle=FileTitle.Left(Which);
    }
    return FileTitle;
}

CString CTestPluginRangeDlg::SetFileTitleFromFileName(CString FileName,CString ExtendName) //获取文件名和扩展名
{
    CString FileTitle=FileName;
    int Which=FileTitle.ReverseFind('.');
    if (Which!=-1)
    {
        FileTitle=FileTitle.Left(Which);
    }
    
    FileTitle += ExtendName;
    return FileTitle;
}

void CTestPluginRangeDlg::OnSelchangeTabApk(NMHDR* pNMHDR, LRESULT* pResult) 
{
    int CurSel = m_tabApks.GetCurSel();
    switch(CurSel)
    {
    case 0:        
        m_sysApksList.ShowWindow(SW_SHOW);
        m_preInstApksList.ShowWindow(SW_HIDE);
        break;
    case 1:
        m_preInstApksList.ShowWindow(SW_SHOW);
        m_sysApksList.ShowWindow(SW_HIDE);
        break;
    default:
        break;
    }    
    *pResult = 0;
}

void CTestPluginRangeDlg::BrowseCurrentAllFile(CString strDir,CListCtrl	&ApksList)
{
	CFileFind file;
	FILETIME ft;
	CString substr,strFilter,strpath;
	CString filename;
	CString tempname;

	if(strDir == _T(""))		
	{
		
		return;		
	}	
	else		
	{		
		if(strDir.Right(1) != _T("\\"))		
			strDir += L"\\";		
		strDir =strDir+_T("*.*");		
    }

	TRACE1("%s\n",strDir);
	BOOL bContinue = file.FindFile(strDir);
	while(bContinue)
	{
		bContinue = file.FindNextFile();
		strpath = file.GetFilePath(); 
		if(file.IsDirectory() && !file.IsDots()) 
		{
			BrowseCurrentAllFile(strpath,ApksList);	
		}else if(!file.IsDirectory() && !file.IsDots())
		{
			filename=file.GetFileName();
			tempname = filename.Right(3);
			TRACE1("%s\n",filename);
			TRACE1("%s\n",tempname);
			if(!tempname.CompareNoCase(_T("apk")))
			{
				SHFILEINFO info;
				CString temp = strDir;
				int index = temp.Find(L"*.*");
				temp.Delete(index,3);
				SHGetFileInfo(temp + file.GetFileName(),0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
				
				GetFileTitleFromFileName(file.GetFileName(),strFilter);
				//int i = m_ImageList.Add(info.hIcon);	          	            
				
				ApksList.InsertItem(filecount_list,info.szDisplayName);
				substr.Format(_T("%d K"),file.GetLength64()/1024);
				ApksList.SetItemText(filecount_list - 1,1,substr);
				file.GetLastWriteTime(&ft);
				CTime FileTime(ft);
				substr = FileTime.Format(_T("%y-%m-%d"));
				ApksList.SetItemText(filecount_list - 1,2,substr);
				filecount_list++;
			}	           
		}
	}

	return;
}

void CTestPluginRangeDlg::ParseApps()
{
    FILETIME ft;
    CString substr,strFilter;
    int filecount_list1 = 1;
    int filecount_list2 = 1;
    m_nCurrenListIndex = 1;
    
    CString str = "";
	CString strRootDir = "";
    CString str_app;
    CString str_root;
	CString strTxt;

	LPWSTR lpDir;
	lpDir=GetRootDir();
	strRootDir = (CString)lpDir;
	if(lpDir)
	{
		delete lpDir;
	} 
    m_sysApksList.DeleteAllItems();
    m_preInstApksList.DeleteAllItems();

	if(m_androidVersion==ANDROID_VERTION_FOUR_TWO)
	{
		str += _T("\\app\\");
		m_strAppsPath = str;
		
		if(str.Right(1) != "\\")
			str += "\\";
		 str += "*.*";

		CFileFind file;
	    BOOL bContinue = file.FindFile(str);
	    while(bContinue)
	    {
	        bContinue = file.FindNextFile();
	        if(!file.IsDirectory() && !file.IsDots())
	        {
	            SHFILEINFO info;
	            CString temp = str;
	            int index = temp.Find(L"*.*");
	            temp.Delete(index,3);
	            SHGetFileInfo(temp + file.GetFileName(),0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
	            
	            GetFileTitleFromFileName(file.GetFileName(),strFilter);
	            //int i = m_ImageList.Add(info.hIcon);
	            
	            if((strFilter == "nm") || (strFilter == "NM"))
	            {
	                
	                m_preInstApksList.InsertItem(filecount_list2,SetFileTitleFromFileName(info.szDisplayName,_T(".apk")));
	                substr.Format(_T("%d K"),file.GetLength64()/1024);
	                m_preInstApksList.SetItemText(filecount_list2 - 1,1,substr);
	                file.GetLastWriteTime(&ft);
	                CTime FileTime(ft);
	                substr = FileTime.Format(_T("%y-%m-%d"));
	                m_preInstApksList.SetItemText(filecount_list2 - 1,2,substr);
	                filecount_list2++;
	            }
	            else
	            {
	                m_sysApksList.InsertItem(filecount_list1,info.szDisplayName);
	                substr.Format(_T("%d K"),file.GetLength64()/1024);
	                m_sysApksList.SetItemText(filecount_list1 - 1,1,substr);
	                file.GetLastWriteTime(&ft);
	                CTime FileTime(ft);
	                substr = FileTime.Format(_T("%y-%m-%d"));
	                m_sysApksList.SetItemText(filecount_list1 - 1,2,substr);
	                filecount_list1++;
	            }
	        }
	    }
	}
	else if(m_androidVersion==ANDROID_VERTION_FOUR_FOUR){
		CFileFind file;
		str = strRootDir+_T("\\app\\");
		m_strAppsPath = str;
		if(str.Right(1) != "\\")
			str += "\\";
		str += "*.*";

		TRACE1("%s\n",m_strAppsPath);
		
		BOOL bContinue = file.FindFile(str);
		while(bContinue)
		{
			bContinue = file.FindNextFile();
			if(!file.IsDirectory() && !file.IsDots())
			{
				SHFILEINFO info;
				CString temp = str;
				int index = temp.Find(L"*.*");
				temp.Delete(index,3);
				SHGetFileInfo(temp + file.GetFileName(),0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
				
				GetFileTitleFromFileName(file.GetFileName(),strFilter);
				
                m_sysApksList.InsertItem(filecount_list1,info.szDisplayName);
				substr.Format(_T("%d K"),file.GetLength64()/1024);
				m_sysApksList.SetItemText(filecount_list1 - 1,1,substr);
				file.GetLastWriteTime(&ft);
				CTime FileTime(ft);
				substr = FileTime.Format(_T("%y-%m-%d"));
				m_sysApksList.SetItemText(filecount_list1 - 1,2,substr);
				filecount_list1++;
			}
	    }

		str = strRootDir+_T("\\priv-app\\");
		m_priAppsPath = str;
		if(str.Right(1) != "\\")
			str += "\\";
		str += "*.*";
		
		TRACE1("%s\n",m_priAppsPath);
		
		bContinue = file.FindFile(str);
		while(bContinue)
		{
			bContinue = file.FindNextFile();
			if(!file.IsDirectory() && !file.IsDots())
			{
				SHFILEINFO info;
				CString temp = str;
				int index = temp.Find(L"*.*");
				temp.Delete(index,3);
				SHGetFileInfo(temp + file.GetFileName(),0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
				
				GetFileTitleFromFileName(file.GetFileName(),strFilter);
				
                m_sysApksList.InsertItem(filecount_list1,info.szDisplayName);
				substr.Format(_T("%d K"),file.GetLength64()/1024);
				m_sysApksList.SetItemText(filecount_list1 - 1,1,substr);
				file.GetLastWriteTime(&ft);
				CTime FileTime(ft);
				substr = FileTime.Format(_T("%y-%m-%d"));
				m_sysApksList.SetItemText(filecount_list1 - 1,2,substr);
				filecount_list1++;
			}
		}


		str = strRootDir+_T("\\preinstall\\");
		m_strAppsPrePath = str;
		if(str.Right(1) != "\\")
			str += "\\";
		str += "*.*";
		TRACE1("%s\n",m_strAppsPrePath);
		if(!FolderExist(m_strAppsPrePath))
			CreateFolder(m_strAppsPrePath);

		bContinue = file.FindFile(str);
		while(bContinue)
		{
			bContinue = file.FindNextFile();
			if(!file.IsDirectory() && !file.IsDots())
			{
				SHFILEINFO info;
				CString temp = str;
				int index = temp.Find(L"*.*");
				temp.Delete(index,3);
				SHGetFileInfo(temp + file.GetFileName(),0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
				
				GetFileTitleFromFileName(file.GetFileName(),strFilter);
				
				m_preInstApksList.InsertItem(filecount_list2,SetFileTitleFromFileName(info.szDisplayName,_T(".apk")));
				substr.Format(_T("%d K"),file.GetLength64()/1024);
				m_preInstApksList.SetItemText(filecount_list2 - 1,1,substr);
				file.GetLastWriteTime(&ft);
				CTime FileTime(ft);
				substr = FileTime.Format(_T("%y-%m-%d"));
				m_preInstApksList.SetItemText(filecount_list2 - 1,2,substr);
				filecount_list2++;
			}
	    }
	}else{
		str =strRootDir+_T("\\app\\");
		m_strAppsPath = str;
		if(str.Right(1) != "\\")
			str += "\\";
	   filecount_list=1;
	   BrowseCurrentAllFile(str,m_sysApksList);

	   str =strRootDir+_T("\\priv-app\\");
	   m_strAppsPath = str;
	   if(str.Right(1) != "\\")
		   str += "\\";
	   filecount_list=1;
	   BrowseCurrentAllFile(str,m_sysApksList);

		str = strRootDir+_T("\\preinstall\\");
		m_strAppsPrePath = str;
		if(!FolderExist(m_strAppsPrePath))
			CreateFolder(m_strAppsPrePath);

		if(str.Right(1) != "\\")
			str += "\\";
	   filecount_list=1;
	   BrowseCurrentAllFile(str,m_preInstApksList);
	}    
}

void CTestPluginRangeDlg::OnRclickListPreinstApks(NMHDR* pNMHDR, LRESULT* pResult) 
{
    m_nCurrenListIndex = 2;
    CMenu Menu;
    CMenu* pPopupMenu; //菜单指针
    POSITION selectedItemPos;

    CPoint Pos;
    GetCursorPos(&Pos); //取得光标位置
    if(!Menu.LoadMenu(IDR_MENULIST))
    {
        return;
    }
    if(!(pPopupMenu = Menu.GetSubMenu(0))) //获取弹出菜单的第一层子菜单的类指针
    {
        return;
    }

    // localize menu item
    LocalizeMenuCaptions(pPopupMenu);
    
    selectedItemPos = m_preInstApksList.GetFirstSelectedItemPosition();
    if (selectedItemPos == NULL)
    {
        pPopupMenu->RemoveMenu(1, MF_BYPOSITION);
    }
    SetForegroundWindow();
    pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Pos.x, Pos.y, this); //弹出菜单
    *pResult = 0;
}

void CTestPluginRangeDlg::OnRclickListSysApks(NMHDR* pNMHDR, LRESULT* pResult) 
{
    m_nCurrenListIndex = 1;
    CMenu Menu;
    CMenu* pPopupMenu; //菜单指针
    POSITION selectedItemPos;
    
    CPoint Pos;
    GetCursorPos(&Pos); //取得光标位置
    if(!Menu.LoadMenu(IDR_MENULIST))   
    { 
        return; 
    } 
    if(!(pPopupMenu = Menu.GetSubMenu(0))) //获取弹出菜单的第一层子菜单的类指针
    {
        return;
    }
    
    // localize menu item
    LocalizeMenuCaptions(pPopupMenu);

    SetForegroundWindow();
    selectedItemPos = m_sysApksList.GetFirstSelectedItemPosition();
    if (selectedItemPos == NULL)
    {
        pPopupMenu->RemoveMenu(1, MF_BYPOSITION);
    }
    pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Pos.x, Pos.y, this); //弹出菜单

    *pResult = 0;
}

// FindMenuItem() will find a menu item string from the specified
// popup menu and returns its position (0-based) in the specified 
// popup menu. It returns -1 if no such menu item string is found.
int CTestPluginRangeDlg::FindMenuItem(CMenu* Menu, LPCTSTR MenuString)
{
    ASSERT(Menu);
    ASSERT(::IsMenu(Menu->GetSafeHmenu()));
    
    int count = Menu->GetMenuItemCount();
    for (int i = 0; i < count; i++)
    {
        CString str;
        if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&
            (_tcscmp(str, MenuString) == 0))
            return i;
    }
    
    return -1;
}

void CTestPluginRangeDlg::LocalizeMenuCaptions(CMenu* pPopupMenu)
{
    CString IDName;
    // Rename menu item
    int pos = FindMenuItem(pPopupMenu, _T("添加APP"));
    if (pos > -1)
    {
        UINT id = pPopupMenu->GetMenuItemID(pos);
        GetPrivateProfileStringW(_T("APKList"),_T("ADD_APK"),_T("ADD_APK"),IDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
        pPopupMenu->ModifyMenu(id, MF_BYCOMMAND, id, IDName);
    }
    
    pos = FindMenuItem(pPopupMenu, _T("删除APP"));
    if (pos > -1)
    {
        UINT id = pPopupMenu->GetMenuItemID(pos);
        GetPrivateProfileStringW(_T("APKList"),_T("DEL_APK"),_T("DEL_APK"),IDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
        pPopupMenu->ModifyMenu(id, MF_BYCOMMAND, id, IDName);
    }

    IDName.ReleaseBuffer(MAX_LENGTH);
}

CTestPluginRangeDlg::~CTestPluginRangeDlg()
{

}

void CTestPluginRangeDlg::OnReplaceShutdownAnimation() 
{
    CString strTxt;
    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("ShutdownAnimation|*.zip||"));
    fileopenbox.m_ofn.lpstrTitle = _T("Open");
    if(fileopenbox.DoModal() == IDOK)
    {
        //copy shutdownanimation.zip 到指定目录   
        if(!FolderExist(strFactoryPath))
        {
            CreateFolder(strFactoryPath);
        }
        
        CString strShutdownAnimationPath = strSystemPath+_T("\\media\\shutdownanimation.zip");
        
        if(!CopyFile(fileopenbox.GetPathName(),strShutdownAnimationPath,FALSE)) 
        { 
            MessageBox(_T("Copy File Fail!"),MB_OK); 
        }else
        {
            GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),
                strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            
            DeleteFolder(strShutdownAnimationBasePath);
            ImportAnimation(strShutdownAnimationBasePath, SHUTDOWN_ANIMATION);
            m_tabLogo.SetCurSel(3);
            needDisplayBootanimation =TRUE;
            int timecount=1000/AndroidAnimation.FramesPersecond;//每一帧需要多少秒
            KillTimer(BOOT_ANIMATION_TIMER);
            SetTimer(SHUTDOWN_ANIMATION_TIMER, timecount, NULL);
        }
    }
}

void CTestPluginRangeDlg::InitSystemPath()
{
    int n;
	LPWSTR lpDir;
	lpDir=GetRootDir();
	strSystemPath = (CString)lpDir;
	if(lpDir)
	{
		delete lpDir;
	}
    n = strSystemPath.ReverseFind('\\');
    strTempPath = strSystemPath.Left(n);
    n = strTempPath.ReverseFind('\\');
    strRootPath = strTempPath.Left(n);


	TemporaryFolderPath=strRootPath+_T("\\dts");
	CreateFolder(TemporaryFolderPath);

	strRecoverInitRcPath =strTempPath+_T("\\Recovery\\recovery\\init.rc");

    strPackageFilePath = strTempPath + _T("\\Android\\package-file");

	strFactoryPath=strSystemPath+_T("\\media\\rkfactory");

	strBootRingtonePath=strSystemPath+_T("\\media\\audio\\startup.wav");
	strShutdownRingtonePath=strSystemPath+_T("\\media\\audio\\shutdown.wav");

	strBuildPropPath=strSystemPath+_T("\\build.prop");

	strDefaultPropPath=strTempPath+_T("\\Recovery\\recovery\\default.prop");
	strKernelPath=strTempPath+"\\Android\\Image\\kernel.img";
	strResourcePath=strTempPath+"\\Android\\Image\\resource.img";
	strMiscPath=strTempPath+"\\Android\\Image\\misc.img";

	strInitSystemConfiginiPath=strRootPath+_T("\\config\\systemconfig.ini");

	strSystemConfigPath=TemporaryFolderPath+_T("\\systemconfig.ini");
	strMoudlePath=strSystemPath+_T("\\lib\\modules");

	strInitConfiginiPath=strRootPath+_T("\\config.ini");

	strResourceDtsPath=strRootPath+_T("\\dts\\Resource.dts");

	strResourceDtbPath=strRootPath+_T("\\dts\\rk-kernel.dtb");

	strFsRamTabPath=strTempPath+("\\Boot\\ramdisk\\fstab.rk30board.bootmode.emmc");

	strFsBootTabPath=strTempPath+("\\Boot\\Boot\\fstab.rk30board.bootmode.emmc");

	strFsPackageFileTabPath=strTempPath+("\\Android\\package-file");


    switch(g_emLang)
    {
    case EmLang_ENG:
        g_strLanguageConfigFilePath=strRootPath+_T("\\Language\\English.ini");
        break;
    case EmLang_CHN:
    default:
        g_strLanguageConfigFilePath=strRootPath+_T("\\Language\\Chinese.ini");
        break;
    }

	strCovertToBMPPath=TemporaryFolderPath+_T("\\temp.bmp");
	//strCovertToPressionBMPPath=TemporaryFolderPath+_T("\\logo.bmp");
	strCovertToPPMPath=TemporaryFolderPath+_T("\\temp.ppm");

	strResourceUbootLogoPath=strRootPath+_T("\\dts\\logo.bmp");
	strResourceKernelLogoPath =strRootPath+_T("\\dts\\logo_kernel.bmp");
	strResourceBatteryPath =strRootPath+_T("\\dts\\charge_anim_desc.txt");

	return;
}
void CTestPluginRangeDlg::InitVariable()
{
    g_bHasUnpacked = FALSE;
    g_bIsCloning = FALSE;

    mDatabkImageSize = 0;
    needDisplayBootanimation = FALSE;
    needDisplayDefaultWallpaper = FALSE;
    isExistAnroidAnimationZip = FALSE;
    isReadubootLogo = FALSE;
	isReadkernelLogo = FALSE;
	isOpenedSysconfigIni= FALSE;
	isSystemConfigIniWrited=FALSE;

    m_AnimationPos = NULL;
	m_dlgNotePad=NULL;
	m_dlgChargeImage=NULL;
	m_dlgBatteryParam=NULL;
	
}

void CTestPluginRangeDlg::InitUIComponents()
{
    CString strIDName;

    InitSystemInfoGroupBox();
    InitUsbConfigGroupBox();
    InitSystemSettingsGroupBox();
    InitSysStatusCloneGroupBox();
    InitLogoConfigGroupBox();
    InitApkConfigGroupBox();

    GetDlgItem(IDC_STATIC_LOGO)->RedrawWindow();

    GetPrivateProfileStringW(_T("SystemSeting"),_T("CONFIRM_MODIFICTION"),_T("CONFIRM_MODIFICTION"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BTN_CONFIRM_MODIFY)->SetWindowText(strIDName);
    
    GetPrivateProfileStringW(_T("SystemSeting"),_T("MODIFY_BUILD_PROP"),_T("MODIFY_BUILD_PROP"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BTN_MODIFY_BUILD_PROP)->SetWindowText(strIDName);
    
    GetPrivateProfileStringW(_T("SystemSeting"),_T("MODIFY_KAY_MAPPED"),_T("MODIFY_KAY_MAPPED"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BTN_MODIFY_KEY_MAP)->SetWindowText(strIDName);

    GetPrivateProfileStringW(_T("SystemSeting"),_T("MODIFY_BATTERY_PARAMS"),_T("MODIFY_BATTERY_PARAMS"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BTN_MODIFY_BATTERY_PARAM)->SetWindowText(strIDName);
    
    GetPrivateProfileStringW(_T("SystemSeting"),_T("MODIFY_KERNEL_PARAMETER"),_T("MODIFY_KERNEL_PARAMETER"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BTN_MODIFY_KERNEL_PARAMETER)->SetWindowText(strIDName);
    strIDName.ReleaseBuffer(MAX_LENGTH);

    m_progressClone.SetRange(0, 100);
    m_progressClone.SetStep(1);
}

void CTestPluginRangeDlg::InitSystemInfoGroupBox()
{
    CString strIDName;

    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_SYS_INFO"),_T("IDC_STATIC_SYS_INFO"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_SYS_INFO)->SetWindowText(strIDName);

    GetPrivateProfileStringW(_T("SystemSeting"),_T("COMPANY"),_T("COMPANY"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC9)->SetWindowText(strIDName);
    GetDlgItem(IDC_SYS_EDIT1)->SetWindowText(NULL);
    GetDlgItem(IDC_SYS_EDIT1)->EnableWindow(FALSE);
    
    GetPrivateProfileStringW(_T("SystemSeting"),_T("MODEL_NUMBER"),_T("MODEL_NUMBER"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC1)->SetWindowText(strIDName);
    GetDlgItem(IDC_SYS_EDIT2)->SetWindowText(NULL);
    GetDlgItem(IDC_SYS_EDIT2)->EnableWindow(FALSE);
    
    GetPrivateProfileStringW(_T("SystemSeting"),_T("BUILD_NUMBER_SDK"),_T("BUILD_NUMBER_SDK"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC8)->SetWindowText(strIDName);
    GetDlgItem(IDC_SYS_EDIT4)->SetWindowText(NULL);
    GetDlgItem(IDC_SYS_EDIT4)->EnableWindow(FALSE);
    
    GetPrivateProfileStringW(_T("SystemSeting"),_T("BUILD_NUMBER"),_T("BUILD_NUMBER"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC7)->SetWindowText(strIDName);
    GetDlgItem(IDC_SYS_EDIT3)->SetWindowText(NULL);
    GetDlgItem(IDC_SYS_EDIT3)->EnableWindow(FALSE);

    strIDName.ReleaseBuffer(MAX_LENGTH);
}

void CTestPluginRangeDlg::InitUsbConfigGroupBox()
{
    CString strIDName;
    
    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_USB_CONFIG"),_T("IDC_STATIC_USB_CONFIG"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_USB_CONFIG)->SetWindowText(strIDName);

    GetPrivateProfileStringW(_T("SystemSeting"),_T("USB_FACTORY"),_T("USB_FACTORY"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC2)->SetWindowText(strIDName);
    GetDlgItem(IDC_SYS_EDIT5)->SetWindowText(NULL);
    GetDlgItem(IDC_SYS_EDIT5)->EnableWindow(FALSE);
    
    GetPrivateProfileStringW(_T("SystemSeting"),_T("DISK_VOLUME"),_T("DISK_VOLUME"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC3)->SetWindowText(strIDName);
    GetDlgItem(IDC_SYS_EDIT6)->SetWindowText(NULL);
    GetDlgItem(IDC_SYS_EDIT6)->EnableWindow(FALSE);

    strIDName.ReleaseBuffer(MAX_LENGTH);
}

void CTestPluginRangeDlg::InitSystemSettingsGroupBox()
{
    CString strTxt;

    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_SYS_SETTINGS"),_T("IDC_STATIC_SYS_SETTINGS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_SYS_SETTINGS)->SetWindowText(strTxt);

	((CEdit*)GetDlgItem(IDC_SYS_EDIT7))->SetSel(0, -1);
	((CEdit*)GetDlgItem(IDC_SYS_EDIT7))->Clear();

    ((CComboBox*)GetDlgItem(IDC_SYS_EDIT7))->EnableWindow(FALSE);


    GetPrivateProfileStringW(_T("SystemSeting"),_T("DPI_SETTING"),_T("DPI_SETTING"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC4)->SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_BOOT_RING"),_T("IDC_STATIC_BOOT_RING"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_BOOT_RING)->SetWindowText(strTxt);
    GetPrivateProfileStringW(_T("Common"),_T("OPEN"),_T("OPEN"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(BTN_REPLACE_BOOT_RING)->SetWindowText(strTxt);
    GetDlgItem(BTN_REPLACE_BOOT_RING)->EnableWindow(FALSE);
    ((CEdit*)GetDlgItem(IDC_EDIT_BOOT_RING_PATH))->SetWindowText(_T(""));

	GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_SHUTDOWN_RING"),_T("IDC_STATIC_SHUTDOWN_RING"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_SHUTDOWN_RING)->SetWindowText(strTxt);
    GetPrivateProfileStringW(_T("Common"),_T("OPEN"),_T("OPEN"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(BTN_REPLACE_SHUTDOWN_RING)->SetWindowText(strTxt);
    GetDlgItem(BTN_REPLACE_SHUTDOWN_RING)->EnableWindow(FALSE);
    ((CEdit*)GetDlgItem(IDC_EDIT_SHUTDOWN_RING_PATH))->SetWindowText(_T(""));

    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_CHECK_ENABLE_BT"),_T("IDC_CHECK_ENABLE_BT"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->SetWindowText(strTxt);
    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->EnableWindow(FALSE);

	GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_CHECK_WAPE_USER"),_T("IDC_CHECK_WAPE_USER"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    ((CButton*)GetDlgItem(IDC_CHECK_WAPE_USER))->SetWindowText(strTxt);
    ((CButton*)GetDlgItem(IDC_CHECK_WAPE_USER))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_CHECK_WAPE_USER))->EnableWindow(FALSE);
	

    strTxt.ReleaseBuffer(MAX_LENGTH);
}

void CTestPluginRangeDlg::InitSysStatusCloneGroupBox()
{
    CString strIDName;
	GetPrivateProfileStringW(_T("SystemSeting"),_T("SYSTEM_CLONE"),_T("SYSTEM_CLONE"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BTN_CLONE)->SetWindowText(strIDName);
    
    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_SYS_SETTINGS_BACKUP"),_T("IDC_STATIC_SYS_SETTINGS_BACKUP"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_SYS_SETTINGS_BACKUP)->SetWindowText(strIDName);

    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_EDIT_CLONE_INTRO"),_T("IDC_EDIT_CLONE_INTRO"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_EDIT_CLONE_INTRO)->SetWindowText(strIDName);
    strIDName.ReleaseBuffer(MAX_LENGTH);
}

void CTestPluginRangeDlg::InitLogoConfigGroupBox()
{
    CString strIDName;
    
    GetDlgItem(IDC_STATIC_PIXEL_INFO)->SetWindowText(NULL);
	GetDlgItem(IDC_STATIC_PIXEL_MAX_INFO)->SetWindowText(NULL);

    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_LOGO_CONFIG"),_T("IDC_STATIC_LOGO_CONFIG"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_LOGO_CONFIG)->SetWindowText(strIDName);
    GetDlgItem(IDC_STATIC_LOGO_CONFIG)->EnableWindow(FALSE);

    GetPrivateProfileStringW(_T("SystemSeting"),_T("REPLACE_BOOT_LOGO"),_T("REPLACE_BOOT_LOGO"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(BTN_REPLACE_BOOT_LOGO)->SetWindowText(strIDName);
    GetDlgItem(BTN_REPLACE_BOOT_LOGO)->EnableWindow(FALSE);


    GetPrivateProfileStringW(_T("SystemSeting"),_T("REPLACE_ANDROID_ANIMATION"),_T("REPLACE_ANDROID_ANIMATION"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(BTN_REPLACE_BOOT_ANIMATION)->SetWindowText(strIDName);
    GetDlgItem(BTN_REPLACE_BOOT_ANIMATION)->EnableWindow(FALSE);

   	GetPrivateProfileStringW(_T("SystemSeting"),_T("REPLACE_WALL_PAPER"),_T("REPLACE_WALL_PAPER"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(BTN_REPLACE_DEFAULT_WALLPAPER)->SetWindowText(strIDName);
    GetDlgItem(BTN_REPLACE_DEFAULT_WALLPAPER)->EnableWindow(FALSE);

	//GetDlgItem(BTN_REPLACE_KERNEL_LOGO)->ShowWindow(SW_HIDE);
	if(m_kernelVersion==KERNEL_VERTION_THREE_ONE_ZERO){
		GetPrivateProfileStringW(_T("SystemSeting"),_T("REPLACE_KERNEL_LOGO"),_T("REPLACE_KERNEL_LOGO"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		//GetDlgItem(BTN_REPLACE_KERNEL_LOGO)->ShowWindow(SW_SHOW);		
		GetDlgItem(BTN_REPLACE_KERNEL_LOGO)->SetWindowText(strIDName);
	    //GetDlgItem(BTN_REPLACE_KERNEL_LOGO)->EnableWindow(FALSE);
	
	}
	
	//GetDlgItem(BTN_REPLACE_KERNEL_LOGO)->ShowWindow(SW_HIDE);

    GetPrivateProfileStringW(_T("SystemSeting"),_T("REPLACE_CHARGE_ANIMATION"),_T("REPLACE_CHARGE_ANIMATION"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(BTN_REPLACE_CHARGE_LOGO)->SetWindowText(strIDName);
    GetDlgItem(BTN_REPLACE_CHARGE_LOGO)->EnableWindow(FALSE);

    // TAB
    m_tabLogo.EnableWindow(FALSE);
    m_tabLogo.SetCurFocus(0);
    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_TAB_LOGO_TAB_0"),_T("IDC_TAB_LOGO_TAB_0"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_tabLogo.InsertItem(0,strIDName);
    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_TAB_LOGO_TAB_1"),_T("IDC_TAB_LOGO_TAB_1"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_tabLogo.InsertItem(1,strIDName);
    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_TAB_LOGO_TAB_2"),_T("IDC_TAB_LOGO_TAB_2"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_tabLogo.InsertItem(2,strIDName);

	if(m_kernelVersion==KERNEL_VERTION_THREE_ONE_ZERO){
		GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_TAB_LOGO_TAB_3"),_T("IDC_TAB_LOGO_TAB_3"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
		m_tabLogo.InsertItem(3,strIDName);
	}

    strIDName.ReleaseBuffer(MAX_LENGTH);

}

void CTestPluginRangeDlg::InitApkConfigGroupBox()
{
    CString strIDName;
    
    m_tabApks.EnableWindow(FALSE);
    m_tabApks.SetCurSel(0);
    m_sysApksList.EnableWindow(FALSE);
    m_sysApksList.DeleteAllItems();
    m_preInstApksList.EnableWindow(FALSE);
    m_preInstApksList.DeleteAllItems();

    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_STATIC_APKS_CONFIG"),_T("IDC_STATIC_APKS_CONFIG"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_APKS_CONFIG)->SetWindowText(strIDName);
    
    // APKS config tab
    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_TAB_APK_TAB_0"),_T("IDC_TAB_APK_TAB_0"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_tabApks.InsertItem(0,strIDName);
    GetPrivateProfileStringW(_T("DIALOG_102"),_T("IDC_TAB_APK_TAB_1"),_T("IDC_TAB_APK_TAB_1"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_tabApks.InsertItem(1,strIDName);
    
    //     m_ImageList.Create(32,32,ILC_COLOR32,10,30);
    //     m_sysApksList.SetImageList(&m_ImageList,LVSIL_NORMAL);
    
    DWORD dwStyle = m_sysApksList.GetExtendedStyle(); 
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
    dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）  
    m_sysApksList.SetExtendedStyle(dwStyle); //设置扩展风格
    GetPrivateProfileStringW(_T("APKList"),_T("FILE_NAME"),_T("FILE_NAME"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_sysApksList.InsertColumn(0, strIDName, LVCFMT_LEFT, 300, 0);//设置列
    GetPrivateProfileStringW(_T("APKList"),_T("FILE_SIZE"),_T("FILE_SIZE"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_sysApksList.InsertColumn(1, strIDName, LVCFMT_LEFT, 100, 1);
    GetPrivateProfileStringW(_T("APKList"),_T("FILE_DATE"),_T("FILE_DATE"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_sysApksList.InsertColumn(2, strIDName, LVCFMT_LEFT, 100, 2);
    m_sysApksList.DeleteAllItems();
    
    m_preInstApksList.SetExtendedStyle(dwStyle); //设置扩展风格 
    GetPrivateProfileStringW(_T("APKList"),_T("FILE_NAME"),_T("FILE_NAME"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_preInstApksList.InsertColumn(0, strIDName, LVCFMT_LEFT, 300, 0);//设置列
    GetPrivateProfileStringW(_T("APKList"),_T("FILE_SIZE"),_T("FILE_SIZE"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_preInstApksList.InsertColumn(1, strIDName, LVCFMT_LEFT, 100, 1);
    GetPrivateProfileStringW(_T("APKList"),_T("FILE_DATE"),_T("FILE_DATE"),strIDName.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
    m_preInstApksList.InsertColumn(2, strIDName, LVCFMT_LEFT, 100, 2);
    m_preInstApksList.DeleteAllItems();
    
    strIDName.ReleaseBuffer(MAX_LENGTH);
}

void CTestPluginRangeDlg::ModifyParameterFile()
{
	int n;
	FILE *file;
	POSITION pos;
	CString str_find,strTxt,str_name;
	char Buffer[MAX_CMDLINE];

	if((file = _tfopen(strParameterPath,_T("wb")))!=NULL)
    {
        for(pos=parameter_list.GetHeadPosition();pos;)
        {
            str_find = parameter_list.GetNext(pos);
            n = str_find.Find(_T("MACHINE_MODEL:"));
            if(n>=0)
            {			
                GetDlgItemText(IDC_SYS_EDIT2,str_name);
                str_find = "MACHINE_MODEL:";
                str_find += str_name;
                str_find += _T("\r\n"); 
            }
			else
			{
				n = str_find.Find(_T("CMDLINE:"));
				if(n>=0)
				{
					if(isSystemConfigIniWrited)
						str_find=strCmdline;
				}
			}

            WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,str_find,-1,Buffer,sizeof(Buffer),NULL,NULL);
            fputs(Buffer,file);
        }
        fclose(file);
    }else {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_OPEN_FILE_FAILED"),_T("open file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
    }
}

void CTestPluginRangeDlg::WriteMiscCommand()
{
	CFile miscFile;
	UINT miscSize;
	int checkwape=FALSE;
	char *miscdata=new char[100*1024*1024];
	char misccommand[]="boot-recovery";
	bootloader_message mischead;
	BOOL bRet;

	checkwape=((CButton*)GetDlgItem(IDC_CHECK_WAPE_USER))->GetCheck();
	if(checkwape){
		bRet = miscFile.Open(strMiscPath,CFile::modeReadWrite|CFile::typeBinary);
		if (!bRet)
		{
			g_pPluginLogObject->Record(_T("misc open failed %d"),GetLastError());
			return;
		}

		miscSize=miscFile.Read(miscdata,miscFile.GetLength());
		if(miscSize!=miscFile.GetLength()){
			g_pPluginLogObject->Record(_T("misc read failed %d"),GetLastError());
			goto END;
		}

		miscFile.Seek(MISC_COMMAND_OFFSET,CFile::begin);
		miscSize=miscFile.Read(&mischead,sizeof(bootloader_message));
		if(miscSize!=sizeof(bootloader_message)){
			g_pPluginLogObject->Record(_T("misc read failed %d"),GetLastError());
			goto END;
		}
		
		if(!strcmp(mischead.command,misccommand)){
			 strcpy(&miscdata[MISC_COMMAND_OFFSET+64], "recovery\n--wipe_data");
			 miscFile.SeekToBegin();
			 miscFile.Write(miscdata,miscFile.GetLength());
		}else{
			g_pPluginLogObject->Record(_T("misccommand failed"));
			goto END;
		}
END:		
		miscFile.Close();
	}

	if(miscdata!=NULL)
			delete	miscdata;
	return;
}

void CTestPluginRangeDlg::WriteSystemConfigToRecoveryInitRc()
{
	int n;
	FILE *file;
	POSITION pos;
	CString strTxt,str_find,str_name;  
    char Buffer[1024];

	if((file = _tfopen(strRecoverInitRcPath,_T("wb")))!=NULL)
    {
        for(pos=volume_list.GetHeadPosition();pos;)
        {
            str_find = volume_list.GetNext(pos);
            n = str_find.Find(_T("setprop UserVolumeLabel"));
            if(n>=0) 
            {
                GetDlgItemText(IDC_SYS_EDIT6,str_name);
                str_find = _T("setprop UserVolumeLabel ");//str_find.Left(n);
                str_find += _T("\"");
                str_find += str_name;
                str_find += _T("\"");
                
            }
            str_find += _T("\n");
            
            WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,str_find,-1,Buffer,sizeof(Buffer),NULL,NULL);
            fputs(Buffer,file);
        }
        fclose(file);
    }else {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_OPEN_FILE_FAILED"),_T("open file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
    }
}


void CTestPluginRangeDlg::WriteSystemConfigToBootInitRc()
{
	int n;
	FILE *file;
	POSITION pos;
	CString strTxt,str_find,str_name;  
    char Buffer[1024];

	if((file = _tfopen(strRecoverInitRcPath,_T("wb")))!=NULL)
    {
        for(pos=volume_list.GetHeadPosition();pos;)
        {
            str_find = volume_list.GetNext(pos);
            n = str_find.Find(_T("setprop UserVolumeLabel"));
            if(n>=0) 
            {
                GetDlgItemText(IDC_SYS_EDIT6,str_name);
                str_find = _T("setprop UserVolumeLabel ");//str_find.Left(n);
                str_find += _T("\"");
                str_find += str_name;
                str_find += _T("\"");
                
            }
            str_find += _T("\n");
            
            WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,str_find,-1,Buffer,sizeof(Buffer),NULL,NULL);
            fputs(Buffer,file);
        }
        fclose(file);
    }else {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_OPEN_FILE_FAILED"),_T("open file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
    }
}
void CTestPluginRangeDlg::CopyKoToSystemMoudle(CString strName)
{
	CString strTxt,strTxtTemp;
	CString strFilePath;
	CString strMoudleFilePath;

	strFilePath.Format(_T("%s\\modules\\%s.ko"),strRootPath,strName);
	strMoudleFilePath.Format(_T("%s\\%s.ko"),strMoudlePath,strName);
	if(PathFileExists(strFilePath))
		CopyFile(strFilePath,strMoudleFilePath,false);
	else
	{		
		GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_COPY_FILE_FAILED"),_T("copy file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
        strTxtTemp.Format(_T("%s\n%s"),strTxt,strFilePath);
		MessageBox(strTxtTemp, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
	}
}

void CTestPluginRangeDlg::WriteTpSystemConfigToBootInitRc()
{
	int n,i,num=0;
	BOOL IsNeedClean=FALSE;
	FILE *file;
	POSITION pos;
	CString strTxt,str_find,str_name;  
    char Buffer[1024];
	CString TpName[10]={
		_T("eeti_egalax_i2c"),
		_T("sitronix_ts_a720"),
		_T("gt811_ts"),
		_T("i30_ts"),
		_T("byd693x_tx"),
	};

	for(i=0;i<5;i++)
	{
		for(pos=initboadrc_list.GetHeadPosition();pos;)
		{
          str_find = initboadrc_list.GetNext(pos);
		  n=str_find.Find(TpName[i]);
		  if(n>=0)
		  {
			  pos=initboadrc_list.FindIndex(num);
			  initboadrc_list.RemoveAt(pos);
			  break;
		  }
		  num++;
		}
		num=0;
	}

	pos=initboadrc_list.Find(_T("on boot"));
	initboadrc_list.InsertAfter(pos,strTpCmdline); 
	
	if((file = _tfopen(strBootInitBoardRcPath,_T("wb")))!=NULL)
    {
        for(pos=initboadrc_list.GetHeadPosition();pos;)
        {
            str_find = initboadrc_list.GetNext(pos);
            str_find += _T("\n");         
            WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,str_find,-1,Buffer,sizeof(Buffer),NULL,NULL);
            fputs(Buffer,file);
        }
        fclose(file);
    }else {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_OPEN_FILE_FAILED"),_T("open file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
    }
}

void CTestPluginRangeDlg::WriteSystemConfigToDefaultProp()
{
	int n;
	FILE *file;
	POSITION pos;
	CString strTxt,str_find,str_name;  
    char Buffer[1024];
	
	if((file = _tfopen(strDefaultPropPath,_T("wb")))!=NULL)
    {
        for(pos=defaultprop_list.GetHeadPosition();pos;)
        {
			
            str_find = defaultprop_list.GetNext(pos);
            n = str_find.Find(_T("ro.product.model"));
            if(n>=0)
            {
                GetDlgItemText(IDC_SYS_EDIT2,str_name);
                str_find = "ro.product.model=";
                str_find += str_name;              
            }
			str_find += "\n";
            WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,str_find,-1,Buffer,sizeof(Buffer),NULL,NULL);
            fputs(Buffer,file);
        }
        fclose(file);
    }else {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_OPEN_FILE_FAILED"),_T("open file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);
    }
}

void CTestPluginRangeDlg::WriteSystemConfigToBuildProp()
{
    int n;
//    FILE *file;
    POSITION pos;
    CString strTxt,str_find,str_name;  
   // char Buffer[1024];
	int nSrcSize;
	CFile cFile;
	int count;

    //if((file = _tfopen(strBuildPropPath,_T("wb")))!=NULL)
	if(cFile.Open(strBuildPropPath, CFile::modeWrite | CFile::modeCreate))
    {
		//cFile.SeekToBegin();
		//cFile.Write("\xef\xbb\xbf", 3);
		count=content_list.GetCount();
        for(pos=content_list.GetHeadPosition();pos;)
        {
            str_find = content_list.GetNext(pos);
            n = str_find.Find(_T("ro.sf.lcd_density="));
            if(n>=0)
            {
                str_find = _T("ro.sf.lcd_density=");	
			    ((CComboBox*)GetDlgItem(IDC_SYS_EDIT7))->GetWindowText(str_name);
				str_find += str_name;       
            }else
            {
                n = str_find.Find(_T("ro.build.display.id"));
                if(n>=0)
                {
                    GetDlgItemText(IDC_SYS_EDIT3,str_name);
                    str_find = _T("ro.build.display.id=");
                    str_find += str_name;
                }else 
                {
                    n = str_find.Find(_T("ro.product.model"));
                    if(n>=0)
                    {
                        GetDlgItemText(IDC_SYS_EDIT2,str_name);
                        str_find = _T("ro.product.model=");
                        str_find += str_name;
                    }else
                    {
                        n = str_find.Find(_T("ro.rksdk.version"));
                        if(n>=0)
                        {
                            GetDlgItemText(IDC_SYS_EDIT4,str_name);
                            str_find = _T("ro.rksdk.version=");
                            str_find += str_name;
                        }
                        else
                        {
                            n = str_find.Find(_T("ro.product.brand"));
                            if(n>=0)
                            {
                                GetDlgItemText(IDC_SYS_EDIT1,str_name);
                                str_find = _T("ro.product.brand=");
                                str_find += str_name;
                            } else {
                                n = str_find.Find(_T("ro.product.usbfactory"));																			
                                if(n>=0)
                                {
                                    str_find = _T("ro.product.usbfactory=");
                                    GetDlgItemText(IDC_SYS_EDIT5,str_name);
                                    str_find += str_name;
                                } else {
                                    n = str_find.Find(_T("ro.rk.bt_enable"));
                                    if(n>=0)
                                    {
                                        str_find = _T("ro.rk.bt_enable=");
                                        if (((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->GetCheck()) {
                                            str_find += _T("true");
                                        } else {
                                            str_find += _T("false");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
			str_find += _T("\n");	
            //WideCharToMultiByte(CP_UTF8,WC_COMPOSITECHECK,str_find,-1,Buffer,sizeof(Buffer),NULL,NULL);
			char *pSrcBuf=NULL;
			UnicodeToU8(pSrcBuf, nSrcSize, (LPTSTR)(LPCTSTR)str_find);
            //fputs(pSrcBuf,file);
			cFile.Write(pSrcBuf, nSrcSize * sizeof(char));
			if(pSrcBuf!=NULL)
				delete[] pSrcBuf;
			count--;
			if(count<=1)
				break;
        }
		cFile.Close();
        //fclose(file);
    }else
    {
        GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_OPEN_FILE_FAILED"),_T("open file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
        MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
        strTxt.ReleaseBuffer(MAX_LENGTH);		
    }
}
void CTestPluginRangeDlg::OnPackFinishMsg(WPARAM wParam, LPARAM lParam) 
{
	RemoveTempFolderFile();
	InitVariable();
	EnableUIControls();	
	OnUnPackFinishMsg(0,0);
}

void CTestPluginRangeDlg::OnBtnConfirmModify() 
{
    //g_bHasUnpacked = FALSE;
	CString FileType, strTxt;
	FileType=_T("img Files(*.img)|*.img||");

    CFileDialog fileopenbox(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,FileType);

    GetPrivateProfileStringW(_T("Common"),_T("OPEN"),_T("OPEN"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);	
    fileopenbox.m_ofn.lpstrTitle = strTxt;
    strTxt.ReleaseBuffer(MAX_LENGTH);
    needDisplayBootanimation = FALSE;
    if(fileopenbox.DoModal() == IDOK)
    {
		m_strNewImagePath = fileopenbox.GetPathName();
        needDisplayBootanimation =FALSE;
// 		KillTimer(BOOT_ANIMATION_TIMER);
// 		KillTimer(SHUTDOWN_ANIMATION_TIMER);
		
		//如果内核设置有修改过parameter，就重新加载
		CFileStatus filestatus;
		CString strParameterTime;
		CFile::GetStatus(strParameterPath,filestatus);
		strParameterTime=filestatus.m_mtime.Format("%Y-%m-%d %H:%M:%S");

		if(strParameterTime.CompareNoCase(strInitParameterTime)!=0)
		{
			if(ReadParameterFile()<0)
			{
				GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_READ_FILE_PARAMETER"),_T("Read parameter failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);		
				MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
				strTxt.ReleaseBuffer(MAX_LENGTH);
				return;
			}		
			GetComlineParameter();
			ParsePartitionsInfo(ExtractCmdlinePartitionsInfo());	
		}
		
		AfxBeginThread(ModifyChangesThread, this);
		needDisplayBootanimation = TRUE;
	}
	needDisplayBootanimation = TRUE;
}

void CTestPluginRangeDlg::RemoveTempFolder()
{
	DeleteFolder(TemporaryFolderPath);
}

void CTestPluginRangeDlg::RemoveTempFolderFile()
{
	DeleteFolderFile(TemporaryFolderPath);	
}

void CTestPluginRangeDlg::DisableUIControls()
{
    GetDlgItem(IDC_TAB_LOGO)->EnableWindow(FALSE);
    
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT2))->EnableWindow(FALSE); 
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT5))->EnableWindow(FALSE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT4))->EnableWindow(FALSE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT6))->EnableWindow(FALSE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT3))->EnableWindow(FALSE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT1))->EnableWindow(FALSE);
    ((CEdit*)GetDlgItem(IDC_EDIT_BOOT_RING_PATH))->EnableWindow(FALSE);
    
    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_WAPE_USER))->EnableWindow(FALSE);

    ((CComboBox*)GetDlgItem(IDC_SYS_EDIT7))->EnableWindow(FALSE);
    
    ((CButton*)GetDlgItem(BTN_REPLACE_BOOT_LOGO))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(BTN_REPLACE_BOOT_ANIMATION))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(BTN_REPLACE_DEFAULT_WALLPAPER))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(BTN_REPLACE_KERNEL_LOGO))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(BTN_REPLACE_BOOT_RING))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(BTN_REPLACE_SHUTDOWN_RING))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(BTN_REPLACE_CHARGE_LOGO))->EnableWindow(FALSE); 
    ((CButton*)GetDlgItem(IDC_BTN_CONFIRM_MODIFY))->EnableWindow(FALSE);
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_BUILD_PROP))->EnableWindow(FALSE); 
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_BATTERY_PARAM))->EnableWindow(FALSE); 
	((CButton*)GetDlgItem(IDC_BTN_MODIFY_KERNEL_PARAMETER))->EnableWindow(FALSE); 
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_KEY_MAP))->EnableWindow(FALSE); 

	
    
    m_tabApks.EnableWindow(FALSE);
    m_sysApksList.EnableWindow(FALSE);
    m_preInstApksList.EnableWindow(FALSE);

    m_progressClone.SetPos(0);

	g_bHasUnpacked=FALSE;
	((CButton*)GetDlgItem(IDC_BTN_CLONE))->EnableWindow(FALSE);
}

void CTestPluginRangeDlg::EnableUIControls()
{
    GetDlgItem(IDC_TAB_LOGO)->EnableWindow(TRUE);
    
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT2))->EnableWindow(TRUE); 
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT5))->EnableWindow(TRUE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT4))->EnableWindow(TRUE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT6))->EnableWindow(TRUE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT3))->EnableWindow(TRUE);
    ((CEdit*)GetDlgItem(IDC_SYS_EDIT1))->EnableWindow(TRUE);
    ((CEdit*)GetDlgItem(IDC_EDIT_BOOT_RING_PATH))->EnableWindow(TRUE);    
    
    ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_BT))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_WAPE_USER))->EnableWindow(TRUE);

    ((CComboBox*)GetDlgItem(IDC_SYS_EDIT7))->EnableWindow(TRUE);    
    
    ((CButton*)GetDlgItem(BTN_REPLACE_BOOT_LOGO))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(BTN_REPLACE_BOOT_ANIMATION))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(BTN_REPLACE_DEFAULT_WALLPAPER))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(BTN_REPLACE_KERNEL_LOGO))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(BTN_REPLACE_BOOT_RING))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(BTN_REPLACE_SHUTDOWN_RING))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(BTN_REPLACE_CHARGE_LOGO))->EnableWindow(TRUE); 
    ((CButton*)GetDlgItem(IDC_BTN_CONFIRM_MODIFY))->EnableWindow(TRUE); 
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_BUILD_PROP))->EnableWindow(TRUE);
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_BATTERY_PARAM))->EnableWindow(TRUE); 
	((CButton*)GetDlgItem(IDC_BTN_MODIFY_KERNEL_PARAMETER))->EnableWindow(TRUE); 
    ((CButton*)GetDlgItem(IDC_BTN_MODIFY_KEY_MAP))->EnableWindow(TRUE); 

    m_tabApks.EnableWindow(TRUE);
    m_sysApksList.EnableWindow(TRUE);
    m_preInstApksList.EnableWindow(TRUE);
}

HBRUSH CTestPluginRangeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    CRect rect;
    COLORREF crBackground = RGB(0, 0, 0);
    COLORREF crText = RGB(255, 255, 255);
    CWnd* pStaticLogoWnd = GetDlgItem(IDC_STATIC_LOGO);
    CWnd* pPixelInfoWnd = GetDlgItem(IDC_STATIC_PIXEL_INFO);
	CWnd* pMaxPixelInfoWnd = GetDlgItem(IDC_STATIC_PIXEL_MAX_INFO);
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
    switch(pWnd->GetDlgCtrlID()) 
    { 
    case IDC_STATIC_LOGO:
        pStaticLogoWnd->GetClientRect(&rect);
        pDC->FillSolidRect(rect, crBackground); 
        m_brBackBrush.DeleteObject();
        m_brBackBrush.CreateSolidBrush(crBackground);
        return static_cast<HBRUSH> (m_brBackBrush.GetSafeHandle());
    case IDC_STATIC_PIXEL_INFO:
	case IDC_STATIC_PIXEL_MAX_INFO:
        pDC->SetTextColor(crText);
        pDC->SetBkMode(TRANSPARENT);
        return HBRUSH(GetStockObject(HOLLOW_BRUSH));
    default: 
        break; 
    }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CTestPluginRangeDlg::OnBtnModifyBuildProp() 
{
	needDisplayBootanimation =FALSE;
//    ShellExecute(NULL, _T("open"), _T("NOTEPAD.EXE"), _T("d:\\Work\\RKTool\\RKFactoryTools\\FirmwareTool_Release_v3.1\\Temp\\System\\build.prop"), NULL, SW_SHOWNORMAL); 
	WriteSystemConfigToBuildProp();  
    m_dlgNotePad = new CSysCfgDlg(this, strBuildPropPath);
    m_dlgNotePad->DoModal();
	ReadBuildpropFile();
    GetBuildPropParameter();
	needDisplayBootanimation = TRUE;
}

void CTestPluginRangeDlg::OnBtnModifyKeyMap() 
{
    CString strFilePath,strSysConfigPath;
    needDisplayBootanimation =FALSE;
    strSysConfigPath = strRootPath+_T("\\Temp\\System\\usr\\keylayout\\rk29-keypad.kl"); 
    m_dlgNotePad = new CSysCfgDlg(this, strSysConfigPath);
    m_dlgNotePad->DoModal();
	needDisplayBootanimation =TRUE;
}

void CTestPluginRangeDlg::OnReplaceChargeLogo() 
{
    needDisplayBootanimation =FALSE;
    m_dlgChargeImage = new CChargeImage(this);
	m_dlgChargeImage->SetLanguagePath(g_strLanguageConfigFilePath);
	m_dlgChargeImage->SetAndroidVersion(m_androidVersion);
    int i = m_dlgChargeImage->DoModal();
	m_dlgChargeImage = NULL;
    needDisplayBootanimation =TRUE;
}

void CTestPluginRangeDlg::OnBtnModifyBatteryParam() 
{
	// TODO: Add your control notification handler code here
	needDisplayBootanimation = FALSE;
	m_dlgBatteryParam = new CDlgBatteryParam(this);
    m_dlgBatteryParam->DoModal();
	m_dlgChargeImage = NULL;
	needDisplayBootanimation = TRUE;
}

void CTestPluginRangeDlg::OnBtnClone() 
{
    AfxBeginThread(SystemStatusCloneThread, this);
    ((CButton*)GetDlgItem(IDC_BTN_CLONE))->EnableWindow(FALSE);
}

void CTestPluginRangeDlg::OnReplaceBootRing() 
{
    CString strTxt;

    //CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("ringtone|*.wav||All File(*.*)|*.*||"));
    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("ringtone(*.wav)|*.wav|All File(*.*)|*.*||"));
	fileopenbox.m_ofn.lpstrTitle = _T ("Open boot ringtone");
    
    if(fileopenbox.DoModal() == IDOK)
    {
        CString strAudioPath;

        strAudioPath =strSystemPath+_T("\\media\\audio");
        
        if(!FolderExist(strAudioPath))
        {
            CreateFolder(strAudioPath);
        }
               
        if(!CopyFile(fileopenbox.GetPathName(),strBootRingtonePath,FALSE)) 
        { 
           	GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_COPY_FILE_FAILED"),_T("copy file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
			((CEdit*)GetDlgItem(IDC_EDIT_BOOT_RING_PATH))->SetWindowText(_T(""));
        }else
        {
			GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
            MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
            strTxt.ReleaseBuffer(MAX_LENGTH);
            ((CEdit*)GetDlgItem(IDC_EDIT_BOOT_RING_PATH))->SetWindowText(strBootRingtonePath);
        }
    }
}

BOOL CTestPluginRangeDlg::LoadConfig()
{
	if (!PathFileExists(strConfigPath))
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("LoadConfig-->PathFileExists failed"));
		}
		return FALSE;
	}
	bool bRet;
	bRet = m_iniConfig.Load((LPTSTR)(LPCTSTR)strConfigPath);
	if (!bRet)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("LoadConfig-->Load file failed"));
		}
		return FALSE;
	}
	cmIniSection *pSection;
	TCHAR tzADBSection[]=_T("ADB");
	pSection = m_iniConfig.GetSection(tzADBSection);
	if (!pSection)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("LoadConfig-->GetSection Language failed"));
		}
		return FALSE;
	}
	CString strValue;
	strValue.Format(_T("%s"), pSection->GetKeyValue(_T("VID")).c_str() );
	int vid;
	vid = _ttoi((LPTSTR)(LPCTSTR)strValue);
	if (vid<=0)
	{
		if (g_pPluginLogObject)
		{
			g_pPluginLogObject->Record(_T("LoadConfig-->GetKeyValue kinds failed"));
		}
		return FALSE;
	}
    m_strAdbVid = strValue;
    g_pPluginLogObject->Record(_T("Got adb device vid:0x%s"), m_strAdbVid);

	return TRUE;
}


DWORD CTestPluginRangeDlg::GetDtbFileSize(CString mPath) 
{
	CFile cFile;
	
	if(cFile.Open(mPath, CFile::modeRead|CFile::typeBinary))
    {
		return cFile.GetLength();
	}
	return 0;
}



void CTestPluginRangeDlg::OnBtnModifyKernelParameter() 
{
    // TODO: Add your control notification handler code here
/*
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

		if(!CopyFile(fileopenbox.GetPathName(),strResouceDtbPath,FALSE)) 
        { 
            MessageBox(_T("Copy File Fail!"),MB_OK);
			return;
        }

		size=GetDtbFileSize(strResouceDtbPath);
		if(size < 0)
			g_pPluginLogObject->Record(_T("open %s failed"), strResouceDtbPath);
		
		ret=resource_importDts_Dtb(strResourcePath,strResouceDtbPath);
		if(ret<0)
		{
			g_pPluginLogObject->Record(_T("Import Logo failed :%d"), ret);
			return;
		}

		GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);	
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);
	}
 
*/
	int size=0,n=0;
	CString strTxt;
	CFileStatus filestatus;
	CString strStarTime,strEndTime;
	CFile::GetStatus(strResourceDtsPath,filestatus);
	strStarTime=filestatus.m_mtime.Format("%Y-%m-%d %H:%M:%S");
	
	needDisplayBootanimation =FALSE;
    m_dlgNotePad = new CSysCfgDlg(this, strResourceDtsPath);
    m_dlgNotePad->DoModal();
	needDisplayBootanimation = TRUE;
	
	CFile::GetStatus(strResourceDtsPath,filestatus);
	strEndTime=filestatus.m_mtime.Format("%Y-%m-%d %H:%M:%S");
	
	if(strEndTime.CompareNoCase(strStarTime)!=0)
	{
		CString strCmd;
		int iRet=0;
		CString strResult;
		CString g_strCovert = strRootPath +_T("\\bin\\dtstodtb.bat");
		strCmd = CMySpawnConsumer::CovertDtstoDtb(g_strCovert);
		if (!RunOhterExe(strCmd,&strResult))
		{
		    MessageBox(strResult);
			return;
		}

		if(FileExist(strResourceBatteryPath)&&(FileExist(strResourceKernelLogoPath))){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery_kernellogo.bat");
		}else if(FileExist(strResourceBatteryPath)){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_battery.bat");
		}else if(FileExist(strResourceKernelLogoPath)){
			g_strCovert = strRootPath +_T("\\bin\\packresource_with_kernellogo.bat");
		}else{
			g_strCovert = strRootPath +_T("\\bin\\packresource.bat");
		}
		
		strCmd = CMySpawnConsumer::CovertRunBat(g_strCovert);
		if (!RunOhterExe(strCmd,&strResult))
		{
			
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_IMPORT_RESOURCE_LOGO"),_T("ERROR_IMPORT_RESOURCE_LOGO"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
			g_pPluginLogObject->Record(_T("resource import logo %s\n"),strResult);
			return;
		}
	}
		
/*        
		g_strCovert = strRootPath +_T("\\bin\\resourcetool.bat");
		if (!RunOhterExe(strCmd,&strResult))
		{
			
			GetPrivateProfileStringW(_T("Logo"),_T("ERROR_IMPORT_RESOURCE_DTB"),_T("ERROR_IMPORT_RESOURCE_DTB"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			strTxt.ReleaseBuffer(MAX_LENGTH);
			g_pPluginLogObject->Record(_T("resource import logo %s\n"),strResult);
			return;
		}
	
		size=GetDtbFileSize(strResourceDtbPath);
		if(size < 0)
			g_pPluginLogObject->Record(_T("open %s failed"), strResourceDtbPath);
		
		iRet=resource_importDtb(strResourcePath,strResourceDtbPath);
		if(iRet<0)
		{
			g_pPluginLogObject->Record(_T("Import  failed :%d"), iRet);
			return;
		}
		
		GetPrivateProfileStringW(_T("SystemSeting"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);	
		MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
		strTxt.ReleaseBuffer(MAX_LENGTH);	
*/


}

void CTestPluginRangeDlg::OnDestroy() 
{

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	RemoveTempFolder();
	
 	PartitionListFree();
	
	gThreadIsExist=FALSE;
	WaitForSingleObject(pWinThread->m_hThread, INFINITE); 
	if(g_pPluginLogObject)
	{
		delete g_pPluginLogObject;
		g_pPluginLogObject=NULL;
	}
	pWinThread=NULL;
}

BOOL CTestPluginRangeDlg::FlushPartitionsToParameterList()
{
    int n;
    POSITION pos_parameter, pos_cmdline;
    CString str_find;
    for(pos_parameter=parameter_list.GetHeadPosition();pos_parameter;)
    {
        str_find = parameter_list.GetNext(pos_parameter);
        n = str_find.Find(_T("CMDLINE"));		
        if(n>=0)
        {
            str_find = _T("CMDLINE:");
            for(pos_cmdline=comline_list.GetHeadPosition();pos_cmdline;)
            {
                str_find += comline_list.GetNext(pos_cmdline);	
            }	
            return ReplaceCStringListByKeywords(parameter_list, _T("CMDLINE:"), str_find);
        }
    }
    return FALSE;
}

int CTestPluginRangeDlg::ReadPackageFile()
{
    char *pBuffer = NULL;
    char *x = NULL;
    char content_buf[MAX_CMDLINE];
    char loop = 1;
    int  content_len = 0;
    CString str, strTxt;	
    LPWSTR lpDir = NULL;
    int nSize;

	mPackageFileList.RemoveAll();
    
    memset(content_buf,0,sizeof(content_buf));
    pBuffer = read_file(strPackageFilePath, 0);
    content_len = 0;
    loop = 1;
    if (!pBuffer){
        
        return -1;
    }
    else
    {		
        x = pBuffer;
        while(loop)
        {
            switch(*x)
            {
            case 0x0D:
                content_buf[content_len] = 0x0D;
                content_len++;					
                x++;
                if(*x == 0x0a) x++;					
                content_buf[content_len] = 0x0A;
                content_len++;
                content_buf[content_len] = 0x00;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                mPackageFileList.AddTail(lpDir);
				if (lpDir)
				{
					delete[] lpDir;
					lpDir =	 NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            case  0: 
                loop = 0;
                nSize = strlen(content_buf);
                AnsiToUnicode(lpDir,nSize,content_buf);
                mPackageFileList.AddTail(lpDir);
				if (lpDir)
				{
					delete[] lpDir;
					lpDir =	 NULL;
				}
                content_len = 0;
                memset(content_buf,0,sizeof(content_buf));
                break;
            default: content_buf[content_len] = *x++;
                content_len++;
                break;
            }
        }
    }	
    if(pBuffer)
		free(pBuffer);
    return 0;
}

void CTestPluginRangeDlg::ModifyPackageFile() {
    int n;
    FILE *file;
    POSITION pos;
    CString str_find,strTxt;
    char Buffer[1024];
    if (!FileExist(m_strModulePath+_T("Temp\\Android\\Image\\databk.img"))) {
        return;
    }
    for(pos=mPackageFileList.GetHeadPosition();pos;)
    {
        str_find = mPackageFileList.GetNext(pos);
        n = str_find.Find(_T("databk"));
        if (n >= 0)
        {
            // package-file has databk
            return;
        }
    }

    if((file = _tfopen(strPackageFilePath,_T("wb")))!=NULL)
    {
        for(pos=mPackageFileList.GetHeadPosition();pos;)
        {
            str_find = mPackageFileList.GetNext(pos);
            n = str_find.Find(_T("system"));
            if (n >= 0)
            {
                str_find += _T("databk		Image/databk.img");
                str_find += _T("\r\n");
            }
            WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,str_find,-1,Buffer,sizeof(Buffer),NULL,NULL);
            fputs(Buffer,file);
        }
    }
	fclose(file);
}

BOOL CTestPluginRangeDlg::ModifyPartitionsParameter()
{
    CString strDatabkPath, strPartitionInfo;
    DWORD nPartitionSize;
    
    strDatabkPath = m_strModulePath + _T("Temp\\Android\\Image\\databk.img");
    if (FileExist(strDatabkPath)) {
        nPartitionSize = GetProperPartitionSectorSize(strDatabkPath);
        if (nPartitionSize > 0) {
            ModifyPartitionSize(_T("databk"), nPartitionSize);
            strPartitionInfo = GeneratePartitionsInfo();
            if (strPartitionInfo.IsEmpty()) return FALSE;
            if(!ReplaceCStringListByKeywords(comline_list, _T("mtdparts"), strPartitionInfo)) return FALSE;
            if(FlushPartitionsToParameterList()){
				return UpdateFstab();
			}
			else{
				return FALSE;
			}
        } else {
            g_pPluginLogObject->Record(_T("Can't find proper databk partition size"));
        }
    } else {
        return TRUE;
    }
    return FALSE;
}

DWORD CTestPluginRangeDlg::GetProperPartitionSectorSize(DWORD dwFileSize)
{
	DWORD nMultipleSize, nPartitionSize;
    
    nMultipleSize = 128 * 1024 * 1024;
    // ceil to a multiple of 128MB
    nPartitionSize = ((dwFileSize + nMultipleSize - 1) / nMultipleSize) * nMultipleSize;
	
    // convert to section
    return nPartitionSize /= 512;
}

DWORD CTestPluginRangeDlg::GetProperPartitionSectorSize(CString strImagePath)
{
    CFileStatus fs;
    
    DWORD nMultipleSize, nPartitionSize;
    
    nMultipleSize = 128 * 1024 * 1024;

    if (!CFile::GetStatus(strImagePath, fs)) {
        g_pPluginLogObject->Record(_T("Can't get status for  databk.img in %s"), strImagePath);
        return -1; 
    }

    // ceil to a multiple of 128MB
    nPartitionSize = ((fs.m_size + nMultipleSize - 1) / nMultipleSize) * nMultipleSize;

    // convert to section
    return nPartitionSize /= 512;
}

CString CTestPluginRangeDlg::ExtractCmdlinePartitionsInfo()
{
    POSITION pos;
    CString str_find;
    CString strTemp;
    int n;

    for(pos=comline_list.GetHeadPosition();pos;)
    {
        str_find = comline_list.GetNext(pos);
        n = str_find.Find(_T("mtdparts"));
        if(n>=0) 
        { 
            strTemp = str_find;
        }
    }
    return strTemp;
}

BOOL CTestPluginRangeDlg::ReplaceCStringListByKeywords(CStringList& strList, CString keywords, CString content) {
    POSITION pos;
    CString strTemp, strPartitinos;
    int n;
    
    for(pos=strList.GetHeadPosition();pos;)
    {
        strTemp = strList.GetNext(pos);
        n = strTemp.Find(keywords);
        if(n>=0) 
        { 
            strPartitinos = strTemp;
            break;
        }
    }

    if (strPartitinos && !strPartitinos.IsEmpty())
    {
        pos = strList.Find(strPartitinos);
        if (pos != NULL) {
            strList.InsertBefore(pos, content);
            pos = strList.Find(strPartitinos);
            strList.RemoveAt(pos);
            return TRUE;
        } else {
            g_pPluginLogObject->Record(_T("can't find strPartitinos:%s"), strPartitinos);
        }
    } else {
        g_pPluginLogObject->Record(_T("can't match keyword:%s"), keywords);
    }

	

    return FALSE;
}
void CTestPluginRangeDlg::PartitionListFree() {
	POSITION pos;
    //TRACE1("%s \n", strPartitionName);
    for (pos = mPartitionList.GetHeadPosition(); pos; )
    {
        CPartitionInfo *info = (CPartitionInfo*)mPartitionList.GetNext(pos);
		if(info)
			delete info;
	}
	mPartitionList.RemoveAll();
}

BOOL CTestPluginRangeDlg::ParsePartitionsInfo(CString strParitions) {
    int n;
    CString strTemp, strOnePartition;
    CPartitionInfo* info;

    n = strParitions.Find(_T(":"));
    if (n >= 0) {
        strTemp = strParitions.Mid(n + 1);
        do 
        {
            n = strTemp.Find(_T(","));
            if (n >= 0) {
                strOnePartition = strTemp.Mid(0, n);
                strTemp.Delete(0, n + 1);
                //TRACE1("%s \n", strOnePartition);
                info = new CPartitionInfo();
                if (ParseOnePartition(strOnePartition, info)) {
                    mPartitionList.AddTail(info);
                }
				else
					delete info;
            } else {
                //TRACE1("%s \n", strTemp);
                strOnePartition = strTemp;
                info = new CPartitionInfo();
                if (ParseOnePartition(strOnePartition, info)) {
                    mPartitionList.AddTail(info);
                }
				else
					delete info;
				
                return TRUE;
            }
        } while (TRUE);
    }
    return FALSE;
}

int CTestPluginRangeDlg::ConvertCStringoHex(CString strHex) {
    LPTSTR buf;
    int nValue, nRet;

    buf = strHex.GetBuffer(16);
    nRet = _stscanf(buf, _T("%x"), &nValue);
    if (nRet > 0)
    {
        return nValue;
    }
    return 0;
}

BOOL CTestPluginRangeDlg::ParseOnePartition(CString strOnePartition, CPartitionInfo* info) {
    int m, n;
    CString strTemp;

    m = strOnePartition.Find(_T("@"));
    if (m >= 0)
    {
        strTemp = strOnePartition.Mid(0, m);
        //TRACE1("LEN:%s \n", strTemp);
        info->length = ConvertCStringoHex(strTemp);
        n = strOnePartition.Find(_T("("));
        if (n >= 0) {
            strTemp = strOnePartition.Mid(m+1, n-m-1);
            //TRACE1("ADDR:%s \n", strTemp);
            info->address = ConvertCStringoHex(strTemp);
            m = strOnePartition.Find(_T(")"));
            if (m >= 0) {
                strTemp = strOnePartition.Mid(n+1, m-n-1);
                //TRACE1("NAME:%s \n", strTemp);
                info->strPartitionName = strTemp;
                return TRUE;
            } else {
                g_pPluginLogObject->Record(_T("Not found %s in partition %s"), _T(")"), strOnePartition);
            }
        } else {
            g_pPluginLogObject->Record(_T("Not found %s in partition %s"), _T("("), strOnePartition);
        }
    } else {
        g_pPluginLogObject->Record(_T("Not found %s in partition %s"), _T("@"), strOnePartition);
    }
    return FALSE;
}

BOOL CTestPluginRangeDlg::PartitionExist(CString strPartitionName) {
    POSITION pos;
    //TRACE1("%s \n", strPartitionName);
    for (pos = mPartitionList.GetHeadPosition(); pos; )
    {
        CPartitionInfo *info = (CPartitionInfo*)mPartitionList.GetNext(pos);
        //TRACE1("%s \n", info->strPartitionName);
        if (!info->strPartitionName.CompareNoCase(strPartitionName))
        {
            return TRUE;
        }
    }
    return FALSE;
}

void CTestPluginRangeDlg::ModifyPartitionSizeInternal(CString strPartitionName, int nSectorSize) {
    // Note: the first addr is 0x00002000(misc)
    int addr = 0x00002000, len = 0;
    POSITION pos;
    for (pos = mPartitionList.GetHeadPosition(); pos; )
    {
        CPartitionInfo *info = (CPartitionInfo*)mPartitionList.GetNext(pos);
        if (!info->strPartitionName.CompareNoCase(strPartitionName))
        {
            info->length = nSectorSize;
            break;
        }
    }

    // recalculate the addr
    for (pos = mPartitionList.GetHeadPosition(); pos; )
    {
        CPartitionInfo *info = (CPartitionInfo*)mPartitionList.GetNext(pos);
        info->address = addr + len;
        len = info->length;
        addr = info->address;
        //TRACE1("NAME:%s\n", info->strPartitionName);
        //TRACE1("ADDR:%x\n", info->address);
        //TRACE1("LEN:%x\n", info->length);
    }
}

void CTestPluginRangeDlg::AddPartition(CString strPartitionName, int nSectorSize) {
    POSITION pos;

    pos = mPartitionList.GetTailPosition();
    if (pos != NULL) {
        CPartitionInfo* info = new CPartitionInfo();
        info->strPartitionName = strPartitionName;
        info->length = nSectorSize;
        mPartitionList.InsertBefore(pos, info);
		//delete info;
    } else {
        g_pPluginLogObject->Record(_T("AddPartition->Can not get tail position in mPartitionList"));
        return;
    }


    ModifyPartitionSizeInternal(strPartitionName, nSectorSize);
}

void CTestPluginRangeDlg::ModifyPartitionSize(CString strPartitionName, int nSectorSize) {
    if (PartitionExist(strPartitionName))
    {
        ModifyPartitionSizeInternal(strPartitionName, nSectorSize);
    } else {
        // try to add this new partition at the end
        AddPartition(strPartitionName, nSectorSize);
    }
}

CString CTestPluginRangeDlg::GeneratePartitionsInfo() {
    POSITION pos = NULL;
    CString strTemp, strPartitinos;
    int n = 0;
    
    for(pos=comline_list.GetHeadPosition();pos;)
    {
        strTemp = comline_list.GetNext(pos);
        n = strTemp.Find(_T("mtdparts"));
        if(n>=0) 
        { 
            strPartitinos = strTemp.Mid(0, strTemp.Find(_T(":")) + 1);
            //TRACE1("%s\n", strPartitinos);
            break;
        }
    }

    if (strPartitinos.IsEmpty())
    {
        g_pPluginLogObject->Record(_T("GeneratePartitionsInfo->strPartitinos is Empty"));
        return strPartitinos;
    }

    for (pos = mPartitionList.GetHeadPosition(); pos; )
    {
        CPartitionInfo *info = (CPartitionInfo*)mPartitionList.GetNext(pos);
        if (pos != NULL)
        {
            strTemp.Format(_T("0x%08x@0x%08x(%s),"), info->length, info->address, info->strPartitionName);
            strPartitinos += strTemp;
        } 
        else
        {
            strTemp.Format(_T("-@0x%08x(%s)"), info->address, info->strPartitionName);
            strPartitinos += strTemp;
        }
    }
    //TRACE1("%s\n", strPartitinos);
    return strPartitinos;
}

int CTestPluginRangeDlg::GetPartitionSizeInBytes(CString strPartitionName) {
    POSITION pos;
    CString strTemp, strPartitinos;
    for (pos = mPartitionList.GetHeadPosition(); pos; )
    {
        CPartitionInfo *info = (CPartitionInfo*)mPartitionList.GetNext(pos);
        if (!info->strPartitionName.CompareNoCase(strPartitionName))
        {
			g_pPluginLogObject->Record(strPartitionName+_T("%d"),info->length);
            return info->length * 512;
        }
    }
    return -1;
}

LRESULT CTestPluginRangeDlg::OnUpdateCloneBtnMsg( WPARAM wParam, LPARAM lParam )
{
    if ((g_bIsCloning)||(!g_bHasUnpacked))
    {
        return 0;
    }
    if (wParam == 1)
    {
        ((CButton*)GetDlgItem(IDC_BTN_CLONE))->EnableWindow(TRUE);
    } else {
         ((CButton*)GetDlgItem(IDC_BTN_CLONE))->EnableWindow(FALSE);
    }
    return 0;
}
void CTestPluginRangeDlg::WriteSystemConfigToParameter()
{
	ModifyParameterFile();  
}

void CTestPluginRangeDlg::OnCheckEnableUserFormat() 
{
	// TODO: Add your control notification handler code here

}

void CTestPluginRangeDlg::OnReplaceShutdownRing() 
{
	// TODO: Add your control notification handler code here
	   CString strTxt;
	   
	   //CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("ringtone|*.wav||All File(*.*)|*.*||"));
	   CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("ringtone(*.wav)|*.wav|All File(*.*)|*.*||"));
	   fileopenbox.m_ofn.lpstrTitle = _T ("Open boot ringtone");
	   
	   if(fileopenbox.DoModal() == IDOK)
	   {
		   CString strAudioPath;
		   
		   strAudioPath =strSystemPath+_T("\\media\\audio");
		   
		   if(!FolderExist(strAudioPath))
		   {
			   CreateFolder(strAudioPath);
		   }
		   
		   if(!CopyFile(fileopenbox.GetPathName(),strShutdownRingtonePath,FALSE)) 
		   { 
			   GetPrivateProfileStringW(_T("SystemSeting"),_T("ERROR_COPY_FILE_FAILED"),_T("copy file failed!"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);					
			   MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			   strTxt.ReleaseBuffer(MAX_LENGTH);
			   ((CEdit*)GetDlgItem(IDC_EDIT_SHUTDOWN_RING_PATH))->SetWindowText(_T(""));
		   }else
		   {
			   GetPrivateProfileStringW(_T("Common"),_T("LOAD_SUCCESS"),_T("LOAD_SUCCESS"),strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH,g_strLanguageConfigFilePath);
			   MessageBox(strTxt, m_strMsgBoxTitle, MB_OK);
			   strTxt.ReleaseBuffer(MAX_LENGTH);
			   ((CEdit*)GetDlgItem(IDC_EDIT_SHUTDOWN_RING_PATH))->SetWindowText(strShutdownRingtonePath);
		   }
    }
}
