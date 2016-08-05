#include "EmGlobal.h"

#include "EmNet.h"
#include "EmTcpPackWriter.h"


void em::EmGlobal::Startup()
{
	EmNet::Startup();
	EmTcpPackWriter::StaticInit();
}

void em::EmGlobal::Cleanup()
{
	EmTcpPackWriter::StaticExit();
	EmNet::Cleanup();
}
