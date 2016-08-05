#ifndef _h_EmTcpServerMonitorThread
#define _h_EmTcpServerMonitorThread

#include "EmBase.h"
#include "EmThread.h"

namespace em
{
	class EmTcpServerMonitorThread : public EmThread
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