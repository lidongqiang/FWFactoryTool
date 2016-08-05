#ifndef _h_EmTcpRecvThread
#define _h_EmTcpRecvThread

#include "EmBase.h"
#include "EmTcpBase.h"
#include "EmThread.h"

namespace em
{
	class EmTcpConnectWorker;

	class EmTcpRecvThread : public EmThread
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