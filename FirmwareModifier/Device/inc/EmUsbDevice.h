#ifndef _h_EmUsbDevice
#define _h_EmUsbDevice

#include <string>

#include "../../Common/inc/EmBase.h"





namespace em
{
	class EmUsbDevice
	{
	public:
		EmUsbDevice();
		virtual ~EmUsbDevice();

		std::string GetDevicePath();
		void SetDevicePath(const char* szDevicePath);

		std::string GetClassGuid();
		void SetClassGuid(const char* szClassGuid);

		std::string GetDriverMark();
		void SetDriverMark(const char* szDriverMark);

		std::string GetDriverManufacture();
		void SetDriverManufacture(const char* szDriverManufacture);

		std::string GetPhysicalName();
		void SetPhysicalName(const char* szPhysicalName);

		std::string GetLocationInfo();
		void SetLocationInfo(const char* szLocationInfo);

		std::string GetFriendlyName();
		void SetFriendlyName(const char* szFriendlyName);

		std::string GetHardwareId();
		void SetHardwareId(const char* szHardwardId);

		std::string GetInstanceId();
		void SetInstanceId(const char* szInstanceId);

		std::string GetDesc();
		void SetDesc(const char* szDesc);

		std::string GetServiceType();
		void SetServiceType(const char* szServiceType);

		std::string FetchSubProperty(const char* szSubKey);
		std::string FetchSerialNumber();
		std::string FetchPlainSerialNumber();
		std::string FetchPid();
		std::string FetchVid();
		std::string FetchMi();
		std::string FetchName();



		
	private:
		std::string m_strFriendlyName;
		std::string m_strInstanceId;
		std::string m_strHardwareId;
		std::string m_strDesc;
		std::string m_strServiceType;
		std::string m_strLocationInfo;
		std::string m_strClassGuid;
		std::string m_strDriverMark;
		std::string m_strDriverManufacture;
		std::string m_strPhysicalName;
		std::string m_strDevicePath;
	};
}


#endif