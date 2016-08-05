#ifndef _h_IEmTcpServerAcceptCallback
#define _h_IEmTcpServerAcceptCallback

#include "EmBase.h"

#include <string>
#include <vector>
#include <map>




#ifndef SOCKET
typedef unsigned int SOCKET;
#endif 

namespace em
{
	class EmTcpConnectWorker;
	class IEmTcpServerAcceptCallback
	{
	public:
		virtual int OnAcceptNewClient(SOCKET xSockClientNew
			, INT64 iKeyNew
			, INT64 iKeyPrevious
			, std::map<INT64,EmTcpConnectWorker*>* pConnectTable) = 0;

		virtual int OnAcceptMaxIdle() = 0;

		virtual int OnAcceptClose() = 0;
		
		virtual int OnAcceptException(int iExceptionCode) = 0;
	};
}
#endif