#ifndef MYSPAWNCONSUMER_HEADER
#define MYSPAWNCONSUMER_HEADER

#include "cmSpawn.h"
#include "cmStrCode.h"
using namespace cm;
enum ENUM_STRING_CODE{SC_ANSI=0,SC_UNICODE,SC_UTF8};
#if defined(_UNICODE) || defined(UNICODE)
typedef wstring	tstring;
typedef wstringstream tstringstream;
#else
typedef string	tstring;
typedef stringstream tstringstream;
#endif

class CMySpawnConsumer
{
public:
	CMySpawnConsumer();
	static CString PackUnionCmd(CString strExe, CString strFwType,CString strOldBoot,CString strOldFw, CString strNewFw);
	static CString UnpackUnionCmd(CString strExe, CString strFwPath, CString strOutputDir);
	static CString PackAndroidCmd(CString strExe, CString strSrcPath, CString strOutputFile);
	static CString UnpackAndroidCmd(CString strExe, CString strSrcPath, CString strOutputDir);
	static CString PackCramfs(CString strExe, CString strSrcPath, CString strOutputFile);
	static CString UnpackCramfsCmd(CString strExe, CString strSrcPath, CString strOutputDir);

	static CString UnpackBootImgCmd(CString strExe);
	static CString UnpackRamdiskCmd(CString strExe);
	static CString ReplaceBootLogoCmd(CString strExe);
	static CString ReplaceRamdiskLogoCmd(CString strExe);
	static CString PackBootImgCmd(CString strExe);
	static CString PackRamdiskCmd(CString strExe);
	
	static CString UnpackRecoveryCmd(CString strExe);
	static CString UnpackKernelRecoveryCmd(CString strExe);
	static CString PackRecoveryCmd(CString strExe);
	static CString PackKernelRecoveryCmd(CString strExe);


	static CString UnpackBootCmd(CString strExe,CString strParam,CString strBoot);
	static CString UnpackExtCmd(CString strExe,CString strSrcPath,CString strOutputDir);
	static CString PackExtCmd(CString strExe);
	static CString mkkrnlimgCmd(CString strExe,CString strSrcFile,CString strDstFile);
	static CString mkBootImgCmd(CString strExe,CString strKernel,CString strRamdisk,CString strResource,CString strOutputFile);
	BOOL GetBatCmdResult(tstring &strInfo);
	VOID SetSpawnObject(cmSpawn *pSpawn);
	BOOL GetSpawnString(tstring &strSpawnOutput,ENUM_STRING_CODE emCode=SC_ANSI);
private:
	 BOOL GetSpawnData(PBYTE &pData);
	 BOOL SpawnDataToString(tstring &strDst,PBYTE pSrc,ENUM_STRING_CODE emCode);
	 VOID ReleaseSpawnData(PBYTE pData);
	
private:
	cmSpawn *m_pSpawn;
};
#endif