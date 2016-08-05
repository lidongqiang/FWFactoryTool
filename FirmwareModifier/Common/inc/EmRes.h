#ifndef _h_EmRes
#define _h_EmRes


#include "EmBase.h"
#include <afx.h>



namespace em
{

class EmRes
{
public:
	EmRes();
	virtual ~EmRes();
	static int GetBytes(BYTE * buf, int nResId, const char* szResType, HINSTANCE hResHandle = NULL);
	static int GetBytesSize(int nResId, const char* szReType , HINSTANCE hResHandle = NULL);

};//class EmRes

}//namespace em

#endif// #ifndef EM_RES_H
