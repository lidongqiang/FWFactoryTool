#ifndef _h_EmTcpServerCleanThread
#define _h_EmTcpServerCleanThread

#include "EmBase.h"
#include "EmTcpBase.h"

#include "EmThread.h"

namespace em
{
	class EmTcpServerCleanThread : public EmThread
	{
	public:
		virtual void InitStopTag();
		virtual void StopSafely();
		virtual void Run();
	protected:
		virtual void PreRun();
		virtual void PostRun();
	};
}

#endif