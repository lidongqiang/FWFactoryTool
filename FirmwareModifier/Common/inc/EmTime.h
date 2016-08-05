#ifndef _h_EmTime
#define _h_EmTime


#include "EmBase.h"

#include <afx.h>

#include <string>

namespace em
{
	typedef struct tagEmTimeLength
	{
		int iDays;
		int iHours;
		int iMiutes;
		int iSeconds;
	}EmTimeLength;

	class EmTime
	{
	private:
		int m_iYear;
		int m_iMonth;
		int m_iDate;
		int m_iHour;
		int m_iMinute;
		int m_iSecond;
		int m_iDay;
		int m_iMillisec;
		int m_iStamp;

		
		void MakeTime();
		void MakeDay();
	public:
		EmTime();
		~EmTime();
		bool IsUnixStart();
		bool Refresh();
		void LoadCurrentTime();
		int GetYear() const;
		int GetMonth() const;
		int GetDate() const;
		int GetHour() const;
		int GetMinute() const;
		int GetSecond() const;
		int GetDay() const;
		int GetStamp() const;
		INT64 GetStamp13() const;
		int GetMillisec() const;

		bool SetYear(int iYear);
		bool SetMonth(int iMonth);
		bool SetDate(int iDate);
		bool SetHour(int iHour);
		bool SetMinute(int iMonth);
		bool SetSecond(int iSecond);
		bool SetMillisec(int iMonth);
		bool SetStamp(int iStamp);
		bool SetStamp13(INT64 iStamp);

		bool AddDay(int iDate);
		bool AddHour(int iHour);
		bool AddMinute(int iMonth);
		bool AddSecond(int iSecond);
		bool AddMillisec(int iMillisec);
		std::string ToString();
		
		bool FromStamp(int iStamp);
		bool FromStamp13(INT64 iStamp);

		static bool StampToObject(EmTime *pTime, int iStamp);
		static bool SysTimeToObject(EmTime *pTime, const SYSTEMTIME* pSysTime);
		static bool FileTimeToObject(EmTime *pTime, const FILETIME* pFileTime);
		static bool ObjectToSysTime(SYSTEMTIME* pSysTime, const  EmTime *pTime);
		static bool ObjectToFileTime(FILETIME* pFileTime, const EmTime *pTime);
		static int ObjectToStamp(const EmTime *pTime);
		static int CountDaysByMonth( int iYear, int iMonth);
		static int CountDaysByYear(int iYear);

		std::string Format(const char* szSrc);

		static int CurrentYear();
		static int CurrentMonth();
		static int CurrentDate();
		static int CurrentHour();
		static int CurrentMinute();
		static int CurrentSecond();
		static int CurrentMillisec();
		static int CurrentDay();
		static int CurrentStamp();
		static INT64 CurrentStamp13();
		static bool IsLeapYear(int iYear);
		static std::string CurrentFormat(const char* szSrc);
		static int GetTimeZoneMinutes();
		static void SecondsToLength(EmTimeLength *pLength, int iTotalSeconds);
		static int LengthToSeconds(EmTimeLength *pLength);
		static std::string FixTimeStrLen(const char* szSrc);
		static bool StrToObject(EmTime *pTime, const char* szSrc);
		static int StrToStamp(const char* szSrc);
		static std::string StamptoStr(int iStamp);

	};
}


#endif