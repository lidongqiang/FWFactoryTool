#ifndef _h_EmUtil
#define _h_EmUtil

#include "EmBase.h"

namespace em
{

/*
some utility functions for some basic math operation  	
*/
class EmUtil
{
public:
	static int HexChar2Int(char chHex);

	static char Int2HexChar(int iNum);

	static bool IsSpaceChar(char chSrc);

	/*
	make an int value a valid bool value
	it will turn 0 to "false"
	and turn any other number to "true"
	*/
	static bool BoolFix(int val);

	/*
	to check whether a handle is valid
	a valid handle is neither 0 nor  -1
	*/
	static bool IsInvalidHandle(void* hHandle);

	/*
	like CRT pow(int,int),but only for integer
	usage:
	int i = PowPlus(2,3);
	ASSERT(i == 8);
	*/
	static __int64 PowPlus(int iBase, unsigned int iEx);
	static int Abs(int iSrc);

	/*
	get random number between iBegin and iEnd
	including iBegin , iEnd
	*/
	static __int64 Rand(int iBegin, int iEnd);

};//class EmUtil

}//namespace em

#endif //#ifndef _EM_UTIL_H