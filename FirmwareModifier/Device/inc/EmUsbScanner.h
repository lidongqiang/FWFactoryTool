#ifndef _h_EmUsbScanner
#define _h_EmUsbScanner

#include "../../Common/inc/EmBase.h"
#include "EmUsbDevice.h"

#include <string>
#include <vector>
#include <map>

#include <afx.h>

namespace em
{
	class EmUsbScanner
	{
	public:
		int Scan(const GUID& rGuid);
		int ListByGuid(std::vector<EmUsbDevice>& rDeviceList, const GUID& rGuid);
		int ListAll(std::vector<EmUsbDevice>& rDeviceList);
		std::string SearchPlainSerial(const char* szVid, const char* szPid);
		bool ExistedDevice(EmUsbDevice& rDevice);
		int AppendDevice(EmUsbDevice& rDevice);
		int FilterByClassGuid(const GUID& rGuid);
		int FilterByVid(const char* szVid);
		int FilterByServiceType(const char* szServiceType);
		int Count();
		void Clear();

	private:
		std::vector<EmUsbDevice> m_xDeviceList;
	};//class EmUsbScanner
}//namespace em


#endif//#ifndef EM_USB_SCANNER_H