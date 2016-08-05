#ifndef _h_EmProcess
#define _h_EmProcess

#include "EmBase.h"

#include <string>

#include <afx.h>

namespace em
{
	class EmProcess
	{

	public:

		static bool Execute(const char* szAppName, const char* szCmd, unsigned short wShowWindow, bool bWait);

		static bool EnableDebugPrivilege();
		static long  FindProcessIdByName(const char* szFind);
		
		static bool TerminateProcessByName(const char* szProcessName, unsigned int nExitCode = 0);

		static int SendCommand(char* bufOutput, const char* szCmd);
        static int SendCommand(char** bufRawOutput, const char* utf8Cmd,  int nTimeoutCount);
		static std::string SendCommandUtf8( const char* szCmd);
		static std::string SendCommandAnsi( const char* szCmd);

		/*
		@return:count of window founded
		@szTitle:the WindowText property
		@iAction:0=close,1=quit(without warning,quit the app directly)
		*/
		static int CloseWindowByTitle(const char* szTitle, int iAction = 0);

		/* 
		get current directory 
		*/
		static std::string CurrentDir();

		/* 
		current application file short name,without directory 
		*/
		static std::string CurrentName();

		/* 
		current application file full path 
		*/
		static std::string CurrentPath();

	private:
		typedef struct tagEmCloseWindowParam
		{
			const char *szTitle;
			int *piWindowCount;
			int iAction;
		}EmCloseWindowParam;

		static bool CALLBACK CloseWindowByTitleProc(HWND hWnd, LPARAM lParam);
	};
}


#endif