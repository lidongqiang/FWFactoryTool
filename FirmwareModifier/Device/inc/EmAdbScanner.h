#ifndef _h_EmAdbScanner
#define _h_EmAdbScanner

#include "../../Common/inc/EmBase.h"

#include <string>
#include <map>

#include "EmAdbDevice.h"
#include "EmUsbScanner.h"

namespace em
{
	class EmUsbDevice;
	class EmUsbScanner;
	class EmAdbScanner
	{
	public:
		EmAdbScanner();
		virtual ~EmAdbScanner();
		int Scan();
        int ScanByVid(const char *vid);
		void Clear();
		int ListInstalled(std::vector<EmAdbDevice>& rList, bool bInstalled);
		int GetDeviceBySerial(const char* szSerial, EmAdbDevice& xDevice);
		std::string GetInstanceIdByVidPid(const char* szVid, const char* szPid);

	private:
		bool IsInList(EmUsbDevice& xDevice);
		bool IsPossibleAdb(EmUsbDevice& xDevice);

		
		EmUsbScanner m_xUsbScanner;
		std::map<std::string,EmAdbDevice> m_xAdbTable;
	};//class EmAdbScanner
}//namespace em


#endif//#ifndef EM_ADB_SCANNER_H