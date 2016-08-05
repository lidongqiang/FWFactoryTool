#ifndef _h_EmTcpPackReader
#define _h_EmTcpPackReader

#include "EmBase.h"
#include "EmTcpBase.h"

#include <string>

namespace em
{
	class IEmStateNoticer;
	class EmTcpPackReader
	{
	public:

		int m_iWorkSide;

		EmTcpPackReader();
		virtual ~EmTcpPackReader();

		void SetKey(INT64 iKey);
		void SetTempDir(const char* szDir);
		void SetDeleteTempFile(bool bDelete);
		void SetRunningStateNoticer(IEmStateNoticer* pRunningStateNoticer);

		void ResetHolder();
		void BeginHolder();

		int ProcessBytes(const char* bufRecv, int iCount, INT64 iKey);
		int ProcessBytesBufferToPack(const char* bufRecv, int iCount, INT64 iKey);
		int ProcessBytesInsidePack(const char* bufRecv, int iCount, INT64 iKey);
		
		virtual int OnCmdText(const char* szText, INT64 iKey);
		virtual int OnCmdFile(const char* szFileName, int iFileSize, INT64 iKey);
		virtual int OnCmdRaw(const char* szFileName, int iFileSize, INT64 iKey);
		virtual int OnCmdBeat(INT64 iKey);
		virtual int OnCmdClose(INT64 iKey);
		virtual int OnCmdUnknown(const char* szCmd, INT64 iKey);

		virtual int OnBrokenPack(const char* bufPack, int iPackSize);
		virtual int OnProgressFile(const char* szTempFile, int iTotalSize, int iCurrentSize, int iOnceSize, INT64 iKey);
		


	protected:


		IEmStateNoticer* m_pRunningStateNoticer;
		INT64 m_iKey;

		int m_iHolderHeadSize;
		int m_iHolderMetaSize;
		int m_iHolderContentSize;
		int m_iHolderTailSize;

		int m_iPackContentSize;
		int m_iPackMetaSize;

		int m_iFileOffset;
		int m_iFilePartSize;
		int m_iFileTotalSize;

		int m_iTempCompleteSize;

		bool m_bHolderContinue;
		bool m_bHeadContinue;
		bool m_bMetaContinue;
		bool m_bContentContinue;
		bool m_bTailContinue;

		bool m_bUsingMiddleBuffer;
		bool m_bHasTempFile;
		bool m_bDeleteTempFile;

		char* m_bufHolderHead;
		char* m_bufHolderMeta;
		char* m_bufHolderContent;
		char* m_bufHolderTail;

		std::string m_strHolderCommand;
		std::string m_strHolderFileName;
		std::string m_strHolderCharset;
		std::string m_strTempDir;

	};
}
#endif