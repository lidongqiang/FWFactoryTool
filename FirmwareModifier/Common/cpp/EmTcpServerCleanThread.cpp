#include "EmTcpServerCleanThread.h"
#include "EmTcpServerAcceptWorker.h"

using namespace em;

void em::EmTcpServerCleanThread::InitStopTag()
{
	((EmTcpServerAcceptWorker*)m_pWorker)->m_bNeedWorking = true;
}

void em::EmTcpServerCleanThread::StopSafely()
{
	((EmTcpServerAcceptWorker*)m_pWorker)->m_bNeedWorking = false;
}

void em::EmTcpServerCleanThread::Run()
{
	((EmTcpServerAcceptWorker*)m_pWorker)->ProcClean();
}

void em::EmTcpServerCleanThread::PreRun()
{
	EmThread::PreRun();
}

void em::EmTcpServerCleanThread::PostRun()
{
	EmThread::PostRun();
}



