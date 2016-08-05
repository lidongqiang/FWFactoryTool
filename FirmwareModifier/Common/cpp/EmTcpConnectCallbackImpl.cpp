#include "EmTcpConnectCallbackImpl.h"

using namespace em;

em::EmTcpConnectCallbackImpl::EmTcpConnectCallbackImpl()
{
	
}

em::EmTcpConnectCallbackImpl::~EmTcpConnectCallbackImpl()
{
}

int em::EmTcpConnectCallbackImpl::OnCmdText( const char* szText, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnCmdFile( const char* szFileName, int iFileSize, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnCmdRaw( const char* szFileName, int iFileSize, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnCmdBeat( INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnCmdClose( INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnProgressFile( const char* szTempFile, int iTotalSize, int iCurrentSize, int iOnceSize, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnCmdUnknown( const char* szCmd, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnSendBytesOnce( SOCKET xSockClient, INT64 iKey , const char* bySend, int iCompleteSize, int iTotalSize, int iCommandType )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnRecvBytesOnce( SOCKET xSockClient, INT64 iKey , const char* bytesRecv, int iCount )
{
	return ProcessBytes((const char*)bytesRecv,iCount,iKey);
}

int em::EmTcpConnectCallbackImpl::OnSendException( SOCKET sockClient, INT64 iKey, int iExceptionCode )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnRecvException( SOCKET sockClient, INT64 iKey, int iExceptionCode )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnConnect( SOCKET sockClient, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnClose( SOCKET sockClient, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpConnectCallbackImpl::OnRecvMaxIdle( SOCKET sockClient, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}
