#include "EmNet.h"
#include "EmCharSet.h"
#include "EmTsz.h"
#include "EmSz.h"
#include "EmStlStr.h"
#include "EmMemory.h"
#include "IEmNetStreamReceiver.h"
#include "IEmNetStreamSender.h"
#include <WINSOCK2.H>

#include "EmHandy.h"
#include "EmStr.h"


bool em::EmNet::s_bStartup = false;

em::EmNet::EmNet()
{

}

em::EmNet::~EmNet()
{

}

int em::EmNet::Startup()
{
	if(HasStartup())
	{
		return 0;
	}
	WSAData xWsaData;
	int iResult = ::WSAStartup(MAKEWORD(2,2),&xWsaData);
	if(iResult != NO_ERROR)
	{
		return EM_ERR_NET_STARTUP;
	}
	s_bStartup = true;

	return 0;
}

int em::EmNet::Cleanup()
{
	if(!HasStartup())
	{
		return 0;
	}
	int iResult = ::WSACleanup();
	if(iResult != NO_ERROR)
	{
		return EM_ERR_NET_CLEANUP;
	}
	s_bStartup = false;
	return 0;
}

bool em::EmNet::HasStartup()
{
	return s_bStartup;
}

SOCKET em::EmNet::SocketCreateTcp()
{
	SOCKET sock = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	return sock;
}

int em::EmNet::SocketClose( SOCKET* sock )
{
	if(*sock == INVALID_SOCKET)
	{
		return 0;
	}
	int iResult = 0;
	if(*sock != 0)
	{
		iResult = ::closesocket(*sock);
	}
	*sock = INVALID_SOCKET;
	if(iResult != NO_ERROR)
	{
		return EM_ERR_NET_CLOSE;
	}
	
	return 0;
}

int em::EmNet::SocketShutdown( SOCKET sock )
{
	int iResult = ::shutdown(sock,SD_BOTH);
	if(iResult != NO_ERROR)
	{
		return EM_ERR_NET_SHUTDOWN;
	}
	return 0;
}

int em::EmNet::SocketBind( SOCKET sock, EmNetServer *pServer )
{
	if(pServer == NULL)
	{
		return EM_ERR_MEM_NULL;
	}
	SOCKADDR_IN xAddr;
	memset(&xAddr,0,sizeof(xAddr));
	xAddr.sin_family = AF_INET;
	xAddr.sin_port = htons(pServer->nPort);

	if(pServer->bAny)
	{
		xAddr.sin_addr.S_un.S_addr = ADDR_ANY;
	}
	else if(strlen(pServer->szIp) == 0)
	{
		if(strlen(pServer->szHostName) == 0)
		{
			return EM_ERR_NET_INVALID_HOSTNAME;
		}

		if(EmSz::IsIp(pServer->szHostName))
		{
			strcpy(pServer->szIp,pServer->szHostName);
		}
		else
		{
			strcpy(pServer->szIp, IpTextByHostName(pServer->szHostName).c_str() );
		}
	}
	
	xAddr.sin_addr.S_un.S_addr = htonl((u_long)EmStlStr::IpTextToNum(pServer->szIp)) ;
	
	int iResult = ::bind(sock,(SOCKADDR*)&xAddr,sizeof(xAddr));
	if(iResult != NO_ERROR)
	{
		return EM_ERR_NET_BIND;
	}
	return 0;
}

int em::EmNet::TcpListen( SOCKET sock, int iBackLog /*= 5*/ )
{
	int iResult = ::listen(sock,iBackLog);
	if(iResult != NO_ERROR)
	{
		return EM_ERR_NET_LISTEN;
	}
	return 0;
}

SOCKET em::EmNet::TcpAccept( SOCKET sock, EmNetClient *pClient )
{
	SOCKADDR_IN xAddrClient;
	memset(&xAddrClient,0,sizeof(xAddrClient));
	int iAddrClientLen = sizeof(xAddrClient);
	SOCKET xSocket = ::accept(sock,(SOCKADDR*)&xAddrClient,&iAddrClientLen);
	
	if(xSocket == INVALID_SOCKET)
	{
		return xSocket;
	}
	
	if(pClient != NULL)
	{
		pClient->nPort = ntohs(xAddrClient.sin_port);
		strcpy(pClient->szIp,EmStlStr::IpNumToText(htonl(xAddrClient.sin_addr.S_un.S_addr)).c_str());
		pClient->xSocket = xSocket;
	}

	return xSocket;
}

int em::EmNet::TcpConnect( SOCKET sock, EmNetServer *pServer, int iTimeoutSeconds /*= 5*/ )
{
	if(pServer == NULL)
	{
		return EM_ERR_MEM_NULL;
	}

	SOCKADDR_IN xAddr;
	memset(&xAddr,0,sizeof(xAddr));
	xAddr.sin_family = AF_INET;
	xAddr.sin_port = htons(pServer->nPort);
	
	if(strlen(pServer->szIp) == 0)
	{
		if(strlen(pServer->szHostName) == 0)
		{
			return EM_ERR_NET_INVALID_HOSTNAME;
		}

		if(EmSz::IsIp(pServer->szHostName))
		{
			strcpy(pServer->szIp,pServer->szHostName);
		}
		else
		{
			strcpy(pServer->szIp, IpTextByHostName(pServer->szHostName).c_str() );
		}
	}
	
	xAddr.sin_addr.S_un.S_addr = htonl((u_long)EmStlStr::IpTextToNum(pServer->szIp)) ;

	unsigned long ulNonBlock = 1; 
	int iResult = ioctlsocket(sock, FIONBIO, (unsigned long*)&ulNonBlock); 
	if(iResult == SOCKET_ERROR)
	{
		return EM_ERR_NET_IOCTL;
	}

	connect(sock,(SOCKADDR*)&xAddr,sizeof(xAddr));

	timeval stTimeout; 
	fd_set fdRead; 
	FD_ZERO(&fdRead); 
	FD_SET(sock, &fdRead); 
	stTimeout.tv_sec = iTimeoutSeconds; 
	stTimeout.tv_usec =0; 
	iResult = select(0, 0, &fdRead, 0, &stTimeout); 
	if(iResult <= 0) 
	{ 
		return EM_ERR_NET_TIMEOUT; 
	}

	unsigned long ulBlock = 0; 
	iResult = ioctlsocket(sock, FIONBIO, (unsigned long*)&ulBlock); 
	if(iResult == SOCKET_ERROR)
	{
		return EM_ERR_NET_IOCTL;
	}

	return 0;
}

int em::EmNet::TcpSend( SOCKET xSock, const char *bufSend, int iSendLen, int iSendOnceLen /*= 40960*/, IEmNetStreamSender *pSender /*=NULL*/)
{
	int iSendComplete = 0;
	int iResult = 0;
	int iTempSize = 0;
	int iFailCount = 0;
	if(iSendOnceLen <= 0){
		iSendOnceLen = EM_NET_DEFAULT_SEND_ONCE_SIZE;
	}
	do
	{
		if(iSendLen - iSendComplete < EM_NET_DEFAULT_SEND_ONCE_SIZE)
		{
			iTempSize = iSendLen - iSendComplete;
		}
		else
		{
			iTempSize = EM_NET_DEFAULT_SEND_ONCE_SIZE;
		}
		iResult = ::send(xSock,bufSend + iSendComplete, iTempSize,0);

		if(iResult >= 0)
		{
			iFailCount = 0;
			iSendComplete += iResult;
			if(pSender != NULL)
			{
				iResult = pSender->OnSendOnce(bufSend,iSendLen,iSendComplete,iResult);
				if(iResult < 0)
				{
					return iResult;
				}else if(iResult == EM_NET_SEND_STOP){
					break;
				}
			}
		}
		else
		{
			iFailCount++;
		}
		if(iFailCount > 1)
		{
			break;
		}

	}while(iSendComplete < iSendLen);

	if(iSendComplete < 0)
	{
		return EM_ERR_NET_SEND;
	}

	return iSendComplete;
}

int em::EmNet::TcpRecv( SOCKET sock, char *bufRead, int lenLimit, int iRecvOnceLen /*= 40960*/, IEmNetStreamReceiver *pReceiver /*=NULL*/)
{
	int iReadTotal = 0;
	int iResult = 0;
	int iFailCount = 0;

	if(iRecvOnceLen <= 0){
		iRecvOnceLen = EM_NET_DEFAULT_RECV_ONCE_SIZE;
	}

	do
	{
		iResult = ::recv(sock,bufRead + iReadTotal,iRecvOnceLen,0);
		if(iResult > 0)
		{
			iReadTotal += iResult;
			if(iReadTotal > lenLimit)
			{
				if(pReceiver != NULL)
				{
					pReceiver->OnReceiveOverflow(bufRead,lenLimit,iReadTotal);

					return EM_ERR_NET_RECV_OVERFLOW;
				}
			}
			iFailCount = 0;

			if(pReceiver != NULL)
			{
				iResult = pReceiver->OnReceiveOnce(bufRead,lenLimit,iReadTotal,iResult);
				if(iResult < 0)
				{
					return iResult;
				}
				else if(iResult == EM_NET_RECEIVE_STOP)
				{
					break;
				}
			}
		}

		if(iResult < 0)
		{
			iFailCount++;
		}

		if(iFailCount >= 2)
		{
			break;
		}

 	}while(iReadTotal < lenLimit);

	if(iResult < 0 && iReadTotal == 0)
	{
		return EM_ERR_NET_RECV;
	}

	return iReadTotal;
}

bool em::EmNet::SocketIsValid( SOCKET sock )
{
	if(sock == INVALID_SOCKET)
	{
		return false;
	}
	return true;
}

int em::EmNet::TcpSendStr( SOCKET sock, const char* szMsg, EmEncode eEncoding /*= EmEncodeUtf8*/ )
{
	int iWrite = 0;
	int iSrcLen = strlen(szMsg);
	int iAllocLen = 0;
	int iSendTotal = 0;
	int iResult = 0;
	if(strlen(szMsg) == 0)
	{
		return 0;
	}
	char *bufWrite = NULL;
	if(eEncoding == EmEncodeUtf8)
	{
		bufWrite = (char*)szMsg;
		iWrite = strlen(bufWrite);
	}
	else if(eEncoding == EmEncodeUnicode)
	{
		iAllocLen = iSrcLen * 2 + 2;
		bufWrite = (char*)malloc(iAllocLen);
		memset(bufWrite,0,iAllocLen);
		EmCharSet::Utf8ToUnicode((wchar_t *)bufWrite,szMsg);
		iWrite = wcslen((wchar_t *)bufWrite) * 2 + 2;
	}
	else
	{
		iAllocLen = iSrcLen * 2 + 2;
		bufWrite = (char*)EM_ALLOC(bufWrite,iAllocLen);
		memset(bufWrite,0,iAllocLen);
		EmCharSet::Utf8ToAnsi(bufWrite,szMsg);
		iWrite = strlen(bufWrite);
	}

	int iTempSize = 0;
	int iFailCount = 0;
	do
	{
		if(iWrite - iSendTotal < EM_NET_DEFAULT_SEND_ONCE_SIZE)
		{
			iTempSize = iWrite - iSendTotal;
		}
		else
		{
			iTempSize = EM_NET_DEFAULT_SEND_ONCE_SIZE;
		}
		iResult = ::send(sock,bufWrite + iSendTotal, iTempSize,0);
		if(iResult > 0)
		{
			iSendTotal += iResult;
		}

		if(iResult <= 0)
		{
			iFailCount++;
		}
		if(iFailCount > 2)
		{
			break;
		}
	}while(iSendTotal < iWrite);

	if(iAllocLen != 0)
	{
		EM_FREE_NULL( bufWrite);
	}
	
	if(iSendTotal == 0)
	{
		return EM_ERR_NET_SEND;
	}
	return iSendTotal;
}

std::string em::EmNet::TcpRecvStr( SOCKET sock, int lenLimit, EmEncode eEncoding /*= EmEncodeUtf8*/ )
{
	std::string strResult;
	char *bufRead = (char*)malloc(lenLimit);
	memset(bufRead,0,lenLimit);

	int iReadTotal = 0;
	int iResult = 0;
	int iFailCount = 0;
	do
	{
		iResult = ::recv(sock,bufRead + iReadTotal,EM_NET_DEFAULT_RECV_ONCE_SIZE,0);
		if(iResult > 0)
		{
			iReadTotal += iResult;
			iFailCount = 0;
		}
		if(iResult < 0)
		{
			iFailCount++;
		}


		if(iFailCount >= 2)
		{
			break;
		}

	}while( iReadTotal < lenLimit);
	
	if(iReadTotal <= 0)
	{
		return strResult;
	}	
	
	int iAllocLen = 0;
	if(iReadTotal > 0)
	{
		if(eEncoding == EmEncodeUtf8)
		{
			strResult.append(bufRead,iReadTotal);
		}
		else if(eEncoding == EmEncodeUnicode)
		{
			iAllocLen = iReadTotal*2 + 2;
			char *bufU8 = (char*)malloc(iAllocLen);
			memset(bufU8,0,iAllocLen);
			EmCharSet::UnicodeToUtf8(bufU8,(wchar_t*)bufRead);
			strResult = bufU8;
			EM_FREE_NULL( bufU8);
		}
		else if(eEncoding == EmEncodeAnsi)
		{
			iAllocLen = iReadTotal * 2 + 2;
			char *bufU8 = (char*)malloc(iAllocLen);
			memset(bufU8,0,iAllocLen);
			EmCharSet::AnsiToUtf8(bufU8,bufRead);
			strResult = bufU8;
			EM_FREE_NULL( bufU8);
		}
	}

	EM_FREE_NULL( bufRead);
	
	return strResult;
}



int em::EmNet::SocketSetTimeoutSend( SOCKET sock, int iTimeoutMilli /*= 5000*/ )
{
	int iResult = ::setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&iTimeoutMilli,sizeof(iTimeoutMilli));
	if(iResult < 0)
	{
		return EM_ERR_NET_SETOPT;
	}
	return 0;
}

int em::EmNet::SocketSetTimeoutRecv( SOCKET sock, int iTimeoutMilli /*= 5000*/ )
{
	int iResult = ::setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&iTimeoutMilli,sizeof(iTimeoutMilli));
	if(iResult < 0)
	{
		return EM_ERR_NET_SETOPT;
	}
	return 0;
}

std::string em::EmNet::IpTextByHostName( const char* szHostName )
{
	std::string csIp;

	struct hostent* pHost = NULL;
	pHost = ::gethostbyname(szHostName);
	if(pHost == NULL)
	{
		return csIp;
	}
	csIp = EmStlStr::IpNumToText( ntohl( *(u_long*)pHost->h_addr_list[0] ) );
	return csIp;
}

int em::EmNet::HandyLink(const char* szHostName, unsigned short nPort
						 , const char *bufWrite, char *bufRead
						 , int iReadLimit, int iWriteLen
						 , IEmNetStreamSender *pSender /*= NULL*/, IEmNetStreamReceiver *pReceiver /*= NULL*/
						 , int iSendOnceLen /*= 4096*/, int iRecvOnceLen /*= 4096*/
						 , int iTimeoutMilli /*= 5000*/)
{
	std::string strReturn;
	
	int iRecvLen = 0;

	int iResult = 0;
	SOCKET sock = SocketCreateTcp();
	if(sock == INVALID_SOCKET)
	{
		return EM_ERR_NET_CREATE;
	}

	
	
	EmNetServer stSockServer;
	memset(&stSockServer,0,sizeof(stSockServer));
	stSockServer.nPort = nPort;
	strcpy(stSockServer.szHostName,szHostName);

	int iRecvTimeout = 300;

	if(iReadLimit >= 1024*1024*100){
		iRecvTimeout = 3500;
	}else if(iReadLimit >= 1024*1024*50){
		iRecvTimeout = 3000;
	}else if(iReadLimit >= 1024*1024*10){
		iRecvTimeout = 2500;
	}else if(iReadLimit >= 1024*1024*5){
		iRecvTimeout = 1800;
	}else if(iReadLimit >= 1024*1024*1){
		iRecvTimeout = 1500;
	}else if(iReadLimit >= 1024*500){
		iRecvTimeout = 1400;
	}else if(iReadLimit >= 1024*100){
		iRecvTimeout = 1200;
	}else{
		iRecvTimeout = 1000;
	}
	
	SocketSetTimeoutSend(sock,300);
	SocketSetTimeoutRecv(sock,iRecvTimeout);

	SocketSetRecvBufSize(sock,iRecvOnceLen);
	SocketSetSendBufSize(sock,iSendOnceLen);
	
	iResult = TcpConnect(sock,&stSockServer, iTimeoutMilli / 1000);
	if(iResult < 0)
	{
		return iResult;
	}
	
	iResult = TcpSend(sock,bufWrite,iWriteLen,iSendOnceLen,pSender);
	if(iResult < 0)
	{
		return iResult;
	}

	iResult = TcpRecv(sock,bufRead,iReadLimit,iRecvOnceLen,pReceiver);
	if(iResult < 0)
	{
		return iResult;
	}
	
	iRecvLen = iResult;

	iResult = ::closesocket(sock);
	if(iResult < 0)
	{
		return iResult;
	}


	return iRecvLen;
}

std::string em::EmNet::HandyLinkStr(const char* szHostName, unsigned short nPort
									, const char *bufWrite
									, int iReadLimit, int iWriteLen
									, EmEncode enc /*= EmEncodeUtf8*/
									, IEmNetStreamSender *pSender /*= NULL*/, IEmNetStreamReceiver *pReceiver /*= NULL*/
									, int iSendOnceLen /*= 4096*/, int iRecvOnceLen /*= 4096*/
									, int iTimeoutMilli /*= 5000*/)
{
	std::string strReturn;
	
	int iResult = 0;
	SOCKET sock = SocketCreateTcp();
	if(sock == INVALID_SOCKET)
	{
		return strReturn;
	}
	
	EmNetServer stSockServer;
	memset(&stSockServer,0,sizeof(stSockServer));
	stSockServer.nPort = nPort;
	strcpy(stSockServer.szHostName,szHostName);
	
	SocketSetTimeoutSend(sock,200);
	SocketSetTimeoutRecv(sock,300);

	SocketSetSendBufSize(sock,iSendOnceLen);
	SocketSetRecvBufSize(sock,iRecvOnceLen);
	
	
	iResult = TcpConnect(sock,&stSockServer, iTimeoutMilli / 1000);
	if(iResult < 0)
	{
		return strReturn;
	}
	
	iResult = TcpSendStr(sock,bufWrite,enc);
	if(iResult < 0)
	{
		return strReturn;
	}
	
	strReturn = TcpRecvStr(sock,iReadLimit,enc);
	::closesocket(sock);
	
	return strReturn;
}

bool em::EmNet::TestConnection( const char* szHostName, unsigned short nPort, int iTimeoutMilli /*= 5000*/ )
{
	int iResult = 0;
	SOCKET sock = SocketCreateTcp();
	if(sock == INVALID_SOCKET)
	{
		return false;
	}
	
	EmNetServer stSockServer;
	memset(&stSockServer,0,sizeof(stSockServer));
	stSockServer.nPort = nPort;
	strcpy(stSockServer.szHostName,szHostName);
	
	iResult = TcpConnect(sock,&stSockServer, iTimeoutMilli / 1000);
	if(iResult < 0)
	{
		return false;
	}

	::closesocket(sock);
	
	return true;
}

std::string em::EmNet::GetLocalIpText()
{
	std::string csDest;
	char szHostName[128] = {0};
	gethostname(szHostName, 128);
	hostent* pHost = gethostbyname(szHostName);

	csDest = EmStlStr::IpNumToText( ntohl( *(u_long*)pHost->h_addr_list[0] ) );

	return csDest;
}

int em::EmNet::TcpRecvOnce( SOCKET sock, char *bufRead, int lenLimit )
{
	int iReadTotal = 0;
	int iResult = 0;
	iResult = ::recv(sock,bufRead,lenLimit,0);
	
	if(iResult < 0)
	{
		return EM_ERR_NET_RECV;
	}

	return iResult;
}

std::string em::EmNet::TcpRecvOnceStr( SOCKET sock, int lenLimit, EmEncode eEncoding /*= EmEncodeUtf8*/ )
{
	std::string strResult;
	char *bufRead = (char*)EM_ALLOC(bufWrite,lenLimit);
	memset(bufRead,0,lenLimit);
	int iResult = 0;
	iResult = ::recv(sock,bufRead,lenLimit,0);
	int iReadTotal = iResult;
	
	if(iReadTotal < 0)
	{
		EM_FREE_NULL( bufRead);
		return strResult;
	}	
	
	int iAllocLen = 0;
	if(iReadTotal > 0)
	{
		if(eEncoding == EmEncodeUtf8)
		{
			strResult = bufRead;
		}
		else if(eEncoding == EmEncodeUnicode)
		{
			iAllocLen = iReadTotal * 2 + 2;
			char *bufU8 = (char*)malloc(iAllocLen);
			memset(bufU8,0,iAllocLen);
			EmCharSet::UnicodeToUtf8(bufU8,(wchar_t*)bufRead);
			strResult = bufU8;
			EM_FREE_NULL( bufU8);
		}
		else
		{
			iAllocLen = iReadTotal * 2 + 2;
			char *bufU8 = (char*)malloc(iAllocLen);
			memset(bufU8,0,iAllocLen);
			EmCharSet::AnsiToUtf8(bufU8,bufRead);
			strResult = bufU8;
			EM_FREE_NULL( bufU8);
		}
	}
	
	EM_FREE_NULL( bufRead);
	
	return strResult;
}

int em::EmNet::SocketPeerPort( SOCKET xSock )
{
	if(xSock == INVALID_SOCKET){
		return 0;
	}

	SOCKADDR_IN xAddr;
	memset(&xAddr,0,sizeof(xAddr));
	int iNameLen = sizeof(xAddr);
	int iResult = getpeername(xSock,(SOCKADDR*)&xAddr,&iNameLen);
	if(iResult != 0){
		return 0;
	}

	if(iNameLen > 0){
		return ntohs(xAddr.sin_port);
	}

	return 0;
}

std::string em::EmNet::SocketPeerAddress( SOCKET xSock )
{
	std::string strResult;

	if(xSock == INVALID_SOCKET){
		return strResult;
	}
	
	
	SOCKADDR_IN xAddr;
	memset(&xAddr,0,sizeof(xAddr));
	int iNameLen = sizeof(xAddr);
	int iResult = getpeername(xSock,(SOCKADDR*)&xAddr,&iNameLen);
	if(iResult != 0){
		return strResult;
	}
	
	if(iNameLen > 0){
		strResult = EmStlStr::IpNumToText(htonl(xAddr.sin_addr.S_un.S_addr));
	}
	
	return strResult;
}

int em::EmNet::SocketSetRecvBufSize( SOCKET sock, int iBufSize )
{
	setsockopt(sock,SOL_SOCKET, SO_SNDBUF, (char*)&iBufSize,sizeof(iBufSize));
	return 0;
}

int em::EmNet::SocketSetSendBufSize( SOCKET sock, int iBufSize )
{
	setsockopt(sock,SOL_SOCKET, SO_RCVBUF, (char*)&iBufSize,sizeof(iBufSize));
	return 0;
}
