#include "EmTcpPackWriter.h"
#include "EmSz.h"
#include "EmFile.h"
#include "EmHandy.h"
//#include <string>

using namespace em;
using namespace std;

bool EmTcpPackWriter::s_bStaticInited = false;
char* EmTcpPackWriter::s_bufBeatCache = NULL;
char* EmTcpPackWriter::s_bufCloseCache = NULL;

int em::EmTcpPackWriter::StaticInit()
{
	try{
		if(s_bStaticInited){
			return 0;
		}
		EM_FREE_NULL(s_bufBeatCache);
		EM_FREE_NULL(s_bufCloseCache);
		s_bufBeatCache = (char*)malloc(EM_TCP_PACK_MIN_SIZE + 1);
		s_bufCloseCache = (char*)malloc(EM_TCP_PACK_MIN_SIZE + 1);
		memset(s_bufBeatCache,0,EM_TCP_PACK_MIN_SIZE + 1);
		memset(s_bufCloseCache,0,EM_TCP_PACK_MIN_SIZE + 1);

		strcpy((char*)s_bufBeatCache, EM_TCP_PACK_BEGIN_STR);
		strcpy((char*)s_bufBeatCache + EM_TCP_PACK_BOUND_SIZE, EM_TCP_CMD_BEAT_STR);
		EmSz::PadRight((char*)s_bufBeatCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1);
		strcpy((char*)s_bufBeatCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1, "secure");
		EmSz::PadRight((char*)s_bufBeatCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2);
		strcpy((char*)s_bufBeatCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2, "0");
		EmSz::PadRight((char*)s_bufBeatCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3);
		strcpy((char*)s_bufBeatCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3, "utf-8");
		EmSz::PadRight((char*)s_bufBeatCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4);
		strcpy((char*)s_bufBeatCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4, "0");
		EmSz::PadRight((char*)s_bufBeatCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 5);
		strcpy((char*)s_bufBeatCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 5, EM_TCP_PACK_END_STR);

		strcpy((char*)s_bufCloseCache, EM_TCP_PACK_BEGIN_STR);
		strcpy((char*)s_bufCloseCache + EM_TCP_PACK_BOUND_SIZE, EM_TCP_CMD_CLOSE_STR);
		EmSz::PadRight((char*)s_bufCloseCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1);
		strcpy((char*)s_bufCloseCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1, "secure");
		EmSz::PadRight((char*)s_bufCloseCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2);
		strcpy((char*)s_bufCloseCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2, "0");
		EmSz::PadRight((char*)s_bufCloseCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3);
		strcpy((char*)s_bufCloseCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3, "utf-8");
		EmSz::PadRight((char*)s_bufCloseCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4);
		strcpy((char*)s_bufCloseCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4, "0");
		EmSz::PadRight((char*)s_bufCloseCache,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 5);
		strcpy((char*)s_bufCloseCache + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 5, EM_TCP_PACK_END_STR);
		s_bStaticInited = true;
	}catch(...){
		s_bStaticInited = false;
		return EM_ERR_MEM;
	}

	return 0;
}

int em::EmTcpPackWriter::StaticExit()
{
	try{
		EM_FREE_NULL(s_bufBeatCache);
		EM_FREE_NULL(s_bufCloseCache);
		s_bStaticInited = false;
	}catch(...){
		return EM_ERR_MEM;
	}
	return 0;
}

const char* em::EmTcpPackWriter::GetBeatBytes()
{
	return s_bufBeatCache;
}

const char* em::EmTcpPackWriter::GetCloseBytes()
{
	return s_bufCloseCache;
}

int em::EmTcpPackWriter::MakeTextBytes( char* bufHolder, int iContentSize )
{
	if(bufHolder == NULL){
		return EM_ERR_MEM_NULL;
	}
	char szHead[EM_TCP_PACK_HEAD_SIZE + 1] = {0};
	char szSize[50] = {0};
	EmSz::Int2Str(szSize,iContentSize);

	strcpy((char*)szHead, EM_TCP_PACK_BEGIN_STR);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE, EM_TCP_CMD_TEXT_STR);
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1, "secure");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2, szSize);
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3, "utf-8");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4, "0");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 5);

	memcpy(bufHolder,szHead,EM_TCP_PACK_HEAD_SIZE);
	memcpy(bufHolder + EM_TCP_PACK_HEAD_SIZE + iContentSize,EM_TCP_PACK_END_STR,EM_TCP_PACK_BOUND_SIZE);


	return iContentSize + EM_TCP_PACK_MIN_SIZE;
}

int em::EmTcpPackWriter::MakeRawBytes( char* bufHolder, int iContentSize )
{
	if(bufHolder == NULL){
		return EM_ERR_MEM_NULL;
	}

	char szHead[EM_TCP_PACK_HEAD_SIZE + 1] = {0};
	char szSize[50] = {0};
	EmSz::Int2Str(szSize,iContentSize);

	strcpy((char*)szHead, EM_TCP_PACK_BEGIN_STR);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE, EM_TCP_CMD_RAW_STR);
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1, "secure");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2, szSize);
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3, "utf-8");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4, "0");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 5);
	
	memcpy(bufHolder,szHead,EM_TCP_PACK_HEAD_SIZE);
	memcpy(bufHolder + EM_TCP_PACK_HEAD_SIZE + iContentSize,EM_TCP_PACK_END_STR,EM_TCP_PACK_BOUND_SIZE);

	return iContentSize + EM_TCP_PACK_MIN_SIZE;
}

int em::EmTcpPackWriter::MakeFileBytes( char* bufHolder, int iBufSize
			, const char* szLocalFileName, const char* szRemoteFileName, int iFileSize, int iFileOffset, int iPartSize )
{
	if(bufHolder == NULL){
		return EM_ERR_MEM_NULL;
	}

	int iReadSize = 0;
	int iPackSize = 0;

	char szHead[EM_TCP_PACK_HEAD_SIZE + 1] = {0};
	char szContentSize[50] = {0};
	char szMetaSize[50] = {0};
	char szMeta[EM_TCP_PACK_FILE_META_SIZE + 1] = {0};
	EmSz::Int2Str(szContentSize,iPartSize);
	EmSz::Int2Str(szMetaSize,EM_TCP_PACK_FILE_META_SIZE);

	int iTempMetaSize = 0;
	EmSz::Int2Str(szMeta,iFileSize);
	strcat(szMeta,"-");
	iTempMetaSize = strlen(szMeta);
	EmSz::Int2Str(szMeta + iTempMetaSize,iFileOffset);
	strcat(szMeta,"-");
	iTempMetaSize = strlen(szMeta);
	EmSz::Int2Str(szMeta + iTempMetaSize,iPartSize);
	strcat(szMeta,"-");
	iTempMetaSize = strlen(szMeta);
	strcat(szMeta,szRemoteFileName);

	//EmSz::PadRight((char*)szMeta,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_FILE_META_SIZE);

	strcpy((char*)szHead, EM_TCP_PACK_BEGIN_STR);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE, EM_TCP_CMD_FILE_STR);
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1, "secure");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2, szContentSize);
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3, "utf-8");
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4);
	strcpy((char*)szHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4, szMetaSize);
	EmSz::PadRight((char*)szHead,EM_TCP_PACK_PAD_STR[0],EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 5);

	EmSz::PadRight((char*)szMeta,' ',EM_TCP_PACK_FILE_META_SIZE);

	memcpy(bufHolder + iPackSize,szHead,EM_TCP_PACK_HEAD_SIZE);
	iPackSize += EM_TCP_PACK_HEAD_SIZE;

	memcpy(bufHolder + iPackSize,szMeta,EM_TCP_PACK_FILE_META_SIZE);
	iPackSize += EM_TCP_PACK_FILE_META_SIZE;


	iReadSize = EmFile::ReadBufferByPos(szLocalFileName,bufHolder + iPackSize, iFileOffset, iPartSize);
	if(iReadSize < iPartSize){
		return -1;
	}
	iPackSize += iPartSize;

	memcpy(bufHolder + iPackSize,EM_TCP_PACK_END_STR,EM_TCP_PACK_BOUND_SIZE);
	iPackSize += EM_TCP_PACK_BOUND_SIZE;

// 	string strTemp;
// 	strTemp.append(bufHolder,iPackSize);
// 	EmHandy::DebugTraceFile("c:/tcp.txt","file pack");
// 	EmHandy::DebugTraceFile("c:/tcp.txt",strTemp.c_str());

	return iPackSize;
}
