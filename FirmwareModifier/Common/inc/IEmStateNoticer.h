#ifndef _h_IEmStateNoticer
#define _h_IEmStateNoticer

#include "EmBase.h"

namespace em
{

class IEmStateNoticer
{
public:
	virtual void SetState(int iState) = 0;
	virtual int GetState() = 0;
};//class IEmWebScriptReceiver

}//namespace em


#endif//#ifndef _h_IEmStateNoticer

