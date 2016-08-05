#include "stdafx.h"
#include "MySpawnConsumer.h"

CMySpawnConsumer::CMySpawnConsumer() 
{
	m_pSpawn = NULL;
}


CString CMySpawnConsumer::PackUnionCmd(CString strExe, CString strFwType,CString strOldBoot,CString strOldFw, CString strNewFw)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\" ") + strFwType + 
		     _T(" \"") + strOldBoot + _T("\"") +
			 _T(" \"") + strOldFw + _T("\"") +
			 _T(" \"") + strNewFw + _T("\"") +
			 _T(" -os_type:ANDROIDOS");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("PackUnionCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::UnpackUnionCmd(CString strExe, CString strFwPath, CString strOutputDir)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\" -unpack") + 
		_T(" \"") + strFwPath + _T("\"") +
		_T(" \"") + strOutputDir + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("UnpackUnionCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::PackAndroidCmd(CString strExe, CString strSrcPath, CString strOutputFile)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\" -pack") + 
		_T(" \"") + strSrcPath + _T("\"") +
		_T(" \"") + strOutputFile + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("PackAndroidCmd:%s\r\n"),strCmd);
	return strCmd;
}

CString CMySpawnConsumer::UnpackAndroidCmd(CString strExe, CString strSrcPath, CString strOutputDir)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\" -unpack") + 
		_T(" \"") + strSrcPath + _T("\"") +
		_T(" \"") + strOutputDir + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("UnpackAndroidCmd:%s\r\n"),strCmd);
	return strCmd;
}

CString CMySpawnConsumer::PackCramfs(CString strExe, CString strSrcPath, CString strOutputFile)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"") + 
		_T(" \"") + strSrcPath + _T("\"") +
		_T(" \"") + strOutputFile + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("PackCramfs:%s\r\n"),strCmd);
	return strCmd;
}

CString CMySpawnConsumer::UnpackCramfsCmd(CString strExe, CString strSrcPath, CString strOutputDir)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"") + 
		_T(" -x \"") + strOutputDir + _T("\"") +
		_T(" \"") + strSrcPath + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("UnpackCramfsCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::UnpackBootCmd(CString strExe,CString strParam,CString strBoot)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"") + 
		_T(" \"") + strParam + _T("\"") +
		_T(" \"") + strBoot + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("UnpackBootCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::UnpackExtCmd(CString strExe,CString strSrcPath,CString strOutputDir)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"") + 
		_T(" -x \"") + strSrcPath + _T("\"") +
		_T(" \"") + strOutputDir + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("UnpackExtCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::mkkrnlimgCmd(CString strExe,CString strSrcFile,CString strDstFile)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"") + 
		_T(" \"") + strSrcFile + _T("\"") +
		_T(" \"") + strDstFile + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("mkkrnlimgCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::mkBootImgCmd(CString strExe,CString strKernel,CString strRamdisk,CString strResource,CString strOutputFile)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"") + 
		_T(" --kernel \"") + strKernel + _T("\"") +
		_T(" --ramdisk \"") + strRamdisk + _T("\"")+
		_T(" --second \"") + strResource + _T("\"")+
		_T(" --output \"") + strOutputFile + _T("\"");
//	strCmd = _T("\"")+strCmd+_T("\"");
	TRACE(_T("mkBootImgCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::UnpackRecoveryCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("UnpackRecoveryCmd:%s\r\n"),strCmd);
	return strCmd;
}

CString CMySpawnConsumer::UnpackKernelRecoveryCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("UnpackKernelRecoveryCmd:%s\r\n"),strCmd);
	return strCmd;
}

CString CMySpawnConsumer::PackRecoveryCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("PackRecoveryCmd:%s\r\n"),strCmd);
	return strCmd;
}

CString CMySpawnConsumer::PackKernelRecoveryCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("PackKernelRecoveryCmd:%s\r\n"),strCmd);
	return strCmd;
}

CString CMySpawnConsumer::UnpackBootImgCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("UnpackBootImgCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::UnpackRamdiskCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("UnpackRamdiskCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::ReplaceBootLogoCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("ReplaceBootLogoCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::ReplaceRamdiskLogoCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("ReplaceRamdiskLogoCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::PackBootImgCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("PackBootImgCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::PackRamdiskCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("PackRamdiskCmd:%s\r\n"),strCmd);
	return strCmd;
}
CString CMySpawnConsumer::PackExtCmd(CString strExe)
{
	CString strCmd;
	strCmd = _T("\"") + strExe + _T("\"");
	
	TRACE(_T("PackExtCmd:%s\r\n"),strCmd);
	return strCmd;
}

BOOL CMySpawnConsumer::GetBatCmdResult(tstring &strInfo)
{
	BOOL bResult=FALSE;
	if (strInfo.size()<=0)
	{
		return FALSE;
	}
	tstringstream stream(strInfo);
	int iReceiveCount=0;
	tstring strLine;
	tstring::size_type pos,line_size;
	
	while(!stream.eof())
	{
		getline(stream,strLine);
		line_size = strLine.size();
		if (strLine[line_size-1]==_T('\r'))
		{
			strLine = strLine.substr(0,line_size-1);
		}
		
		pos = strLine.find(_T("unexpected end of file"));
		if( pos != tstring::npos)
		{
			bResult = false;
			break;
		}

		pos =strLine.find(_T("Info:"));
		if (pos!=tstring::npos)
		{
			pos = strLine.find(_T("success"));

			if (pos!=tstring::npos)
			{
				bResult = TRUE;
				break;
			}
		}
		pos =strLine.find(_T("Error:"));
		if (pos!=tstring::npos)
		{
			break;
		}
	}
	
	return bResult;
}

BOOL CMySpawnConsumer::GetSpawnData(PBYTE &pData)
{
	if (m_pSpawn==NULL)
	{
		return FALSE;
	}
	BOOL bRet=FALSE;
	DWORD dwSize;
	pData = NULL;
	bRet = m_pSpawn->GetOutputData(NULL,dwSize);
	if (bRet)
	{
		if (dwSize>0)
		{
			pData = new BYTE[dwSize];
			if (pData)
			{
				bRet = m_pSpawn->GetOutputData(pData,dwSize);
			}
		}
		else
			bRet = FALSE;
	}
	return bRet;
}
BOOL CMySpawnConsumer::SpawnDataToString(tstring &strDst,PBYTE pSrc,ENUM_STRING_CODE emCode)
{
	if (!pSrc)
	{
		return FALSE;
	}
	BOOL bRet=FALSE;
	LPTSTR lpDst;
	int nSize;
	if (emCode==SC_UNICODE)
	{
		strDst = (PWCHAR)pSrc;
		bRet = TRUE;
	}
	else if (emCode==SC_ANSI)
	{
		bRet =  cmStrCode::AnsiToUnicode(lpDst,nSize,(PCHAR)pSrc);
		if (bRet)
		{
			strDst = lpDst;
			delete []lpDst;
		}
	}
	else if (emCode==SC_UTF8)
	{
		bRet =  cmStrCode::Utf8ToUnicode(lpDst,nSize,(PCHAR)pSrc);
		if (bRet)
		{
			strDst = lpDst;
			delete []lpDst;
		}
	}
	return bRet;
}
VOID CMySpawnConsumer::ReleaseSpawnData(PBYTE pData)
{
	if (pData)
	{
		delete []pData;
	}
}

VOID CMySpawnConsumer::SetSpawnObject(cmSpawn *pSpawn)
{
	if (pSpawn)
	{
		m_pSpawn = pSpawn;
	}
}
BOOL CMySpawnConsumer::GetSpawnString(tstring &strSpawnOutput,ENUM_STRING_CODE emCode)
{
	BOOL bRet;
	PBYTE pSpawnData;
	bRet = GetSpawnData(pSpawnData);
	if (bRet)
	{
		bRet = SpawnDataToString(strSpawnOutput,pSpawnData,emCode);
		ReleaseSpawnData(pSpawnData);
	}
	return bRet;
}