#ifndef _h_EmHttpFileResponse
#define _h_EmHttpFileResponse

#include "EmBase.h"

#include <string>



namespace em
{

class EmHttpFileResponse
{

private:
	int m_iLimit;
	char *m_bufTotal;
	char *m_bufHead;
	char *m_bufContent;
	bool m_bAlloc;
	int m_iContentLen;
	int m_iTotalLen;
	int m_iHeadLen;
	int m_iRecvOnceSize;
public:

	EmHttpFileResponse();
	~EmHttpFileResponse();

	char * AllocMem(int iLimit);
	int FreeMem();
	int SetTotalBuffer(const char * buf);
	std::string GetHeadText();
	char *GetContentBuffer();
	char *GetTotalBuffer();
	int GetStatusCode();
	int GetContentLen();
	int GetHeadLen();
	int Compute();
	void Clear();
	void SetTotalLen(int iLen);
	void SetRecvOnceSize(int iRecvOnceSize);
	int GetTotalLen();
	bool HasAllocMem();

};//class EmHttpFileResponse

}//namespace em

#endif//#ifndef _EM_HTTP_FILE_RESPONSE_H