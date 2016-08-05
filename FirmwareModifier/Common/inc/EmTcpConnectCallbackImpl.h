#ifndef _h_EmTcpConnectCallbackImpl
#define _h_EmTcpConnectCallbackImpl

#include "EmBase.h"
#include "EmTcpBase.h"
#include "EmTcpPackReader.h"
#include "IEmTcpConnectCallback.h"

namespace em
{

	class EmTcpConnectCallbackImpl 
		: public virtual EmTcpPackReader
		, public virtual IEmTcpConnectCallback
	{

	public:
		EmTcpConnectCallbackImpl();
		virtual ~EmTcpConnectCallbackImpl();

///////////////////////////////////////////////////
		//begin EmTcpPackReader
	
		virtual int OnCmdText(const char* szText, INT64 iKey);
		virtual int OnCmdFile(const char* szFileName, int iFileSize, INT64 iKey);
		virtual int OnCmdRaw(const char* szFileName, int iFileSize, INT64 iKey);
		virtual int OnCmdBeat(INT64 iKey);
		virtual int OnCmdClose(INT64 iKey);
		virtual int OnCmdUnknown(const char* szCmd, INT64 iKey);
		virtual int OnProgressFile(const char* szTempFile, int iTotalSize, int iCurrentSize, int iOnceSize, INT64 iKey);
		
		
		//end EmStreamReceiver
////////////////////////////////////////////


		/////////////////////////////////////////////////////////////
		//begin IEmTcpConnectCallback
		
		virtual int OnSendBytesOnce(SOCKET xSockClient, INT64 iKey
			, const char* bySend, int iCompleteSize, int iTotalSize, int iCommandType);

		virtual int OnRecvBytesOnce(SOCKET xSockClient, INT64 iKey
			, const char* bytesRecv, int iCount);
		
		virtual int OnSendException(SOCKET sockClient, INT64 iKey, int iExceptionCode);

		virtual int OnRecvException(SOCKET sockClient, INT64 iKey, int iExceptionCode);
		
		virtual int OnConnect(SOCKET sockClient, INT64 iKey);

		virtual int OnClose(SOCKET sockClient, INT64 iKey);

		virtual int OnRecvMaxIdle(SOCKET sockClient, INT64 iKey);
		
		//end EtTcpClientOperator
		////////////////////////////////////////////////
	};
}
#endif