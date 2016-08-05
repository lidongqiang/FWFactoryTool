#ifndef _h_EmTcpServerAcceptCallbackImpl
#define _h_EmTcpServerAcceptCallbackImpl

#include "EmBase.h"
#include "EmTcpBase.h"
#include "IEmTcpServerAcceptCallback.h"

#include <string>
#include <vector>
#include <map>







namespace em
{
	class EmTcpConnectWorker;

	class EmTcpServerAcceptCallbackImpl:
		public virtual IEmTcpServerAcceptCallback
	{
	public:
		virtual int OnAcceptNewClient(SOCKET xSockClientNew
			, INT64 iKeyNew 
			, INT64 iKeyPrevious
			, std::map<INT64,EmTcpConnectWorker*>* pConnectTable);
		
		virtual int OnAcceptMaxIdle();
		
		virtual int OnAcceptClose();
		
		virtual int OnAcceptException(int iExceptionCode);
	};
}
#endif