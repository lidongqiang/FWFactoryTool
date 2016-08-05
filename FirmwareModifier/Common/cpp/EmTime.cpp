#include "stdafx.h"
#include "../inc/EmTime.h"
#include "../inc/EmStlStr.h"
#include "../inc/EmSz.h"
#include <afx.h>

em::EmTime::EmTime()
{
	m_iYear = 1970;
	m_iMonth = 1;
	m_iDate = 1;
	m_iHour = 0;
	m_iMinute = 0;
	m_iSecond = 0;
	m_iMillisec = 0;
	m_iDay = 4;
	m_iStamp = 0;
}

em::EmTime::~EmTime()
{

}

int em::EmTime::CurrentYear()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st.wYear;
}

int em::EmTime::CurrentMonth()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st.wMonth;
}

int em::EmTime::CurrentDate()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st.wDay;
}

int em::EmTime::CurrentHour()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st.wHour;
}

int em::EmTime::CurrentMinute()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st.wMinute;
}

int em::EmTime::CurrentSecond()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st.wSecond;
}

int em::EmTime::CurrentMillisec()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return (int)(st.wMilliseconds);
}

int em::EmTime::CurrentDay()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return st.wDayOfWeek;
}

int em::EmTime::CurrentStamp()
{
	time_t iStamp = time(NULL);
	iStamp += (-GetTimeZoneMinutes() * 60);
	return (int)iStamp;
}

bool em::EmTime::IsLeapYear( int iYear )
{
	if(iYear % 400 == 0)
	{
		return true;
	}
	else if(iYear % 100 != 0 && iYear % 4 == 0)
	{
		return true;
	}
	
	return false;
}

std::string em::EmTime::CurrentFormat( const char* szSrc )
{
	EmTime tNow;
	tNow.LoadCurrentTime();
	return tNow.Format(szSrc);
}

int em::EmTime::GetYear() const
{
	return m_iYear;
}

int em::EmTime::GetMonth() const
{
	return m_iMonth;
}

int em::EmTime::GetDate() const
{
	return m_iDate;
}

int em::EmTime::GetHour() const
{
	return m_iHour;
}

int em::EmTime::GetMinute() const
{
	return m_iMinute;
}

int em::EmTime::GetSecond() const
{
	return m_iSecond;
}

int em::EmTime::GetDay() const
{
	return m_iDay;
}

bool em::EmTime::SetYear( int iYear )
{
	m_iYear = iYear;
	return true;
}

bool em::EmTime::SetMonth( int iMonth )
{
	if(iMonth > 12 || iMonth < 1)
	{
		return false;
	}
	m_iMonth = iMonth;
	return true;
}

bool em::EmTime::SetDate( int iDate)
{

	if(iDate > 31 || iDate < 1)
	{
		return false;
	}
	m_iDate = iDate;
	return true;
}

bool em::EmTime::SetHour( int iHour )
{
	if( iHour > 23 )
	{
		return false;
	}
	m_iHour = iHour;
	return true;
}

bool em::EmTime::SetMinute( int iMinute )
{
	if( iMinute > 60 )
	{
		return false;
	}
	m_iMinute = iMinute;
	return true;
}

bool em::EmTime::SetSecond( int iSecond )
{
	if( iSecond > 60 )
	{
		return false;
	}
	m_iSecond = iSecond;
	return true;
}

bool em::EmTime::SetStamp( int iStamp )
{
	m_iStamp = iStamp;
	return true;
}

bool em::EmTime::SetMillisec( int iMillisec )
{
	if( iMillisec > 999 )
	{
		return false;
	}
	return true;
}

int em::EmTime::GetStamp() const
{
	return m_iStamp;
}

int em::EmTime::GetMillisec() const
{
	return m_iMillisec;
}

bool em::EmTime::Refresh()
{
	if(m_iDate > CountDaysByMonth(m_iYear,m_iMonth))
	{
		return false;
	}
	MakeTime();
	MakeDay();
	return true;
}

bool em::EmTime::AddDay( int iDate )
{
	m_iStamp += iDate * 24 * 60 * 60;
	StampToObject(this,m_iStamp);
	return true;
}

bool em::EmTime::AddHour( int iHour )
{
	m_iStamp += iHour * 60 * 60;
	StampToObject(this,m_iStamp);
	return true;
}

bool em::EmTime::AddMinute( int iMinute )
{
	m_iStamp += iMinute * 60;
	StampToObject(this,m_iStamp);
	return true;
}

bool em::EmTime::AddSecond( int iSecond )
{
	m_iStamp += iSecond;
	StampToObject(this,m_iStamp);
	return true;
}

bool em::EmTime::AddMillisec( int iMillisec )
{
	m_iStamp += iMillisec / 1000;
	StampToObject(this,m_iStamp);
	return true;
}

void em::EmTime::MakeTime()
{	
	m_iStamp = ObjectToStamp(this);
}

void em::EmTime::LoadCurrentTime()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);

	m_iYear = st.wYear;
	m_iMonth = st.wMonth;
	m_iDate = st.wDay;
	m_iHour = st.wHour;
	m_iMinute = st.wMinute;
	m_iSecond = st.wSecond;
	//m_iDay = st.wDayOfWeek;
	m_iMillisec = (int)(st.wMilliseconds);
	MakeTime();
	MakeDay();
}

std::string em::EmTime::Format( const char* szSrc )
{
	char szDest[500] = {0};


	char szYear[10] = {0};
	char szMonth[10] = {0};
	char szDate[10] = {0};
	char szHour[10] = {0};
	char szMinute[10] = {0};
	char szSecond[10] = {0};
	char szMilliSec[10] = {0};
	char szDayOfWeek[10] = {0};

	EmSz::Int2Str(szYear,m_iYear);
	EmSz::Int2Str(szMonth,m_iMonth);
	EmSz::Int2Str(szDate,m_iDate);
	EmSz::Int2Str(szHour,m_iHour);
	EmSz::Int2Str(szMinute,m_iMinute);
	EmSz::Int2Str(szSecond,m_iSecond);
	EmSz::Int2Str(szMilliSec,m_iMillisec);
	EmSz::Int2Str(szDayOfWeek,m_iDay);

	EmSz::PadLeft(szYear,'0',2);
	EmSz::PadLeft(szMonth,'0',2);
	EmSz::PadLeft(szDate,'0',2);
	EmSz::PadLeft(szHour,'0',2);
	EmSz::PadLeft(szMinute,'0',2);
	EmSz::PadLeft(szSecond,'0',2);
	EmSz::PadLeft(szMilliSec,'0',3);

	strcpy(szDest,szSrc);
	EmSz::Lower(szDest);

	EmSz::Replace(szDest,"y",szYear);
	EmSz::Replace(szDest,"m",szMonth);
	EmSz::Replace(szDest,"d",szDate);
	EmSz::Replace(szDest,"h",szHour);
	EmSz::Replace(szDest,"i",szMinute);
	EmSz::Replace(szDest,"s",szSecond);
	EmSz::Replace(szDest,"w",szDayOfWeek);
	EmSz::Replace(szDest,"l",szMilliSec);
	std::string strDest = szDest;
	return strDest;
}

bool em::EmTime::FromStamp( int iStamp )
{
	StampToObject(this,iStamp);
	return true;
}

bool em::EmTime::StampToObject(EmTime *pTime, int iStamp )
{
	int iSecOfDay = 24 * 60 * 60;
	int iDayOfYear = 0;
	int iDayOfMonth = 0;
	int iYear = 1970;
	int iMonth = 1;
	int iDate = 1;
	int iHour = 0;
	int iMinute = 0;
	int iSecond = 0;
	int iSecLeft = iStamp;
	int iDay = 0;

	if(pTime == NULL)
	{
		return false;
	}

	while(true)
	{
		if(iSecLeft <= 0)
		{
			break;
		}
		iDayOfYear = CountDaysByYear(iYear);
		if(iDayOfYear * iSecOfDay > iSecLeft)
		{
			break;
		}else
		{
			iSecLeft -= iDayOfYear * iSecOfDay;
			iYear++;
		}
	}

	while(true)
	{
		if(iSecLeft <= 0)
		{
			break;
		}
		iDayOfMonth = CountDaysByMonth(iYear,iMonth);
		if(iDayOfMonth * iSecOfDay > iSecLeft)
		{
			break;
		}
		else
		{
			iSecLeft -= iDayOfMonth * iSecOfDay;
			iMonth++;
		}
	}

	while(true)
	{
		if(iSecLeft <= 0)
		{
			break;
		}
		if(iSecOfDay > iSecLeft)
		{
			break;
		}
		else
		{
			iSecLeft -= iSecOfDay;
			iDate++;
		}
	}

	while(true)
	{
		if(iSecLeft <= 0)
		{
			break;
		}
		if(3600 > iSecLeft)
		{
			break;
		}
		else
		{
			iSecLeft -= 3600;
			iHour++;
		}
	}

	while(true)
	{
		if(iSecLeft <= 0)
		{
			break;
		}
		if(60 > iSecLeft)
		{
			break;
		}
		else
		{
			iSecLeft -= 60;
			iMinute++;
		}
	}

	iSecond = iSecLeft;

	pTime->SetYear(iYear);
	pTime->SetMonth(iMonth);
	pTime->SetDate(iDate);
	pTime->SetHour(iHour);
	pTime->SetMinute(iMinute);
	pTime->SetSecond(iSecond);
	pTime->SetMillisec(0);
	pTime->SetStamp(iStamp);
	pTime->MakeDay();

	return true;
}

int em::EmTime::CountDaysByMonth( int iYear, int iMonth )
{
	int iDays = 0;
	bool bIsLeap = IsLeapYear(iYear);

	switch(iMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		iDays = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		iDays = 30;
		break;
	case 2:
		iDays = bIsLeap ? 29 : 28;
		break;
	default:
		break;
	}
	return iDays;
}

int em::EmTime::CountDaysByYear( int iYear )
{
	int iDays = IsLeapYear(iYear) ? 366 : 365;
	return iDays;
}

int em::EmTime::GetTimeZoneMinutes()
{
	TIME_ZONE_INFORMATION tzi;
	::GetTimeZoneInformation(&tzi);
	return tzi.Bias;
}

int em::EmTime::ObjectToStamp( const EmTime *pTime )
{
	int iYearLoop = 1970;
	int iMonthLoop = 1;
	int iStamp = 0;

	if(pTime == NULL)
	{
		return 0;
	}
	
	if( pTime->GetYear() >= 1970 )
	{
		while(true)
		{
			if(iYearLoop >= pTime->GetYear())
			{
				break;
			}
			iStamp += CountDaysByYear(iYearLoop) * 24 * 60 * 60;
			iYearLoop++;
		}

		while(true)
		{
			if(iMonthLoop >= pTime->GetMonth())
			{
				break;
			}
			iStamp += CountDaysByMonth(iYearLoop,iMonthLoop) * 24 * 60 * 60;
			iMonthLoop++;
		}

		iStamp += (pTime->GetDate() - 1) * 24 * 60 * 60;
		iStamp += pTime->GetHour() * 60 * 60;
		iStamp += pTime->GetMinute() * 60;
		iStamp += pTime->GetSecond();
	}
	else
	{
		iYearLoop = 1970;
		while(true)
		{
			if(iYearLoop <= pTime->GetYear() + 1)
			{
				break;
			}
			iStamp -= CountDaysByYear(iYearLoop) * 24 * 60 * 60;
			iYearLoop--;
		}
		
		iMonthLoop = 12;
		while(true)
		{
			if(iMonthLoop <= pTime->GetMonth())
			{
				break;
			}
			iStamp -= CountDaysByMonth(iYearLoop,iMonthLoop) * 24 * 60 * 60;
			iMonthLoop--;
		}
		
		iStamp -= (CountDaysByMonth(iYearLoop,iMonthLoop) - pTime->GetDate()) * 24 * 60 * 60;
		iStamp -= (24 - pTime->GetHour() - 1) * 60 * 60;
		iStamp -= (60 - pTime->GetMinute() - 1) * 60;
		iStamp -= (60 - pTime->GetSecond());
	}

	return iStamp;
}

void em::EmTime::MakeDay()
{
	int iStampRef = 0;
	int iStampDiff = 0;
	int iDays = 0;
	int iDayBase = 0;

	
	if(m_iStamp == 0)
	{
		MakeTime();
	}
	//1970-1-1 0:0:0 thursday 4
	iStampRef = 0;
	iDayBase = 4;
	iStampDiff = m_iStamp - iStampRef;
	if(iStampDiff >= 0)
	{
		iDays =  (int) ( iStampDiff / (24 * 60 * 60) );
		iDays = (iDays) % 7;
		m_iDay = (iDays + iDayBase) % 7;
	}
	else
	{
		iDays =  (int) (((double)-iStampDiff / (double)(24 * 60 * 60)) + 0.5 );
		iDays = (iDays) % 7;
		m_iDay = (7 - iDays + iDayBase) % 7;
	}
}

void em::EmTime::SecondsToLength( EmTimeLength *pLength, int iTotalSeconds )
{
	int iSecLeft = iTotalSeconds;
	if(pLength == NULL)
	{
		return;
	}

	pLength->iDays = iSecLeft / (24 * 60 * 60);
	iSecLeft = iSecLeft % (24 * 60 * 60);

	pLength->iHours = iSecLeft / (60 * 60);
	iSecLeft = iSecLeft % (60 * 60);

	pLength->iMiutes = iSecLeft / 60;
	iSecLeft = iSecLeft % 60;

	pLength->iSeconds = iSecLeft;
}

int em::EmTime::LengthToSeconds( EmTimeLength *pLength )
{
	int iTotalSeconds = 0;
	if(pLength == NULL)
	{
		return 0;
	}
	iTotalSeconds += pLength->iDays * 24 * 60 * 60;
	iTotalSeconds += pLength->iHours * 60 * 60;
	iTotalSeconds += pLength->iMiutes * 60;
	iTotalSeconds += pLength->iSeconds;
	return iTotalSeconds;
}

std::string em::EmTime::FixTimeStrLen( const char* szSrc )
{
	int iSrcLen = strlen(szSrc);
	char szDest[50] = {0};

	std::string strDest;
	strcpy(szDest,szSrc);
	if(iSrcLen == 4)
	{
		strcat(szDest,"-00-00 00:00:00");
	}
	else if(iSrcLen == 7)
	{
		strcat(szDest,"-00 00:00:00");
	}
	else if(iSrcLen == 10)
	{
		strcat(szDest," 00:00:00");
	}
	else if(iSrcLen == 13)
	{
		strcat(szDest,":00:00");
	}
	else if(iSrcLen == 16)
	{
		strcat(szDest,":00");
	}

	strDest	= szDest;
	return strDest;
}

bool em::EmTime::StrToObject( EmTime *pTime, const char* szSrc )
{
	int iSrcLen = strlen(szSrc);
	if(iSrcLen != 19
		|| pTime == NULL)
	{
		return false;
	}

	if(!EmSz::IsDateTime(szSrc))
	{
		return false;
	}

	char szYear[5] = {0};
	char szMonth[3] = {0};
	char szDate[3] = {0};
	char szHour[3] = {0};
	char szMinute[3] = {0};
	char szSecond[3] = {0};

	EmSz::SubStr(szYear,szSrc,0,4);
	EmSz::SubStr(szMonth,szSrc,5,2);
	EmSz::SubStr(szDate,szSrc,8,2);
	EmSz::SubStr(szHour,szSrc,11,2);
	EmSz::SubStr(szMinute,szSrc,14,2);
	EmSz::SubStr(szSecond,szSrc,17,2);

	pTime->SetYear(EmSz::Str2Int( szYear ));
	pTime->SetMonth(EmSz::Str2Int( szMonth ));
	pTime->SetDate(EmSz::Str2Int( szDate ));
	pTime->SetHour(EmSz::Str2Int( szHour ));
	pTime->SetMinute(EmSz::Str2Int( szMinute ));
	pTime->SetSecond(EmSz::Str2Int( szSecond ));
	pTime->Refresh();

	return true;
}

std::string em::EmTime::ToString()
{
	return Format("Y-m-d H:i:s");
}

int em::EmTime::StrToStamp( const char* szSrc )
{
	int iStamp = 0;
	EmTime xTime;
	bool bParseOk = false;
	if(!EmSz::IsDateTime(szSrc))
	{
		return 0;
	}
	bParseOk = StrToObject(&xTime,szSrc);
	if(bParseOk == false)
	{
		return 0;
	}
	iStamp = xTime.GetStamp();
	return iStamp;
}

std::string em::EmTime::StamptoStr( int iStamp )
{
	EmTime xTime;
	StampToObject(&xTime,iStamp);
	return xTime.ToString();
}

INT64 em::EmTime::CurrentStamp13()
{
	INT64 iResult = 0;
	EmTime xTime;
	xTime.LoadCurrentTime();
	iResult = xTime.GetStamp();
	iResult *= 1000;
	iResult += xTime.GetMillisec();
	return iResult;
}

bool em::EmTime::IsUnixStart()
{
	if(m_iYear != 1970){
		return false;
	}
	if(m_iMonth != 1){
		return false;
	}
	if(m_iDate != 1){
		return false;
	}
	if(m_iHour != 0){
		return false;
	}
	if(m_iMinute != 0){
		return false;
	}
	if(m_iSecond != 0){
		return false;
	}
	return true;
}

bool em::EmTime::SysTimeToObject( EmTime *pTime, const SYSTEMTIME* pSysTime )
{
	if(pSysTime == NULL || pTime == NULL){
		return false;
	}
	pTime->SetYear(pSysTime->wYear);
	pTime->SetMonth(pSysTime->wMonth);
	pTime->SetDate(pSysTime->wDay);
	pTime->SetHour(pSysTime->wHour);
	pTime->SetMinute(pSysTime->wMinute);
	pTime->SetSecond(pSysTime->wSecond);
	pTime->SetMillisec(pSysTime->wMilliseconds);
	pTime->Refresh();
	return true;
}

bool em::EmTime::FileTimeToObject( EmTime *pTime, const FILETIME* pFileTime )
{
	if(pFileTime == NULL || pTime == NULL){
		return false;
	}
	FILETIME xFtLastAccessLocal;
	FileTimeToLocalFileTime(pFileTime,&xFtLastAccessLocal);
	
	SYSTEMTIME xStLastAccess;
	FileTimeToSystemTime(&xFtLastAccessLocal,&xStLastAccess);

	return SysTimeToObject(pTime,&xStLastAccess);
}

bool em::EmTime::ObjectToSysTime( SYSTEMTIME* pSysTime, const EmTime *pTime )
{
	if(pSysTime == NULL || pTime == NULL){
		return false;
	}
	pSysTime->wYear = pTime->GetYear();
	pSysTime->wMonth = pTime->GetMonth();
	pSysTime->wDay = pTime->GetDate();
	pSysTime->wHour = pTime->GetHour();
	pSysTime->wMinute = pTime->GetMinute();
	pSysTime->wSecond = pTime->GetSecond();
	pSysTime->wMilliseconds = pTime->GetMillisec();
	pSysTime->wDayOfWeek = pTime->GetDay();
	return true;
}

bool em::EmTime::ObjectToFileTime( FILETIME* pFileTime, const EmTime *pTime )
{
	if(pFileTime == NULL || pTime == NULL){
		return false;
	}
	FILETIME xFileTimeLocal;
	SYSTEMTIME xSysTime;
	if(!ObjectToSysTime(&xSysTime,pTime)){
		return false;
	}
	SystemTimeToFileTime(&xSysTime,&xFileTimeLocal);
	LocalFileTimeToFileTime(&xFileTimeLocal,pFileTime);
	return true;


}





