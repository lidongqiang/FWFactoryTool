#ifndef _h_EmHttp
#define _h_EmHttp

#include "EmBase.h"

#include <map>
#include <string>



namespace em
{
	class EmHttpFileResponse;
	class EmHttpTextResponse;
	class IEmNetStreamReceiver;
	class IEmNetStreamSender;

	class EmHttp
	{
	public:

		static int GetTextByUrl(EmHttpTextResponse &rResponse, const char* szUrl
			, EmEncode enc =EmEncodeUtf8, int iReadLimit =4096, int iTimeoutMilli = 3000);

		static int PostTextByUrl(EmHttpTextResponse &rResponse, const char* szUrl, const char* szData
			, EmEncode enc =EmEncodeUtf8, int iReadLimit =4096, int iTimeoutMilli = 3000);

		static int GetFileByUrl(EmHttpFileResponse &rResponse, const char* szUrl
			,IEmNetStreamReceiver *pReceiver, int iReadOnceSize = 40960, int iReadLimit = 4096, int iTimeoutMilli = 3000);

		static int GetFileRangeByUrl(EmHttpFileResponse &rResponse, const char* szUrl, int iStartPos, int iSize
			,int iRecvOnceSize = 40960 ,IEmNetStreamReceiver *pReceiver = NULL, int iTimeoutMilli = 3000);

		static int GetFileSizeByUrl( const char* szUrl, int iTimeoutMilli = 3000);

		static int UploadFile(const char* szUrl, const char* szFileName, const char* szFileFormName
			, EmHttpFileResponse &rResponse, std::map<std::string,std::string> *pFormFields
			, IEmNetStreamSender *pSender = NULL, IEmNetStreamReceiver *pReceiver = NULL
			, int iSendOnceSize = 40960, int iRecvOnceSize = 40960
			, int iReadLimit = 4096, int iTimeoutMilli = 3000);

	};
}
#endif