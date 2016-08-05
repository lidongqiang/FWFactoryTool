#ifndef _h_EmGlobal
#define _h_EmGlobal

#include "EmBase.h"

namespace em
{
	class EmGlobal
	{
	public:
		static void Startup();
		static void Cleanup();

	};
}

#endif //#ifndef EM_GLOBAL_H