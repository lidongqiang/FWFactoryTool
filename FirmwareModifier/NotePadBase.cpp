// NotePadBase.cpp : implementation file
//

#include "stdafx.h"
#include "FirmwareModifier.h"
#include "NotePadBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotePadBase dialog


CNotePadBase::CNotePadBase(CWnd* pParent /*=NULL*/, const CString& strFilePath)
	: CDialog(CNotePadBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotePadBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_strFilePath = strFilePath;
}


void CNotePadBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotePadBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotePadBase, CDialog)
	//{{AFX_MSG_MAP(CNotePadBase)
	ON_WM_CLOSE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotePadBase message handlers

BOOL CNotePadBase::OnInitDialog() 
{
	CDialog::OnInitDialog();

    GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_TITLE"),_T("MESSAGE_TITLE"),
        m_strMsgBoxTitle.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);

	TRACE1("%s",m_strFilePath);
	// TODO: Add extra initialization here
	if (!m_strFilePath.IsEmpty())
    {
        CString str;
		POSITION pos;
		CString str_find;
		CString strDisplay;
		CString strTemp;
		int count=0;
  
		((CEdit*)GetDlgItem(IDC_EDIT))->SetLimitText(1024*1024);
        NotePadReadFile(m_strFilePath);	
		for(pos=NotePadList.GetHeadPosition();pos;)
		{
			str_find = NotePadList.GetNext(pos);
			//GetDlgItem(IDC_EDIT)->GetWindowText(strDisplay);
			//strTemp=strDisplay+str_find+_T("\r\n");
			strTemp=strTemp+str_find+_T("\r\n");

		}
		GetDlgItem(IDC_EDIT)->SetWindowText(strTemp);
    }

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNotePadBase::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
    delete(this);
}

void CNotePadBase::OnClose() 
{
    CString strTxt;

    GetPrivateProfileStringW(_T("Prompt"),_T("MESSAGE_CONFIRM_SAVE"),_T("MESSAGE_CONFIRM_SAVE"),
        strTxt.GetBuffer(MAX_LENGTH),MAX_LENGTH, g_strLanguageConfigFilePath);
    int iRes = MessageBox(strTxt, m_strMsgBoxTitle, MB_YESNO | MB_ICONWARNING);
    if( iRes == IDYES )
    {
        OnSaveFile();
    }
	CDialog::EndDialog(0);
}

// 返回文件的编码格式
BOOL CNotePadBase::GetFileEncode( LPCTSTR lpszFileName, emFileEncode& fileEncode )
{
    BOOL bRet = FALSE;
    
    try
    {
        CFile cFile;
        if( !cFile.Open(lpszFileName,CFile::modeRead ) )
            return FALSE;
        
        int nFileLen = cFile.GetLength();
        if ( nFileLen <2 )
        {
            // ANSI 和utf8 编码的文件没有特殊的文件头
            fileEncode = FILE_ANSI;
        }
        else
        {
            char *pBytes = new char[nFileLen]; 
            cFile.Read(pBytes , nFileLen); 
            
            // ucs-2 little endian 编码的文件的头两个字节是"fffe"
            // ucs-2 big endian 编码的文件的头两个字节是"feff"
            if((char)pBytes[0] == (char)0x0FF&& (char)pBytes[1] == (char)0x0FE)
                fileEncode = FILE_UCS2_L;
            else if ((char)pBytes[1] == (char)0x0FF&& (char)pBytes[0] == (char)0x0FE)
                fileEncode = FILE_UCS2_B;
            else
            {
                if (nFileLen >= 3 &&
                    (char)pBytes[0] == (char)0xef&&
                    (char)pBytes[1] == (char)0xbb&&
                    (char)pBytes[2] == (char)0xbf)
                    fileEncode = FILE_UTF8;
                else
                    fileEncode = FILE_ANSI;
            }
            
            delete pBytes;
            pBytes = NULL;
            
        }
        
        cFile.Close();
        bRet = TRUE;
    }
    catch(CException e)
    {
        
    }
    
    return bRet;
}

char *NotePadReadfile(CString fn, unsigned *_sz)
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

BOOL CNotePadBase::NotePadReadFile( LPCTSTR pszFilePath)
{
	char *pBuffer;
	char *x;
	char *content_buf;//[288*1024 +1];
	char loop = 1;
	int  content_len = 0;
	CString str, strTxt;	
	LPWSTR lpDir = NULL;
	int nSize;
	NotePadList.RemoveAll();

	pBuffer = NotePadReadfile(pszFilePath, 0);
	content_buf = (char*)malloc(strlen(pBuffer)+1);
	memset(content_buf,0,strlen(pBuffer)+1);
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
				NotePadList.AddTail(strTxt);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
				content_len = 0;
				memset(content_buf,0,strlen(pBuffer)+1);
				break;
			case  0:                
				loop = 0;
				nSize = strlen(content_buf);
				//AnsiToUnicode(lpDir,nSize,content_buf);
				U8ToUnicode(lpDir,nSize,content_buf);
				strTxt = lpDir;
				NotePadList.AddTail(strTxt);
				if (lpDir)
				{
					delete []lpDir;
					lpDir = NULL;
				}
				content_len = 0;
				memset(content_buf,0,strlen(pBuffer)+1);
				break;
			default: content_buf[content_len] = *x++;
				content_len++;
				break;
			}
		}
	}
	if(pBuffer)
		free(pBuffer);
	 if(content_buf) 
		 free(content_buf);
	return 0;
}
// 读 lpszFilePath 指定的文件，把一行文本作为一个 wstring 保存到vector中
/*
BOOL CNotePadBase::ReadFile( LPCTSTR pszFilePath,vector<wstring>& vLines )
{
	CString strLog;
	
	emFileEncode fileEncode;
	if (!GetFileEncode(pszFilePath,fileEncode))
	{
	
		strLog.Format(_T("ReadFile 获取文件编码格式失败 文件路径：%s")
			,pszFilePath);
		MessageBox(strLog);

		return FALSE;
	}
	strLog.Format(_T("ReadFile 获取文件编码格式为%d 文件路径：%s"),fileEncode, pszFilePath);

	CFile cFile; 
	if(!cFile.Open(pszFilePath , CFile::modeRead|CFile::typeBinary , NULL))
		return FALSE;

	// 空文件
	int nFileLen = cFile.GetLength(); 
	if(0 == nFileLen)
	{
		cFile.Close();
		return TRUE;
	}

	BOOL bRet = TRUE;

	if (FILE_UCS2_L  == fileEncode)// 一般默认的utf16编码都是ucs-2 little endian
	{
		ASSERT(nFileLen >= 2);

		char *pBytes = new char[nFileLen]; 
		cFile.Read(pBytes , nFileLen); 

		const WCHAR wcEnter   = (WCHAR)0x000d;//回车符
		const WCHAR wcNewLine = (WCHAR)0x000a;//换行符

		int nIndex = 2;//跳过FFFE
		WCHAR* pCurPos = (WCHAR*)&pBytes[nIndex];
		wstring wstrLines;

		while(nIndex < nFileLen)
		{	
			WCHAR wcCurChar = *pCurPos;//当前指向的宽字符
			switch(wcCurChar)//判断
			{
			case wcEnter://后面一定有换行，不处理
				break;
			case wcNewLine://换行
				{
					vLines.push_back(wstrLines);
					wstrLines.erase(0,wstrLines.size());
				}
				break;
			default:
				wstrLines += wcCurChar;
			}
			pCurPos++;
			nIndex += 2;
		}
		if (wstrLines.size() > 0)
		{
			vLines.push_back(wstrLines);
			wstrLines.erase(0,wstrLines.size());
		}
		delete pBytes;
		pBytes = NULL;
	}
	if (FILE_ANSI == fileEncode)
	{
			
		char *pBytes = new char[nFileLen]; 
		cFile.Read(pBytes , nFileLen); 		
		
		const char cEnter   = (WCHAR)0x0d;//回车符
		const char cNewLine = (WCHAR)0x0a;//换行符
		
		int nIndex = 0;//跳过FFFE
		char* pCurPos = (char*)&pBytes[nIndex];

		string strLines;
		while(nIndex < nFileLen)
		{
					
			char cCurChar = *pCurPos;//当前指向的宽字符
			switch(cCurChar)//判断
			{
			case cEnter://后面一定有换行，不处理
				break;
			case cNewLine://换行
				{
					USES_CONVERSION;
					wstring wstrLines = A2W(strLines.c_str());
					vLines.push_back(wstrLines);
					strLines.erase(0,strLines.size());
				}
				break;
			default:
				strLines += cCurChar;
			}
			pCurPos++;
			nIndex +=1;
			
		}

		//最后一行
		if (strLines.size() > 0)
		{
			USES_CONVERSION;
			wstring wstrLines = A2W(strLines.c_str());
			vLines.push_back(wstrLines);
			strLines.erase(0,strLines.size());
		}
		
		delete pBytes;
		pBytes = NULL;
	}
	if (FILE_UCS2_B == fileEncode )
	{
		bRet = FALSE;
	}
	if (FILE_UTF8 == fileEncode)
	{
		bRet = FALSE;
	}

	cFile.Close();	
	return bRet;
}
*/
/*
void CNotePadBase::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HWND hFocus;//Edit或RichEdit的句柄
	int LineNum;
	((CEdit*)GetDlgItem(IDC_EDIT))->SetFocus();
	LineNum=((CEdit*)GetDlgItem(IDC_EDIT))->LineIndex();
	/*
	int LineCount=	((CEdit*)GetDlgItem(IDC_EDIT))->GetLineCount();
	int LineIndexNow=((CEdit*)GetDlgItem(IDC_EDIT))->Get();
	for(int i=0;i<LineCount;i++)
	{
		int LineIndex = ::SendMessage(hFocus,EM_LINEINDEX,((WPARAM)i),0);
		if(LineIndex == LineIndexNow)
		{
			LineNum=i;
			break;
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}
*/

BOOL CNotePadBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
