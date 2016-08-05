#include "EmTcpServerAcceptCallbackImpl.h"
#include "EmTcpConnectWorker.h"

using namespace em;

int em::EmTcpServerAcceptCallbackImpl::OnAcceptNewClient( SOCKET xSockClientNew 
			, INT64 iKeyNew , INT64 iKeyPrevious , std::map<INT64,EmTcpConnectWorker*>* pConnectTable )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpServerAcceptCallbackImpl::OnAcceptMaxIdle()
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpServerAcceptCallbackImpl::OnAcceptClose()
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpServerAcceptCallbackImpl::OnAcceptException( int iExceptionCode )
{
	return EM_TCP_ACTION_DEFAULT;
}
