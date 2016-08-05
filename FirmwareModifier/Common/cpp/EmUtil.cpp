#include "stdafx.h"
#include "../inc/EmUtil.h"


#include <stdlib.h>
#include <ctype.h>
#include <time.h>




bool em::EmUtil::BoolFix( int val )
{
	if(val == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool em::EmUtil::IsInvalidHandle( void* hHandle )
{
	if(hHandle == NULL || (int)hHandle == -1)
	{
		return true;
	}
	return false;
}

__int64 em::EmUtil::PowPlus( int iBase, unsigned int iEx )
{
	int i = 0;
	__int64 iRet = 1;
	for(i = 0; i < iEx; i++)
	{
		iRet = iRet * iBase;
	}
	return iRet;
}

__int64 em::EmUtil::Rand( int iBegin, int iEnd )
{
	static bool seed_inited = false;
	__int64 diff = 0;
	__int64 rand_src = 0;
	double rate = 0.0;
	__int64 result = 0;
	unsigned int ts = 0;
	int sec = 0;
	int seed = 0;
	if(!seed_inited)
	{
		ts = (unsigned int)time(NULL);
		sec = ts % 10;
		seed = (ts / 10) * sec;
		srand(seed);
		rand();
		seed_inited = true;
	}

	diff = iEnd - iBegin;
 	result = rand() % diff + 1;
	return result;
}

int em::EmUtil::HexChar2Int( char chHex )
{
	char *szHex = "0123456789abcdef";
	int i = 0;
	for(i = 0; i < 16; i++)
	{
		if(tolower(chHex) == szHex[i])
		{
			return i;
		}
	}
	return -1;
}

char em::EmUtil::Int2HexChar( int iNum )
{	
	char *szHex = "0123456789abcdef";
	if(iNum < 16 && iNum >= 0)
	{
		return szHex[iNum];
	}
	return 0;
}

int em::EmUtil::Abs( int iSrc )
{
	if(iSrc >= 0){
		return iSrc;
	}else{
		return -iSrc;
	}

}

bool em::EmUtil::IsSpaceChar( char chSrc )
{
	if(chSrc > 256){
		return false;
	}

	if(chSrc == EM_SZ_SPACE){
		return true;
	}else if(chSrc == EM_SZ_TAB){
		return true;
	}else if(chSrc == EM_SZ_RETURN){
		return true;
	}else if(chSrc == EM_SZ_LINE){
		return true;
	}

	return false;

}
