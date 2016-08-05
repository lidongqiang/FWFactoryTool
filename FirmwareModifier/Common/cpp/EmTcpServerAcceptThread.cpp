#include "EmTcpServerAcceptThread.h"
#include "EmTcpServerAcceptWorker.h"

using namespace em;

void em::EmTcpServerAcceptThread::InitStopTag()
{
	((EmTcpServerAcceptWorker*)m_pWorker)->m_bNeedWorking = true;
}

void em::EmTcpServerAcceptThread::StopSafely()
{
	((EmTcpServerAcceptWorker*)m_pWorker)->m_bNeedWorking = false;
}

void em::EmTcpServerAcceptThread::Run()
{
	((EmTcpServerAcceptWorker*)m_pWorker)->ProcAccept();
}

void em::EmTcpServerAcceptThread::PreRun()
{
	EmThread::PreRun();
}

void em::EmTcpServerAcceptThread::PostRun()
{
	EmThread::PostRun();
}


