#ifndef _h_EmUrl
#define _h_EmUrl

#include "EmBase.h"


namespace em
{
	class EmUrl
	{
	public:

		static char * Encode(char *szDest, const char *szSrc, bool bEncodeAll);   
		static char * Decode(char *szDest, const char *szSrc, bool bEncodeAll);      
	};
}


#endif