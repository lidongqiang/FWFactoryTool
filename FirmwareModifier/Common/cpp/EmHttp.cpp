#include "EmHttp.h"
#include "EmHttpBase.h"

#include "EmFile.h"
#include "EmStlStr.h"
#include "EmNet.h"
#include "EmHttpFileResponse.h"
#include "EmHttpTextResponse.h"
#include "IEmNetStreamReceiver.h"
#include "IEmNetStreamSender.h"
#include <EmStr.h>


using namespace em;

int em::EmHttp::GetTextByUrl( EmHttpTextResponse &rResponse, const char* szUrl
							 , EmEncode enc /*= EmEncodeUtf8*/, int iReadLimit /*=4096*/, int iTimeoutMilli /*= 3000*/ )
{
	if(strlen(szUrl) == 0)
	{
		return EM_ERR_NET_INVALID_URL;
	}
	
	std::string strRequest = EmHttpBase::MakeGetString(szUrl);

	std::string strHost = EmStlStr::UrlGetHost(szUrl);
	unsigned short nPort = EmStlStr::UrlGetPort(szUrl);
	std::string strResponseAll = EmNet::HandyLinkStr(strHost.c_str(),nPort
		,strRequest.c_str()
		,iReadLimit,strRequest.length()
		,enc);
	
	if(strResponseAll.length() == 0)
	{
		return EM_ERR_NET_NO_RESPONSE;
	}
	
	int iBrPos = EmStlStr::Find(strResponseAll.c_str(),"\r\n\r\n");
	if(iBrPos == -1)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}

	int iSpacePos1 = EmStlStr::FindFrom(strResponseAll.c_str()," ",0);
	if(iSpacePos1 == -1)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}

	int iSpacePos2 = EmStlStr::FindFrom(strResponseAll.c_str()," ",iSpacePos1 + 1);
	if(iSpacePos2 == -1)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}

	int iStatusCode = EmStlStr::Str2Int( EmStlStr::SubStr(strResponseAll.c_str(),iSpacePos1 + 1,iSpacePos2 - iSpacePos1 - 1).c_str() );
	
	if(iStatusCode == 0)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}
	
	
	rResponse.SetStatusCode(iStatusCode);
	rResponse.SetHead( EmStlStr::SubStr(strResponseAll.c_str(),0,iBrPos).c_str() );
	rResponse.SetContent( EmStlStr::SubStr(strResponseAll.c_str(),iBrPos + 4,-1).c_str() );
	
	return 0;
}



int em::EmHttp::GetFileByUrl( EmHttpFileResponse &rResponse, const char* szUrl
							 ,IEmNetStreamReceiver *pReceiver
							 
							 ,int iReadOnceSize /*= 40960*/, int iReadLimit /*=4096*/, int iTimeoutMilli /*= 3000*/ )
{
	int iResult = 0;
	
	if(strlen(szUrl) == 0)
	{
		return EM_ERR_NET_INVALID_URL;
	}
	
	if(!rResponse.HasAllocMem())
	{
		return EM_ERR_OBJ_NOT_READY;
	}

	std::string strRequest = EmHttpBase::MakeGetString(szUrl);
	
	std::string strHost = EmStlStr::UrlGetHost(szUrl);
	unsigned short nPort = EmStlStr::UrlGetPort(szUrl);

	int iSendResult = EmNet::HandyLink(strHost.c_str(),nPort
		,strRequest.c_str(),rResponse.GetTotalBuffer()
		,iReadLimit,strRequest.length()
		,NULL,pReceiver
		,EM_NET_DEFAULT_SEND_ONCE_SIZE,iReadOnceSize
		,iTimeoutMilli);
	
	if(iSendResult < 0)
	{
		
		return iSendResult;
	}
	

	rResponse.SetTotalLen(iSendResult);

	rResponse.Compute();

	return rResponse.GetContentLen();
}

int em::EmHttp::PostTextByUrl( EmHttpTextResponse &rResponse, const char* szUrl, const char* szData 
							  , EmEncode enc /*=EmEncodeUtf8*/, int iReadLimit /*=4096*/, int iTimeoutMilli /*= 3000*/ )
{
	
	if(strlen(szUrl) == 0)
	{
		return EM_ERR_NET_INVALID_URL;
	}
	
	std::string strRequest = EmHttpBase::MakePostString(szUrl,szData);
	
	std::string strHost = EmStlStr::UrlGetHost(szUrl);
	unsigned short nPort = EmStlStr::UrlGetPort(szUrl);
	std::string strResponseAll = EmNet::HandyLinkStr(strHost.c_str(),nPort
		,strRequest.c_str(),iReadLimit,strRequest.length()
		, enc,NULL,NULL,EM_NET_DEFAULT_SEND_ONCE_SIZE,EM_NET_DEFAULT_RECV_ONCE_SIZE,iTimeoutMilli);
	
	if(strResponseAll.length() == 0)
	{
		return EM_ERR_NET_NO_RESPONSE;
	}
	
	int iBrPos = EmStlStr::Find(strResponseAll.c_str(),"\r\n\r\n");
	if(iBrPos == -1)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}
	
	int iSpacePos1 = EmStlStr::FindFrom(strResponseAll.c_str()," ",0);
	if(iSpacePos1 == -1)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}
	int iSpacePos2 = EmStlStr::FindFrom(strResponseAll.c_str()," ",iSpacePos1 + 1);
	if(iSpacePos2 == -1)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}
	
	int iStatusCode = EmStlStr::Str2Int( EmStlStr::SubStr(strResponseAll.c_str(),iSpacePos1 + 1,iSpacePos2 - iSpacePos1 - 1).c_str() );
	
	if(iStatusCode == 0)
	{
		return EM_ERR_NET_INVALID_RESPONSE;
	}
	
	
	rResponse.SetStatusCode(iStatusCode);
	rResponse.SetHead(EmStlStr::SubStr(strResponseAll.c_str(),0,iBrPos).c_str());
	rResponse.SetContent(EmStlStr::SubStr(strResponseAll.c_str(),iBrPos + 4,-1).c_str());
	
	return 0;
}



int em::EmHttp::GetFileSizeByUrl( const char* szUrl, int iTimeoutMilli /*= 3000*/ )
{

	int iResult = 0;
	EmHttpFileResponse xResponse;
	int iReadLimit = 4096;

	if(strlen(szUrl) == 0)
	{
		return EM_ERR_NET_INVALID_URL;
	}
	
	std::string strRequest;
	std::string strHost = EmStlStr::UrlGetHost(szUrl);
	unsigned short nPort = EmStlStr::UrlGetPort(szUrl);
	std::string strPathWithParam = EmStlStr::UrlGetPathWithParam(szUrl);

	std::map<std::string,std::string> mapHead;
	mapHead["Range"] = "bytes=0-0";
	strRequest = EmHttpBase::MakeGetString(szUrl,&mapHead);


	xResponse.SetRecvOnceSize(EM_NET_DEFAULT_RECV_ONCE_SIZE);
	xResponse.AllocMem(iReadLimit);
	
	int iRequestAlloc = strRequest.length() * 4 + 4;

	
	int iSendResult = EmNet::HandyLink(strHost.c_str(),nPort
		,strRequest.c_str(),xResponse.GetTotalBuffer()
		,4096, strRequest.length()
		,NULL,NULL
		,4096,4096,iTimeoutMilli);


	if(iSendResult < 0)
	{
		return iSendResult;
	}

	char szContent[1024] = {0};
	memcpy(szContent,xResponse.GetTotalBuffer(),iSendResult);
	

	xResponse.SetTotalLen(iSendResult);
	

	xResponse.Compute();


	if(xResponse.GetStatusCode() != 206)
	{

		return EM_ERR_NET_INVALID_RESPONSE;
	}


	std::map<std::string,std::string> mapResponse;
	EmHttpBase::ExtractResponseHeadMap(xResponse.GetHeadText().c_str(),&mapResponse);

	if(mapResponse.size() == 0)
	{

		return EM_ERR_NET_INVALID_RESPONSE;
	}


	std::string strRange = mapResponse["Content-Range"];
	if(strRange.length() == 0)
	{

		return EM_ERR_NET_INVALID_RESPONSE;
	}


	int iSlashPos = EmStlStr::Find(strRange.c_str(),"/");
	if(iSlashPos == -1)
	{

		return EM_ERR_NET_INVALID_RESPONSE;
	}


	std::string strSize = EmStlStr::Trim( EmStlStr::SubStr(strRange.c_str(),iSlashPos + 1,-1).c_str() );
	int iSize = EmStlStr::Str2Int(strSize.c_str());
	return iSize;
	
}


int em::EmHttp::GetFileRangeByUrl( EmHttpFileResponse &rResponse, const char* szUrl, int iStartPos, int iSize 
								  ,int iRecvOnceSize /*= 40960*/, IEmNetStreamReceiver *pReceiver /*= NULL*/, int iTimeoutMilli /*= 3000*/ )
{
	int iResult = 0;
	
	if(strlen(szUrl) == 0)
	{
		return EM_ERR_NET_INVALID_URL;
	}
	
	if(!rResponse.HasAllocMem())
	{
		return EM_ERR_OBJ_NOT_READY;
	}

	std::map<std::string,std::string> mapHead;
	std::string strBytes;
	strBytes += "bytes=";
	strBytes += EmStlStr::Int2Str(iStartPos);
	strBytes += "-";
	strBytes += EmStlStr::Int2Str(iStartPos + iSize - 1);
	mapHead["Range"] = strBytes;
	std::string strRequest = EmHttpBase::MakeGetString(szUrl,&mapHead);
	
	std::string strHost = EmStlStr::UrlGetHost(szUrl);
	unsigned short nPort = EmStlStr::UrlGetPort(szUrl);
	

	
	int iSendResult = EmNet::HandyLink(strHost.c_str(),nPort
		,strRequest.c_str(),rResponse.GetTotalBuffer()
		,iSize + 4096,strRequest.length()
		,NULL,pReceiver
		,EM_NET_DEFAULT_SEND_ONCE_SIZE,iRecvOnceSize,iTimeoutMilli);
	

	if(iSendResult < 0)
	{
		return iSendResult;
	}
	
	rResponse.SetTotalLen(iSendResult);
	rResponse.Compute();
	
	return rResponse.GetContentLen();
}

int em::EmHttp::UploadFile( const char* szUrl, const char* szFileName, const char* szFileFormName
			, EmHttpFileResponse &rResponse,  std::map<std::string,std::string> *pFormFields
			, IEmNetStreamSender *pSender /*= NULL*/, IEmNetStreamReceiver *pReceiver /*= NULL*/
			, int iSendOnceSize /*= 40960*/, int iRecvOnceSize /*= 40960*/
			, int iReadLimit /*=4096*/, int iTimeoutMilli /*=3000*/ )
{
	if(pFormFields == NULL)
	{
		return EM_ERR_MEM_NULL;
	}

	if(!rResponse.HasAllocMem())
	{
		return EM_ERR_OBJ_NOT_READY;
	}

	if(!EmFile::IsExisted(szFileName))
	{
		return EM_ERR_FILE_LOST;
	}

	if(strlen(szUrl) < 2)
	{
		return EM_ERR_NET_INVALID_URL;
	}

	int iFileSize = (int)EmFile::Size(szFileName);

	int iSendTotal = 0;
	char *bufSendTotal = (char*)malloc(iFileSize + 10240);

	std::string strBoundaryHead = "-----------------------------";
	strBoundaryHead += "7db13a41b0346";

	std::string strBoundaryContent = "-------------------------------";
	strBoundaryContent += "7db13a41b0346";

	std::string strFileShortName = EmStlStr::FileNameFromPath(szFileName);
	std::string strBeginPart;
	std::string strEndPart;

	std::map<std::string,std::string>::const_iterator it;
	for(it = pFormFields->begin(); it != pFormFields->end(); it++)
	{
		std::string strKey = it->first;
		std::string csVal = it->second;

		strBeginPart += strBoundaryContent;
		strBeginPart += "\r\n";
		strBeginPart += "Content-Disposition: form-data;";
		strBeginPart += "name=\"";
		strBeginPart += strKey;
		strBeginPart += "\"";
		strBeginPart += "\r\n\r\n";
		strBeginPart += csVal;
		strBeginPart += "\r\n";
	}

	strBeginPart += strBoundaryContent;
	strBeginPart += "\r\n";
	strBeginPart += "Content-Disposition: form-data;";
	strBeginPart += " name=\"";
	strBeginPart += szFileFormName;
	strBeginPart += "\"; filename=\"" + strFileShortName + "\"";
	strBeginPart += "\r\n";
	strBeginPart += "Content-Type: application/octet-stream";
	strBeginPart += "\r\n\r\n";

	const char *bufBeginPart = strBeginPart.c_str();
	int iBeginPartSize = strlen(bufBeginPart);

	strEndPart += "\r\n";
	strEndPart += strBoundaryContent;
	strEndPart += "--";
	strEndPart += "\r\n";
	const char *bufEndPart = strEndPart.c_str();
	int iEndPartSize = strlen(bufEndPart);

	int iContentLength = iBeginPartSize + iFileSize + iEndPartSize;

	std::string strHost = EmStlStr::UrlGetHost(szUrl);
	unsigned short nPort = EmStlStr::UrlGetPort(szUrl);
	std::string strPort = EmStlStr::Int2Str( nPort );
	std::string strPathWithParam = EmStlStr::UrlGetPathWithParam(szUrl);
	
	std::string strHead;
	strHead = "POST " + strPathWithParam + " HTTP/1.1";
	strHead += "\r\n";
	strHead += "Host:" + strHost;
	strHead += "\r\n";
	strHead += "Connection: Keep-Alive";
	strHead += "\r\n";
	strHead += "Content-Length: ";
	strHead += EmStlStr::Int2Str(iContentLength);
	strHead += "\r\n";
	strHead += "Cache-Control: no-cache";
	strHead += "\r\n";
	strHead += "Content-Type:";
	strHead += " multipart/form-data; boundary=";
	strHead += strBoundaryHead;
	strHead += "\r\n\r\n";

	const char *bufHead = strHead.c_str();
	int iHeadSize = strlen(bufHead);

	iSendTotal = iHeadSize + iContentLength;

	int iFileRead = EmFile::ReadBuffer(szFileName,bufSendTotal + iHeadSize + iBeginPartSize,iFileSize,false);
	if(iFileRead < iFileSize)
	{
		EM_FREE_NULL( bufSendTotal);
		return EM_ERR_FILE;
	}

	int iBufPos = 0;
	memcpy(bufSendTotal + iBufPos,bufHead,iHeadSize);
	iBufPos += iHeadSize;
	memcpy(bufSendTotal + iBufPos,bufBeginPart,iBeginPartSize);
	iBufPos += iBeginPartSize;
	iBufPos += iFileSize;
	memcpy(bufSendTotal + iBufPos,bufEndPart,iEndPartSize);

	int iResult = EmNet::HandyLink(strHost.c_str(),nPort
		,bufSendTotal,rResponse.GetTotalBuffer()
		,iReadLimit,iSendTotal
		,pSender,pReceiver
		,iSendOnceSize,iRecvOnceSize
		,iTimeoutMilli);

	EM_FREE_NULL( bufSendTotal);

	if(iResult < 0)
	{
		return iResult;
	}

	rResponse.SetTotalLen(iResult);
	rResponse.Compute();

	return iResult;
}
