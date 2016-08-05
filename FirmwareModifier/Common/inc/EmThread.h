#ifndef _h_EmThread
#define _h_EmThread

#include "EmBase.h"

#include <map>
#include <vector>

namespace em
{
	
class EmThread{

public:
	EmThread();
	virtual ~EmThread();

	void StopForcibly();
	void Start();
	
	void* GetHandle();
	unsigned long GetId();
	void SetCreateFlag(unsigned long fCreateFlag);
	bool IsAlive();
	bool IsStarted();
	bool IsSafeEnd();
	void SetWorker(void* pWorker);

	static void AddToTable(INT64 iKey, EmThread* pTh);
	static void CleanInTable();
	
protected:
	virtual void Run() = 0;
	virtual void StopSafely();
	virtual void InitStopTag();

protected:
	static unsigned long __stdcall SRun(void* pvParam);
	void RunWrapper();
	virtual void PreRun();
	virtual void PostRun();
	

	void* m_hTh;
	unsigned long m_iThId;
	int m_iStackSize;
	unsigned long m_fCreateFlag;
	bool m_bStarted;
	bool m_bNaturalEnd;
	bool m_bForcibleEnd;
	void* m_pWorker;

	static std::map<INT64,EmThread*> s_xTable;
	

};

}//namespace
#endif // !H_EmThread
