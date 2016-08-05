#ifndef _h_IEmNetStreamSender
#define _h_IEmNetStreamSender

#include "EmBase.h"

#define EM_NET_SEND_CONTINUE 0
#define EM_NET_SEND_STOP 1

namespace em
{

class IEmNetStreamSender
{
public:

	/*
	 if (@result == 0)
	 {
		stop send
	 }
	*/
	virtual int OnSendOnce(const char *bufSend, int iSendRequire, int iSendAlready, int iSendLast) = 0;

};//class IEmNetStreamSender

}//namespace em;

#endif//#ifndef _I_EM_NET_STREAM_SENDER_H