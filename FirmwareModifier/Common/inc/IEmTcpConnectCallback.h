#ifndef _h_IEmTcpConnectCallback
#define _h_IEmTcpConnectCallback

#include "EmBase.h"

#include <string>
#include <vector>
#include <map>







#ifndef SOCKET
typedef unsigned int SOCKET;
#endif 

namespace em
{
	class IEmTcpConnectCallback
	{
	public:
		virtual int OnSendBytesOnce(SOCKET xSockClient, INT64 iKey
			, const char* bySend, int iCompleteSize, int iTotalSize, int iCommandType) = 0;

		virtual int OnRecvBytesOnce(SOCKET xSockClient, INT64 iKey
			, const char* bytesRecv, int iCount) = 0;
		
		virtual int OnSendException(SOCKET sockClient, INT64 iKey, int iExceptionCode) = 0;

		virtual int OnRecvException(SOCKET sockClient, INT64 iKey, int iExceptionCode) = 0;
		
		virtual int OnConnect(SOCKET sockClient, INT64 iKey) = 0;

		virtual int OnClose(SOCKET sockClient, INT64 iKey) = 0;

		virtual int OnRecvMaxIdle(SOCKET sockClient, INT64 iKey) = 0;
	};
}
#endif