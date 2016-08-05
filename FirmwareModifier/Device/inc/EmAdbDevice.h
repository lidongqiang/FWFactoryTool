#ifndef _h_EmAdbDevice
#define _h_EmAdbDevice

#include "../../Common/inc/EmBase.h"

#include <string>
#include <vector>

namespace em
{
	class EmAdbDevice
	{
	public:
		EmAdbDevice();
		virtual ~EmAdbDevice();

		std::string GetName();
		void SetName(const char* szName);

		std::string GetSerial();
		void SetSerial(const char* szSerial);

		std::string GetVid();
		void SetVid(const char* szVid);

		std::string GetPid();
		void SetPid(const char* szPid);

		std::string GetHardwareId();
		void SetHardwareId(const char* szHardwareId);

		std::string GetInstanceId();
		void SetInstanceId(const char* szInstanceId);

		bool GetInstalled();
		void SetInstalled(bool bInstalled);

		int AppendHardwareId(const char* szHardwardId);

		int ListHardwareId(std::vector<std::string>& xList);

		std::string ListHardwareIdToString();

	private:
		std::string m_strName;
		std::string m_strSerial;
		std::string m_strVid;
		std::string m_strPid;
		std::string m_strHardwareId;
		std::string m_strInstanceId;
		bool m_bInstalled;
		std::vector<std::string> m_xHardwareIdList;
	};
}


#endif