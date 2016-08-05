#include "EmHttpBase.h"
#include "EmStlStr.h"



using namespace em;

std::string em::EmHttpBase::MakeGetString( const char* szUrl , const std::map<std::string,std::string> *pMapStr /*= NULL*/)
{

	if(szUrl == NULL || strlen(szUrl) == 0)
	{
		return "";
	}

	std::string strResult;
	std::string strUrl = szUrl;
	std::string strHost = EmStlStr::UrlGetHost(szUrl);

	std::string strPort = EmStlStr::Int2Str( EmStlStr::UrlGetPort(szUrl) );

	std::string strPathWithParam = EmStlStr::UrlGetPathWithParam(szUrl);
	

	strResult = "GET " + strPathWithParam + " HTTP/1.1";
	strResult.append("\r\n");
	strResult.append("Host:" + strHost);
	strResult.append("\r\n");
	strResult.append("Connection: Keep-Alive");
	

	if(pMapStr != NULL)
	{
		std::map<std::string,std::string>::const_iterator it;
		for(it = pMapStr->begin(); it != pMapStr->end(); it++)
		{
			std::string strKey = it->first;
			std::string strVal = it->second;
			strResult.append("\r\n");
			strResult.append(strKey);
			strResult.append(": ");
			strResult.append(strVal);
		}
	}
	

	strResult.append("\r\n\r\n");
	return strResult;
}


std::string em::EmHttpBase::MakePostString( const char* szUrl, const char* szData )
{
	if(strlen(szUrl) == 0)
	{
		return "";
	}
	
	std::string strResult;
	std::string strUrl = szUrl;
	std::string strHost = EmStlStr::UrlGetHost(szUrl);
	std::string strPort = EmStlStr::Int2Str( EmStlStr::UrlGetPort(szUrl) );
	std::string strPathWithParam = EmStlStr::UrlGetPathWithParam(szUrl);
	
	strResult = "POST " + strPathWithParam + " HTTP/1.1";
	strResult.append("\r\n");
	strResult.append("Host:" + strHost);
	strResult.append("\r\n");
	strResult.append("Connection: Keep-Alive");
	strResult.append("\r\n");
	strResult.append("Cache-Control: no-cache");
	strResult.append("\r\n");
	strResult.append("Content-Length: " + EmStlStr::Int2Str(strlen(szData)));
	strResult.append("\r\n");
	strResult.append("Content-Type: application/x-www-form-urlencoded");
	strResult.append("\r\n\r\n");
	strResult.append(szData);
	return strResult;
}

int em::EmHttpBase::ExtractResponseHeadMap( const char* szHead, std::map<std::string,std::string> *pMapStr )
{
	if(pMapStr == NULL)
	{
		return EM_ERR_MEM_NULL;
	}
	std::string strHead = EmStlStr::Trim(szHead);
	strHead = EmStlStr::Replace(strHead.c_str(),"\r\n","\n");
	int iFirstBrPos = EmStlStr::Find(strHead.c_str(),"\n");
	if(iFirstBrPos == -1)
	{
		return EM_ERR_STR_FORMAT;
	}
	
	std::string strFirstLine = EmStlStr::SubStr(strHead.c_str(),0,iFirstBrPos);
	
	int iFirstColonPos = EmStlStr::Find(strFirstLine.c_str(),":");
	if(iFirstColonPos == -1)
	{
		strHead = EmStlStr::SubStr(strHead.c_str(),iFirstBrPos + 1,-1);
	}
	
	EmStlStr::XsvToMap(*pMapStr,strHead.c_str(),"\n",":");
	
	return pMapStr->size();
}


int em::EmHttpBase::ExtractResponseStatus( const char* szHead )
{
	
	std::string strHead = EmStlStr::Trim(szHead);
	strHead = EmStlStr::Lower(strHead.c_str());
	int iHttpPos = EmStlStr::Find(strHead.c_str(),"http/");
	if(iHttpPos != 0)
	{
		return EM_ERR_STR_FORMAT;
	}
	
	int iSpacePos1 = EmStlStr::FindFrom(strHead.c_str()," ",0);
	if(iSpacePos1 == -1)
	{
		return EM_ERR_STR_FORMAT;
	}
	
	int iSpacePos2 = EmStlStr::FindFrom(strHead.c_str()," ",iSpacePos1 + 1);
	if(iSpacePos2 == -1)
	{
		return EM_ERR_STR_FORMAT;
	}
	
	std::string strStatus = EmStlStr::SubStr(strHead.c_str(),iSpacePos1 + 1,iSpacePos2 - iSpacePos1 - 1);
	strStatus = EmStlStr::Trim(strStatus.c_str());
	
	int iStatus = EmStlStr::Str2Int(strStatus.c_str());
	
	if(iStatus > 1000)
	{
		return 0;
	}
	return iStatus;
}
