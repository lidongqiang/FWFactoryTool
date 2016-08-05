#ifndef _h_EmNet
#define _h_EmNet

#include "EmBase.h"

#include "EmNetBase.h"

#include <string>

#ifndef SOCKET
typedef unsigned int SOCKET;
#endif

namespace em
{


class IEmNetStreamSender;
class IEmNetStreamReceiver;
class EmNet  
{
private:
	static bool s_bStartup;
public:
	EmNet();
	virtual ~EmNet();
	static std::string IpTextByHostName(const char* szHostName);

	static std::string GetLocalIpText();
	static int Startup();
	static int Cleanup();
	static bool HasStartup();

	static SOCKET SocketCreateTcp();
	static int SocketClose(SOCKET *pSock);
	static int SocketShutdown(SOCKET xSock);
	static int SocketBind(SOCKET xSock, EmNetServer *pServer);
	static int TcpListen(SOCKET xSock, int iBackLog = 5);
	static SOCKET TcpAccept(SOCKET xSock, EmNetClient *pClient);
	static int TcpConnect(SOCKET xSock, EmNetServer *pServer, int iTimeoutSeconds = 5);
	static int TcpSend(SOCKET xSock, const char *bufSend, int iSendLen, int iSendOnceLen = 40960, IEmNetStreamSender *pSender = NULL);

	static int TcpSendStr(SOCKET xSock, const char* szMsg, EmEncode enc = EmEncodeUtf8);

	static int TcpRecv(SOCKET xSock, char *bufRead, int iRecvLimit, int iRecvOnceLen = 40960, IEmNetStreamReceiver *pReceiver = NULL);

	static std::string TcpRecvStr(SOCKET xSock, int iRecvLimit, EmEncode enc = EmEncodeUtf8);

	static int TcpRecvOnce(SOCKET xSock, char *bufRead, int iRecvLimit);

	static std::string TcpRecvOnceStr(SOCKET xSock, int lenLimit, EmEncode enc = EmEncodeUtf8);

	static int SocketSetTimeoutSend(SOCKET xSock, int iTimeoutMilli = 5000);
	static int SocketSetTimeoutRecv(SOCKET xSock, int iTimeoutMilli = 5000);
	static int SocketSetRecvBufSize(SOCKET xSock, int iBufSize);
	static int SocketSetSendBufSize(SOCKET xSock, int iBufSize);
	static std::string SocketPeerAddress(SOCKET xSock);
	static int SocketPeerPort(SOCKET xSock);
	static bool SocketIsValid(SOCKET xSock);

	static std::string HandyLinkStr(const char* szHostName, unsigned short nPort
		, const char *bufWrite
		, int iReadLimit, int iWriteLen
		, EmEncode enc = EmEncodeUtf8
		, IEmNetStreamSender *pSender = NULL, IEmNetStreamReceiver *pReceiver = NULL
		, int iSendOnceLen = 4096, int iRecvOnceLen = 4096
		, int iTimeoutMilli = 5000);

	static int HandyLink(const char* szHostName, unsigned short nPort
		, const char *bufWrite, char *bufRead
		, int iReadLimit, int iWriteLen
		, IEmNetStreamSender *pSender = NULL, IEmNetStreamReceiver *pReceiver = NULL
		, int iSendOnceLen = 4096, int iRecvOnceLen = 4096
		, int iTimeoutMilli = 5000);

	static bool TestConnection(const char* szHostName, unsigned short nPort, int iTimeoutMilli = 5000);
	
};//class EmNet



}//namespace em



#endif // !defined  EM_NET_H
