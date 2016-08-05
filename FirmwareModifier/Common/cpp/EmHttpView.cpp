#include "EmHttpView.h"
#include "EmHttp.h"
#include "EmHttpBase.h"
#include "EmHttpTextResponse.h"
#include "EmStlStr.h"


using namespace em;

int em::EmHttpView::Post( std::string &strResult,const char* szUrl, const char* szData
						 , EmEncode eEncode /*= EmEncodeUtf8*/, int iMaxResult /* = 5000 */, int iTimeout/*=3000*/ )
{
	int iResult = 0;
	EmHttpTextResponse xResponse;
	EM_RETURN_IF_NEGATIVE(iResult,EmHttp::PostTextByUrl(xResponse,szUrl,szData,eEncode,iMaxResult,iTimeout));
	strResult = xResponse.GetContent();
	return 0;
	
}

int em::EmHttpView::Get( std::string &strResult,const char* szUrl
						, EmEncode eEncode /*= EmEncodeUtf8*/, int iMaxResult /* = 5000 */, int iTimeout/*=3000*/ )
{
	int iResult = 0;
	EmHttpTextResponse xResponse;
	EM_RETURN_IF_NEGATIVE(iResult,EmHttp::GetTextByUrl(xResponse,szUrl,eEncode,iMaxResult,iTimeout));
	strResult = xResponse.GetContent();
	return 0;
}
