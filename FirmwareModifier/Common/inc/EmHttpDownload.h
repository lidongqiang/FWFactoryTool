#ifndef _h_EmHttpDownload
#define _h_EmHttpDownload

#include "EmBase.h"

#include <string>

#include "IEmNetStreamReceiver.h"


namespace em
{

class EmHttpFileResponse;
class EmHttpDownload : public IEmNetStreamReceiver
{
public:

	EmHttpDownload();
	virtual ~EmHttpDownload();

	int AllInOne(const char* szRemoteUrl, const char* szLocalPath, int iMaxSize, IEmNetStreamReceiver *pReceiver = NULL, int iTimeout = 3000);

	void SetRemoteUrl(const char* szRemoteUrl);
	std::string GetRemoteUrl();

	void SetLocalPath(const char* szLocalPath);
	std::string GetLocalPath();

	void SetMaxSize(int iMaxSize);
	int GetMaxSize();

	void SetTimeout(int iTimeout);
	int GetTimeout();

	void SetStartPos(int iStartPos);
	int GetStartPos();

	void SetNeedSize(int iNeedSize);
	int GetNeedSize();

	void SetRecvOnceSize(int iRecvSize);
	int GetRecvOnceSize();

	void SetReceiver(IEmNetStreamReceiver* pReceiver);
	IEmNetStreamReceiver* GetReceiver();

	void FreeMem();

	int Start();

	void Stop();

	int Save();

	int GetContentLength();
	int GetSrcFileSize();
	void SetSrcFileSize(int iSrcFileSize);

	
	virtual int OnReceiveOverflow( const char *bufRecv, int iRecvLimit, int iRecvAlready );
	
	virtual int OnReceiveOnce( char *bufRecv, int iRecvLimit, int iRecvAlready, int iRecvLast );




private:

	std::string m_strRemoteUrl;
	std::string m_strLocalPath;
	int m_iMaxSize;
	int m_iTimeout;
	bool m_bComplete;
	bool m_bStarted;
	bool m_bNeedStop;


	EmHttpFileResponse* m_pResponse;
	IEmNetStreamReceiver *m_pReceiver;
	int m_iStartPos;
	int m_iNeedSize;
	int m_iTotalReceived;
	int m_iHeadLenth;
	int m_iSrcFileSize;
	int m_iRecvOnceSize;


};//class EmHttpDownload 

}//namespace em

#endif //#ifndef _EM_HTTP_DOWNLOAD_H