#ifndef _h_EmTcpClientMonitorThread
#define _h_EmTcpClientMonitorThread

#include "EmBase.h"
#include "EmThread.h"

namespace em
{
	class EmTcpClientMonitorThread : public EmThread
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