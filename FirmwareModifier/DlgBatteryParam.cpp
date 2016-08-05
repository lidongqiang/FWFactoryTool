// DlgBatteryParam.cpp : implementation file
//

#include "stdafx.h"
#include "FirmwareModifier.h"
#include "DlgBatteryParam.h"

#include <windows.h>
#include <shlwapi.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern cmLog *g_pPluginLogObject;
extern int resource_importDtb(CString ResourcePath,CString dtbPath);
/////////////////////////////////////////////////////////////////////////////
// CDlgBatteryParam dialog


CDlgBatteryParam::CDlgBatteryParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBatteryParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBatteryParam)
	m_batteryCell = -1;
	//}}AFX_DATA_INIT
}


void CDlgBatteryParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBatteryParam)
	DDX_Radio(pDX, IDC_RADIO_CELL_ONE, m_batteryCell);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBatteryParam, CDialog)
	//{{AFX_MSG_MAP(CDlgBatteryParam)
	ON_BN_CLICKED(IDC_BUTTON_READ_BATTERY, OnButtonReadBattery)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_BATTERY, OnButtonWriteBattery)
	ON_BN_CLICKED(IDC_BTN_READ_BATTERY_DATA, OnBtnReadBatteryData)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBatteryParam message handlers

BOOL CDlgBatteryParam::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_batteryCell = -1;

    GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_TITLE"),_T("MESSAGE_TITLE"),
        m_strMsgBoxTitle.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);

	int n;
	LPWSTR lpDir;
	CString strTempPath;
	CString strSystemPath;
	lpDir=GetRootDir();
	strSystemPath = (CString)lpDir;
	if(lpDir)
	{
		delete lpDir;
	}
	n = strSystemPath.ReverseFind('\\');
    strTempPath = strSystemPath.Left(n);
    n = strTempPath.ReverseFind('\\');
    strRootPath=strTempPath.Left(n);
	strResourceDtsPath= strRootPath+_T("\\dts\\Resource.dts");
	strResourceDtbPath= strRootPath+_T("\\dts\\rk-kernel.dtb");
	strResourcePath=strTempPath+"\\Android\\Image\\resource.img";

    InitUIComponents();

    //ReadBatteryParameters();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


DWORD CDlgBatteryParam::GetDtbFileSize(CString mPath) 
{
	CFile cFile;
	int size=0;
	
	if(cFile.Open(mPath, CFile::modeRead|CFile::typeBinary))
    {
		size=cFile.GetLength();
		cFile.Close();
	}
	return size;
}

int CDlgBatteryParam::ReadBatteryParam( CString strKernelPath, STRUCT_BATTERY_PARAM* pBatteryParam )
{
    int nRetCode=0;
    //1.check kernel file
    CFile kernelFile;
    UINT uiKernelSize;
    BOOL bRet;
    bRet = kernelFile.Open(strKernelPath,CFile::modeRead|CFile::typeBinary);
    if (!bRet)
    {
        return -1;
    }
    uiKernelSize = kernelFile.GetLength();
    BYTE *pKernelBuffer=NULL;
    UINT uiRead;
    pKernelBuffer = new BYTE[uiKernelSize];
    if (!pKernelBuffer)
    {
        kernelFile.Close();
        nRetCode = -2;
        goto Exit_ReadBatteryParam;
    }
    uiRead = kernelFile.ReadHuge(pKernelBuffer,uiKernelSize);
    kernelFile.Close();
    if (uiRead!=uiKernelSize)
    {
        nRetCode = -3;
        goto Exit_ReadBatteryParam;
    }
    bRet = CheckKernel(pKernelBuffer);
    if (!bRet)
    {
        nRetCode = -4;
        goto Exit_ReadBatteryParam;
    }
    //2.search param tag
    int iPos;
    iPos = SearchTag(pKernelBuffer);
    if (iPos==-1)
    {
        nRetCode = -5;
        goto Exit_ReadBatteryParam;
    }
    memcpy(&pBatteryParam->dwCell,pKernelBuffer+iPos,sizeof(STRUCT_BATTERY_PARAM)-12);
Exit_ReadBatteryParam:
    if (pKernelBuffer)
    {
        delete pKernelBuffer;
		pKernelBuffer = NULL;
    }
	return nRetCode;
}

void CDlgBatteryParam::UpdateParamToInterface( STRUCT_BATTERY_PARAM* pBatteryParam )
{
    CString strValue;
    if ((pBatteryParam->dwCell!=0)&&(pBatteryParam->dwCell!=1))
    {
        m_batteryCell = -1;
    }
    else
        m_batteryCell = pBatteryParam->dwCell;
    
    strValue.Format(_T("%d"),pBatteryParam->dwPullUp);
    GetDlgItem(IDC_EDIT_PULL_UP)->SetWindowText(strValue);
    strValue.Format(_T("%d"),pBatteryParam->dwPullDown);
    GetDlgItem(IDC_EDIT_PULL_DOWN)->SetWindowText(strValue);
    int i;
    for (i=0;i<11;i++)
    {
        strValue.Format(_T("%d"),pBatteryParam->dwDischarge[i]);
        GetDlgItem(IDC_EDIT_DISCHARGE_1+i)->SetWindowText(strValue);
    }
    for (i=0;i<11;i++)
    {
        strValue.Format(_T("%d"),pBatteryParam->dwCharge[i]);
        GetDlgItem(IDC_EDIT_CHARGE_1+i)->SetWindowText(strValue);
    }
	UpdateData(FALSE);
}

void CDlgBatteryParam::ClearParamInterface()
{
    CString strValue=_T("");
    m_batteryCell = -1;
    GetDlgItem(IDC_EDIT_PULL_UP)->SetWindowText(strValue);
    GetDlgItem(IDC_EDIT_PULL_DOWN)->SetWindowText(strValue);
    int i;
    for (i=0;i<11;i++)
    {
        GetDlgItem(IDC_EDIT_DISCHARGE_1+i)->SetWindowText(strValue);
    }
    for (i=0;i<11;i++)
    {
        GetDlgItem(IDC_EDIT_CHARGE_1+i)->SetWindowText(strValue);
    }
	UpdateData(FALSE);
}

void CDlgBatteryParam::OnButtonReadBattery() 
{
    ReadBatteryParameters();
}

void CDlgBatteryParam::ReadBatteryParameters()
{
    bool bIsRun=true;
    ::SendMessage((HWND)g_msgHwnd,WM_TEST_RUNNING,0,(LPARAM)&bIsRun);
    if (!bIsRun)
    {
        char szDir[MAX_PATH];
        memset(szDir,0,MAX_PATH);
        ::SendMessage((HWND)g_msgHwnd,WM_GET_DIR,(WPARAM)EmMsg_Root_key,(LPARAM)szDir);
        if (strlen(szDir)>0)
        {
            BOOL bRet = FALSE;
            LPWSTR lpDir = NULL;
            int nSize = 0;
            bRet = AnsiToUnicode(lpDir,nSize,szDir);
            if (bRet)
            {
                CString strInfo;
                CString strKernelPath=lpDir;
                delete []lpDir;
                lpDir = NULL;
                strKernelPath += _T("\\Image\\kernel.img");
                if (!PathFileExists(strKernelPath))
                {
                    GetPrivateProfileStringW(_T("Prompt"),_T("ERROR_NOFOUND_KERNEL"),_T("ERROR_NOFOUND_KERNEL"),
                        strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
                    MessageBox(strInfo,m_strMsgBoxTitle,MB_OK|MB_ICONERROR);
                    strInfo.ReleaseBuffer(MAX_LENGTH);
                    return;
                }
                STRUCT_BATTERY_PARAM param;
                int iRet;
                iRet = ReadBatteryParam(strKernelPath,&param);
                if (iRet==0)
                {
                    UpdateParamToInterface(&param);
                    return;
                }
                else
                {
                    ClearParamInterface();
                    if (iRet==-4)
                    {
                        GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_ERROR_INVALID_KERNEL"),_T("IDS_ERROR_INVALID_KERNEL"),
                            strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
                    }
                    else if (iRet==-5)
                    {
                        GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_ERROR_NO_PARAM"),_T("IDS_ERROR_NO_PARAM"),
                            strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
                    }
                    else
                        GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_ERROR_READ_FAIL"),_T("IDS_ERROR_READ_FAIL"),
                        strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
                    MessageBox(strInfo,m_strMsgBoxTitle,MB_OK|MB_ICONERROR);
                    strInfo.ReleaseBuffer(MAX_LENGTH);
                    return;
                }
                
            }
            
        }
	}
}

char *BatteryReadfile(CString fn, unsigned *_sz)
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

BOOL CDlgBatteryParam::BatteryParamReadFile( LPCTSTR pszFilePath)
{
	char *pBuffer;
	char *x;
	char content_buf[8096];
	char loop = 1;
	int  content_len = 0;
	CString str, strTxt;	
	LPWSTR lpDir = NULL;
	int nSize;
	ResourceList.RemoveAll();
	
	memset(content_buf,0,sizeof(content_buf));
	pBuffer = BatteryReadfile(pszFilePath, 0);
	if (!pBuffer) {      
		return -1;
	}
	else{	
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
				ResourceList.AddTail(strTxt);
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
				ResourceList.AddTail(strTxt);
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

void CDlgBatteryParam::WirteBatteryParameters()
{
    bool bIsRun=true;
    ::SendMessage((HWND)g_msgHwnd,WM_TEST_RUNNING,0,(LPARAM)&bIsRun);
    if (!bIsRun)
    {
             
        CString strInfo;
        if (!PathFileExists(strResourceDtsPath))
        {
            GetPrivateProfileStringW(_T("Prompt"),_T("ERROR_NOFOUND_RESOURCE"),_T("ERROR_NOFOUND_RESOURCE"),
                strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
            MessageBox(strInfo,m_strMsgBoxTitle,MB_OK|MB_ICONERROR);
            strInfo.ReleaseBuffer(MAX_LENGTH);
            return;
        }

		if(BatteryParamReadFile(strResourceDtsPath)<0)
		{
			GetPrivateProfileStringW(_T("Prompt"),_T("ERROR_READ_DTS_FILE"),_T("ERROR_READ_DTS_FILE"),
                strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
            MessageBox(strInfo,m_strMsgBoxTitle,MB_OK|MB_ICONERROR);
            strInfo.ReleaseBuffer(MAX_LENGTH);
            return;

		}

        STRUCT_BATTERY_PARAM param;
        if (!GetParamFromInterface(&param))
        {
            GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_ERROR_INVALID_PARAM"),_T("IDS_ERROR_INVALID_PARAM"),
                strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
            MessageBox(strInfo,m_strMsgBoxTitle,MB_OK|MB_ICONERROR);
            return;
        }
        int iRet;
        iRet = WriteBatteryParam(strResourceDtsPath,&param);
        if (iRet==0)
        {
            GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_INFO_WRITE_OK"),_T("IDS_INFO_WRITE_OK"),
                strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
            MessageBox(strInfo,m_strMsgBoxTitle);
            return;
        }
        else
        {
            if (iRet==-2)
            {
                GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_ERROR_INVALID_RESOURCE"),_T("IDS_ERROR_INVALID_RESOURCE"),
                strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
            }
            else{
                GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_ERROR_NO_PARAM"),_T("IDS_ERROR_NO_PARAM"),
                strInfo.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
            }
        
            MessageBox(strInfo,m_strMsgBoxTitle,MB_OK|MB_ICONERROR);
            return;
        }
        
    }
}

void CDlgBatteryParam::InitUIComponents()
{
    CString strTxt;

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_TAB_TITLE"),_T("IDS_TAB_TITLE"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_ONE_BATTERY"),_T("IDS_ONE_BATTERY"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_RADIO_CELL_ONE)->SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_TWO_BATTERIES"),_T("IDS_TWO_BATTERIES"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_RADIO_CELL_TWO)->SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_PULL_UP"),_T("IDS_PULL_UP"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_PULL_UP)->SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_PULL_DOWN"),_T("IDS_PULL_DOWN"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_STATIC_PULL_DOWN)->SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_READ_BATTERY"),_T("IDS_READ_BATTERY"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BUTTON_READ_BATTERY)->SetWindowText(strTxt);
    
    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_WRITE_BATTERY"),_T("IDS_WRITE_BATTERY"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BUTTON_WRITE_BATTERY)->SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_DISCHARGE"),_T("IDS_DISCHARGE"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_GROUP_DISCHARGE)->SetWindowText(strTxt);
    
    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDS_CHARGE"),_T("IDS_CHARGE"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_GROUP_CHARGE)->SetWindowText(strTxt);

    GetPrivateProfileStringW(_T("BatteryParam"),_T("IDC_BTN_READ_BATTERY_DATA"),_T("IDC_BTN_READ_BATTERY_DATA"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    GetDlgItem(IDC_BTN_READ_BATTERY_DATA)->SetWindowText(strTxt);

    strTxt.ReleaseBuffer(MAX_LENGTH);
}

BOOL CDlgBatteryParam::GetParamFromInterface( STRUCT_BATTERY_PARAM* pBatteryParam )
{
	UpdateData(TRUE);
	if (m_batteryCell==-1)
	{
		return FALSE;
	}
	else
		pBatteryParam->dwCell = m_batteryCell;
	CString strValue;
	int nValue;
	GetDlgItem(IDC_EDIT_PULL_UP)->GetWindowText(strValue);
	if (strValue.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		strValue.TrimLeft();
		strValue.TrimRight();
		if (strValue==_T("0"))
		{
			pBatteryParam->dwPullUp = 0;
		}
		else
		{
			nValue = _ttoi((LPTSTR)(LPCTSTR)strValue);
			if (nValue<=0)
			{
				return FALSE;
			}
			pBatteryParam->dwPullUp = nValue;

		}
	}
	GetDlgItem(IDC_EDIT_PULL_DOWN)->GetWindowText(strValue);
	if (strValue.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		strValue.TrimLeft();
		strValue.TrimRight();
		if (strValue==_T("0"))
		{
			pBatteryParam->dwPullDown = 0;
		}
		else
		{
			nValue = _ttoi((LPTSTR)(LPCTSTR)strValue);
			if (nValue<=0)
			{
				return FALSE;
			}
			pBatteryParam->dwPullDown = nValue;
			
		}
	}
	int i;
	for (i=0;i<11;i++)
	{
		GetDlgItem(IDC_EDIT_DISCHARGE_1+i)->GetWindowText(strValue);
		if (strValue.IsEmpty())
		{
			return FALSE;
		}
		else
		{
			strValue.TrimLeft();
			strValue.TrimRight();
			if (strValue==_T("0"))
			{
				pBatteryParam->dwDischarge[i] = 0;
			}
			else
			{
				nValue = _ttoi((LPTSTR)(LPCTSTR)strValue);
				if (nValue<=0)
				{
					return FALSE;
				}
				pBatteryParam->dwDischarge[i] = nValue;
				
			}
		}
	}
	for (i=0;i<11;i++)
	{
		GetDlgItem(IDC_EDIT_CHARGE_1+i)->GetWindowText(strValue);
		if (strValue.IsEmpty())
		{
			return FALSE;
		}
		else
		{
			strValue.TrimLeft();
			strValue.TrimRight();
			if (strValue==_T("0"))
			{
				pBatteryParam->dwCharge[i] = 0;
			}
			else
			{
				nValue = _ttoi((LPTSTR)(LPCTSTR)strValue);
				if (nValue<=0)
				{
					return FALSE;
				}
				pBatteryParam->dwCharge[i] = nValue;
				
			}
		}
	}
	return TRUE;
}

int CDlgBatteryParam::WriteBatteryParam( CString strResourceDtsPath, STRUCT_BATTERY_PARAM* pBatteryParam )
{
	CString str;
	POSITION pos,posback;
	CString str_find,strTemp;
	int count=0;
	int mstart=0,mEnd=0;
	int Num=0,NumMax=0;
	int i=0;
	BOOL isExistBat=FALSE;
	
	CFile cFile;
	int nSrcSize;
	char *pSrcBuf=NULL;
	//strResourceDtsPath=strResourceDtsPath+_T("xxxx");
	if(cFile.Open(strResourceDtsPath, CFile::modeWrite|CFile::modeCreate)){
		count=ResourceList.GetCount();
		for(pos=ResourceList.GetHeadPosition();pos;)
		{
			str_find = ResourceList.GetNext(pos);
			if(str_find.Find(_T("bat_table"))>=0)
			{
				posback=pos;
				while(Num<20){
					//TRACE1("%s\n",str_find);
					mEnd=str_find.Find(_T(">"));
					if(mEnd>=0)
						break;
					str_find=ResourceList.GetNext(pos);
					Num++;
				}
				
				NumMax=Num;
				if(Num>=20){
					cFile.Close();
					return -2;
				}
	
				str_find.Format(_T("bat_table = <%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d>;"), 
					0,0,0,pBatteryParam->dwCell,pBatteryParam->dwPullUp,pBatteryParam->dwPullDown,
					pBatteryParam->dwDischarge[0],pBatteryParam->dwDischarge[1],pBatteryParam->dwDischarge[2],pBatteryParam->dwDischarge[3],pBatteryParam->dwDischarge[4],pBatteryParam->dwDischarge[5],
					pBatteryParam->dwDischarge[6],pBatteryParam->dwDischarge[7],pBatteryParam->dwDischarge[8],pBatteryParam->dwDischarge[9],pBatteryParam->dwDischarge[10],
					pBatteryParam->dwCharge[0],pBatteryParam->dwCharge[1],pBatteryParam->dwCharge[2],pBatteryParam->dwCharge[3],pBatteryParam->dwCharge[4],pBatteryParam->dwCharge[5],
					pBatteryParam->dwCharge[6],pBatteryParam->dwCharge[7],pBatteryParam->dwCharge[8],pBatteryParam->dwCharge[9],pBatteryParam->dwCharge[10]);
				
				ResourceList.GetNext(posback);
				isExistBat=TRUE;
			}
			
			if((Num>0)&&(Num<NumMax)){
				str_find.Empty();
				Num--;
			}

			str_find += _T("\n");
			
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

		if(!isExistBat)
			return -1;

	}
	else{
		return -2;
	}


	CString strCmd,strTxt;
	int iRet=0;
	int size=0;
	CString strResult;
	CString g_strCovert = strRootPath +_T("\\bin\\dtstodtb.bat");
	strCmd = CMySpawnConsumer::CovertDtstoDtb(g_strCovert);
	if (!RunOhterExe(strCmd,&strResult))
	{
		MessageBox(strResult);
		return -3;
	}	
	
	size=GetDtbFileSize(strResourceDtsPath);
	if(size < 0)
		g_pPluginLogObject->Record(_T("open %s failed"), strResourceDtbPath);
	
	iRet=resource_importDtb(strResourcePath,strResourceDtbPath);
	if(iRet<0)
	{
		g_pPluginLogObject->Record(_T("Import  failed :%d"), iRet);
		return -4;
	}
		
	return 0;
}

BOOL CDlgBatteryParam::SaveKernel( BYTE *pKernel,UINT uiKernelSize,CString strKernelPath )
{
    CString strTmpKernelFile;
    PUINT puiCrc;
    GetModuleFileName(NULL, strTmpKernelFile.GetBuffer(MAX_PATH), MAX_PATH);
    strTmpKernelFile.ReleaseBuffer();
    strTmpKernelFile = strTmpKernelFile.Left(strTmpKernelFile.ReverseFind(_T('\\'))+1);
    strTmpKernelFile += _T("tmp_file.bin");
    CFile file;
    BOOL bRet;
    bRet = file.Open(strTmpKernelFile,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);
    if (!bRet)
    {
        return FALSE;
    }
    try
    {
        puiCrc = (PUINT)(pKernel+uiKernelSize-4);
        *puiCrc = CRC_32(pKernel+8,uiKernelSize-12);
        file.WriteHuge(pKernel,uiKernelSize);
    }
    catch (...)
    {
        file.Close();
        return FALSE;
    }
    file.Close();
    bRet = CopyFile(strTmpKernelFile,strKernelPath,FALSE);
    if (bRet)
    {
        DeleteFile(strTmpKernelFile);
    }
	return bRet;
}

void CDlgBatteryParam::OnButtonWriteBattery() 
{
	WirteBatteryParameters();
}

void CDlgBatteryParam::OnBtnReadBatteryData() 
{
	char *pBuffer = NULL;
	char * pfileBuffer=NULL;
    char text[10];
    int index = 0;
    LPWSTR lpDir = NULL;
    CString str;
    int nSize = 0;
    int Postion = 0;
    
    
    CFileDialog fileopenbox(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Battery log infromation|*.log||"));
    fileopenbox.m_ofn.lpstrTitle = _T("´ò¿ª");
    if(fileopenbox.DoModal() == IDOK)
    {
        pfileBuffer = read_file(fileopenbox.GetFileName(),0);
		pBuffer = pfileBuffer;
        memset(text,0,sizeof(text));
        Postion = 0;
        if (!pBuffer) {MessageBox(_T("battery log read error!"),MB_OK);}
        else
        {
            index = 0;
            while(*pBuffer)
            {
                if((*pBuffer>='0')&&(*pBuffer<='9'))
                {
                    text[index] = *pBuffer;
                    index++;
                    pBuffer++;
                }else
                {
                    text[index] = 0;
                    index = 0;
                    nSize = strlen(text);
                    AnsiToUnicode(lpDir,nSize,text);
                    str = lpDir;
					delete[] lpDir;
					lpDir = NULL;
                    switch(Postion)
                    {
                    case 0:  if(!str.CompareNoCase(_T("0")))
                             {
                                 ((CButton*)GetDlgItem(IDC_RADIO_CELL_ONE))->SetCheck(true);
                                 ((CButton*)GetDlgItem(IDC_RADIO_CELL_TWO))->SetCheck(false);
                             }else
                             {
                                 ((CButton*)GetDlgItem(IDC_RADIO_CELL_ONE))->SetCheck(false);
                                 ((CButton*)GetDlgItem(IDC_RADIO_CELL_TWO))->SetCheck(true);
                             }
                             break;
                             
                    case 1:	  SetDlgItemText(IDC_EDIT_PULL_UP,str); break;
                        
                    case 2:   SetDlgItemText(IDC_EDIT_PULL_DOWN,str); break;
                        
                    case 3:   SetDlgItemText(IDC_EDIT_DISCHARGE_1,str); break;
                    case 4:   SetDlgItemText(IDC_EDIT_DISCHARGE_2,str); break;
                    case 5:	  SetDlgItemText(IDC_EDIT_DISCHARGE_3,str); break;
                    case 6:	  SetDlgItemText(IDC_EDIT_DISCHARGE_4,str); break;
                    case 7:	  SetDlgItemText(IDC_EDIT_DISCHARGE_5,str); break;
                    case 8:	  SetDlgItemText(IDC_EDIT_DISCHARGE_6,str); break;
                    case 9:	  SetDlgItemText(IDC_EDIT_DISCHARGE_7,str); break;
                    case 10:  SetDlgItemText(IDC_EDIT_DISCHARGE_8,str); break;
                    case 11:  SetDlgItemText(IDC_EDIT_DISCHARGE_9,str); break;
                    case 12:  SetDlgItemText(IDC_EDIT_DISCHARGE_10,str); break;
                    case 13:  SetDlgItemText(IDC_EDIT_DISCHARGE_11,str); break;
                    case 14:  SetDlgItemText(IDC_EDIT_CHARGE_1,str); break;
                    case 15:  SetDlgItemText(IDC_EDIT_CHARGE_2,str); break;
                    case 16:  SetDlgItemText(IDC_EDIT_CHARGE_3,str); break;
                    case 17:  SetDlgItemText(IDC_EDIT_CHARGE_4,str); break;
                    case 18:  SetDlgItemText(IDC_EDIT_CHARGE_5,str); break;
                    case 19:  SetDlgItemText(IDC_EDIT_CHARGE_6,str); break;
                    case 20:  SetDlgItemText(IDC_EDIT_CHARGE_7,str); break;
                    case 21:  SetDlgItemText(IDC_EDIT_CHARGE_8,str); break;
                    case 22:  SetDlgItemText(IDC_EDIT_CHARGE_9,str); break;
                    case 23:  SetDlgItemText(IDC_EDIT_CHARGE_10,str); break;
                    case 24:  SetDlgItemText(IDC_EDIT_CHARGE_11,str); break;
                        
                    default: break;
                    }
                    //str.Format(_T("%s"),content_buf);
                    //SetDlgItemText(IDC_EDIT_DISCHARGE_1,lpDir);
                    //MessageBox(lpDir,MB_OK);
                    
                    memset(text,0,sizeof(text));
                    pBuffer++;
                    Postion++;
                }
                
                //default: break;
            }            
        }        
	}
	if(pfileBuffer)
	{
		free(pfileBuffer);
	}
}

char * CDlgBatteryParam::read_file( CString fn, unsigned *_sz )
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
    data[sz] = '\n';
    data[sz+1] = 0;
    if(_sz) *_sz = sz;
    return data;
    
oops:
    fclose(fd);
    if(data != 0) free(data);
    return 0;
}

void CDlgBatteryParam::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::EndDialog(0);

}

void CDlgBatteryParam::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	 delete(this);
}
