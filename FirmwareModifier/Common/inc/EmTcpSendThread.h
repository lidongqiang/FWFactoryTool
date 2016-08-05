#ifndef _h_EmTcpSendThread
#define _h_EmTcpSendThread

#include "EmBase.h"
#include "EmTcpBase.h"
#include "EmThread.h"

namespace em
{
	class EmTcpConnectWorker;

	class EmTcpSendThread : public EmThread
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