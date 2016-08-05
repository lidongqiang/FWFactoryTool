#ifndef _h_IEmNetStreamReceiver
#define _h_IEmNetStreamReceiver

#include "EmBase.h"

#define EM_NET_RECEIVE_CONTINUE 0
#define EM_NET_RECEIVE_STOP 1

namespace em
{

class IEmNetStreamReceiver
{
public:
	/*
	 if (@result == 0)
	 {
		stop recv
	 }
	*/
	virtual int OnReceiveOnce(char *bufRecv, int iRecvLimit, int iRecvAlready, int iRecvLast) = 0;
	virtual int OnReceiveOverflow(const char *bufRecv, int iRecvLimit, int iRecvAlready) = 0;

};//class IEmNetStreamReceiver

}//namespace em;

#endif//#ifndef _I_EM_NET_STREAM_RECEIVER_H