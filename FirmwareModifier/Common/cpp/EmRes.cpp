#include "stdafx.h"
#include "../inc/EmRes.h"
#include "../inc/EmTstr.h"

using namespace em;

EmRes::EmRes()
{

}

EmRes::~EmRes()
{

}

int EmRes::GetBytes( BYTE * bytes, int nResId, const char* szResType, HINSTANCE hResHandle /*=NULL*/ )
{
	HRSRC hRes = ::FindResource(hResHandle,MAKEINTRESOURCE(nResId),EmTstr::FromUtf8(szResType).Sz());
	if(hRes == NULL)
	{
		return EM_ERR_RES_FIND;
	}

	DWORD dwResSize = ::SizeofResource(hResHandle,hRes);
	BYTE* pResourceData = (BYTE*)::LoadResource(hResHandle,hRes);
	if(pResourceData == NULL)
	{
		return EM_ERR_RES_LOAD;
	}
	::FreeResource(hRes);

	return dwResSize;
}

int EmRes::GetBytesSize( int nResId, const char* szResType, HINSTANCE hResHandle /*=NULL*/ )
{
	HRESULT hr = S_FALSE;

	HRSRC hRes = ::FindResource(hResHandle,MAKEINTRESOURCE(nResId),EmTstr::FromUtf8(szResType).Sz());
	if(hRes == NULL)
	{
		return EM_ERR_RES_FIND;
	}
	DWORD dwResSize = ::SizeofResource(hResHandle,hRes);

	return dwResSize;
}
