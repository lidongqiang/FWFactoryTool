#ifndef _h_EmTcpServerAcceptThread
#define _h_EmTcpServerAcceptThread

#include "EmBase.h"
#include "IEmTcpServerAcceptCallback.h"
#include <string>
#include <vector>
#include <map>


#include "EmThread.h"





namespace em
{
	class EmTcpServerAcceptThread : public EmThread
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