#ifndef _h_EmNetBase
#define _h_EmNetBase


#define EM_NET_DEFAULT_SEND_ONCE_SIZE 4096
#define EM_NET_DEFAULT_RECV_ONCE_SIZE 4096

#ifndef SOCKET 
typedef unsigned int SOCKET ;
#endif

typedef struct tagEmNetClient
{
	unsigned short nPort;
	char szIp[16];
	SOCKET xSocket;
}EmNetClient;

typedef struct tagEmNetServer
{
	unsigned short nPort;
	bool bAny;
	char szIp[16];
	char szHostName[255];
}EmNetServer;

//typedef bool (*EmNetFnSocketSend)(const char *bufSend, int iSendRequire, int iSendTotal, int iSendLast);
//typedef bool (*EmNetFnSocketRecv)(char *bufRecv, int iRecvLimit, int iRecvTotal, int iRecvLast);

#endif