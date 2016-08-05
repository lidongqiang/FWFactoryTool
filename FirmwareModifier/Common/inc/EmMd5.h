#ifndef _h_EmMd5
#define _h_EmMd5

#include "EmBase.h"

namespace em
{
	
	class EmMd5
	{
	public:
		static char *Encode(char *szDest, const char *szSrc);
	};
}


#endif