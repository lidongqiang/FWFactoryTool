#include "EmTcpPackReader.h"
#include "EmTcpBase.h"
#include "EmSz.h"
#include "EmFile.h"
#include "EmStlStr.h"
#include "EmHandy.h"
#include "IEmStateNoticer.h"

using namespace em;
using namespace std;

em::EmTcpPackReader::EmTcpPackReader()
{
	m_pRunningStateNoticer = NULL;
	m_iWorkSide = 0;

	m_bUsingMiddleBuffer = false;
	m_bHasTempFile = false;
	m_bDeleteTempFile = false;

	m_bHolderContinue = false;
	m_bHeadContinue = false;
	m_bMetaContinue = false;
	m_bContentContinue = false;
	m_bTailContinue = false;
	
	m_iKey = 0;
	m_iFileTotalSize = 0;
	m_iFileOffset = 0;
	m_iFilePartSize = 0;
	
	m_iPackMetaSize = 0;
	m_iPackContentSize = 0;

	m_iHolderHeadSize = 0;
	m_iHolderMetaSize = 0;
	m_iHolderContentSize = 0;
	m_iHolderTailSize = 0;

	m_iTempCompleteSize = 0;
	
	try{
		m_bufHolderHead = (char*)malloc(EM_TCP_PACK_HEAD_SIZE + 1);
		m_bufHolderMeta = (char*)malloc(EM_TCP_PACK_FILE_META_SIZE + 1);
		m_bufHolderContent = (char*)malloc(EM_TCP_PACK_READER_BUF_SIZE + 1);
		m_bufHolderTail = (char*)malloc(EM_TCP_PACK_BOUND_SIZE + 1);
		
		memset(m_bufHolderHead,0,EM_TCP_PACK_HEAD_SIZE + 1);
		memset(m_bufHolderMeta,0,EM_TCP_PACK_HEAD_SIZE + 1);
		memset(m_bufHolderContent,0,EM_TCP_PACK_READER_BUF_SIZE + 1);
		memset(m_bufHolderTail,0,EM_TCP_PACK_BOUND_SIZE + 1);

	}catch(...){
	}
}

em::EmTcpPackReader::~EmTcpPackReader()
{
	EM_FREE_NULL(m_bufHolderHead);
	EM_FREE_NULL(m_bufHolderMeta);
	EM_FREE_NULL(m_bufHolderContent);
	EM_FREE_NULL(m_bufHolderTail);
}

void em::EmTcpPackReader::ResetHolder()
{
	m_strHolderCommand = "";
	m_strHolderCharset = "";

	m_bHolderContinue = false;
	m_bHeadContinue = false;
	m_bMetaContinue = false;
	m_bContentContinue = false;
	m_bTailContinue = false;

	m_iPackContentSize = 0;
	m_iPackMetaSize = 0;

	m_iHolderHeadSize = 0;
	m_iHolderMetaSize = 0;
	m_iHolderContentSize = 0;
	m_iHolderTailSize = 0;
}

void em::EmTcpPackReader::BeginHolder()
{
	m_strHolderCommand = "";
	m_strHolderCharset = "";

	m_bHolderContinue = true;
	m_bHeadContinue = true;
	m_bMetaContinue = false;
	m_bContentContinue = false;
	m_bTailContinue = false;
	
	m_iPackContentSize = 0;
	m_iPackMetaSize = 0;

	m_iHolderContentSize = 0;
	m_iHolderHeadSize = 0;
	m_iHolderTailSize = 0;
	m_iHolderMetaSize = 0;
}

void em::EmTcpPackReader::SetKey( INT64 iKey )
{
	m_iKey = iKey;
}

void em::EmTcpPackReader::SetTempDir( const char* szDir )
{
	m_strTempDir = EmStlStr::FileFixDir(szDir);
	EmFile::MakeDir(m_strTempDir.c_str());
}

void em::EmTcpPackReader::SetDeleteTempFile( bool bDelete )
{
	m_bDeleteTempFile = bDelete;
}

int em::EmTcpPackReader::ProcessBytes( const char* bufRecv, int iCount, INT64 iKey)
{
	m_iKey = iKey;
	int iResult = 0;

// 	if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 		std::string strTemp;
// 		strTemp.append("\r\nProcessBytes:");
// 		strTemp.append(EmStlStr::Int2Str(iCount));
// 		EmHandy::DebugTraceFile("c:/tcp.txt",strTemp.c_str());
// 	}

	iResult = ProcessBytesBufferToPack(bufRecv,iCount,iKey);
	
	return iResult;
}

int em::EmTcpPackReader::ProcessBytesBufferToPack( const char* bufRecv, int iCount, INT64 iKey )
{
	int iResult = 0;
	int iOffset = 0;
	int iComplete = 0;
	int iCountAtZero = 0;

	while(true){

		if(m_pRunningStateNoticer != NULL){
			if(m_pRunningStateNoticer->GetState() == 0){
				break;
			}
		}
		if(iComplete > iCount){
			break;
		}

		if(iComplete == iCount){
			if(iCountAtZero++ > 1){
				break;
			}
		}

		m_iTempCompleteSize = 0;
		iResult = ProcessBytesInsidePack(bufRecv + iOffset,iCount - iOffset,iKey);
		if(iResult == EM_TCP_ACTION_CONNECTION_CLOSE
			|| iResult == EM_TCP_ACTION_RECEIVE_NEXT
		){
			break;
		}
		iComplete = iOffset + m_iTempCompleteSize;
		iOffset = iComplete;
	}
	
	return iResult;
}

int em::EmTcpPackReader::ProcessBytesInsidePack( const char* bufRecv, int iCount, INT64 iKey)
{
	string strTemp;
	
	int iResult = 0;

	string strCommand = "";
	string strSecure = "";
	string strLength = "";
	string strCharset = "";
	string strMetaSize = "";
	string strMetaInfo = "";
	string strFileName;

	string strMetaFileTotalSize = "";
	string strMetaFilePartSize = "";
	string strMetaFileOffset = "";
	string strMetaFileName = "";

	int iMetaFileTotalSize = 0;
	int iMetaFileOffset = 0;
	int iMetaFilePartSize = 0;

	int iPackContentSize = 0;
	int iPackMetaSize = 0;


	int iCopySize = 0;
	int iDiff = 0;
	
	int iFileCurrentSize = 0;

	if(iCount == 0 && !m_bHolderContinue){
		m_iTempCompleteSize = 0;
		return EM_TCP_ACTION_RECEIVE_NEXT;
	}

	try{
		if(iCount >= EM_TCP_PACK_BOUND_SIZE){

			if(!m_bHolderContinue){

				if(memcmp(bufRecv,EM_TCP_PACK_BEGIN_STR,EM_TCP_PACK_BOUND_SIZE) == 0){

					BeginHolder();
					m_iTempCompleteSize = 0;
					return EM_TCP_ACTION_DEFAULT;
				}

			}
		}else{

			if(!m_bHolderContinue){
				if(bufRecv[0] == EM_TCP_PACK_BEGIN_STR[0]){

					BeginHolder();
					m_iTempCompleteSize = 0;
					return EM_TCP_ACTION_DEFAULT;
				}else{

					OnBrokenPack(bufRecv,iCount);
					m_iTempCompleteSize = iCount;
					ResetHolder();
					return EM_TCP_ACTION_DEFAULT;
				}
			}
		}
		
		if(m_bHolderContinue)
		{
			if(m_bTailContinue){

// 				if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 					EmHandy::DebugTraceFile("c:/tcp.txt","m_bTailContinue");
// 				}

				iDiff = EM_TCP_PACK_BOUND_SIZE - m_iHolderTailSize;
				if(iDiff > 0){

					iCopySize = iCount < iDiff ? iCount : iDiff;
					memcpy(m_bufHolderTail + m_iHolderTailSize,bufRecv,iCopySize);
					m_iHolderTailSize += iCopySize;
					m_iTempCompleteSize = iCopySize;
					return EM_TCP_ACTION_DEFAULT;
				}else{


					if(memcmp(m_bufHolderTail,EM_TCP_PACK_END_STR,EM_TCP_PACK_BOUND_SIZE) != 0){
						
						OnBrokenPack(bufRecv,iCount);
						m_iTempCompleteSize = iCount;
						ResetHolder();
						return EM_TCP_ACTION_DEFAULT;
					}

					if(m_strHolderCommand.compare(EM_TCP_CMD_FILE_STR) == 0){

						iFileCurrentSize = EmFile::Size(m_strHolderFileName.c_str());

// 						if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 							EmHandy::DebugTraceFile("c:/tcp.txt","FileSize:");
// 							EmHandy::DebugTraceFile("c:/tcp.txt",EmStlStr::Int2Str(iFileCurrentSize).c_str());
// 						}

						if(iFileCurrentSize == m_iFileTotalSize){
							iResult = OnCmdFile(m_strHolderFileName.c_str(),m_iFileTotalSize,m_iKey);
						}
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_RAW_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_TEXT_STR) == 0){

						m_bufHolderContent[m_iPackContentSize] = 0;
						iResult = OnCmdText(m_bufHolderContent,m_iKey);
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_BEAT_STR) == 0){
						OnCmdBeat(m_iKey);
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_CLOSE_STR) == 0){
						OnCmdClose(m_iKey);

						m_iTempCompleteSize = 0;
						return EM_TCP_ACTION_CONNECTION_CLOSE;
					}

					if(m_bDeleteTempFile 
						&& EmStlStr::Find(m_strHolderFileName.c_str(),".tmp") > 0
						&& EmStlStr::Find(m_strHolderFileName.c_str(),m_strTempDir.c_str()) >= 0
					){
						EmFile::DelFile(m_strHolderFileName.c_str());
					}

					m_iTempCompleteSize = 0;
					ResetHolder();
					return EM_TCP_ACTION_RECEIVE_NEXT;
				}
			}else if(m_bContentContinue){
				
// 				if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 					EmHandy::DebugTraceFile("c:/tcp.txt","m_bContentContinue");
// 				}

				iDiff = m_iPackContentSize - m_iHolderContentSize;

				if(iDiff > 0){
					iCopySize = iCount < iDiff ? iCount : iDiff;
					if(iCount == 0){
						m_iTempCompleteSize = 0;
						return EM_TCP_ACTION_RECEIVE_NEXT;
					}

					if(m_strHolderCommand.compare(EM_TCP_CMD_FILE_STR) == 0){

						iFileCurrentSize = EmFile::Size(m_strHolderFileName.c_str());
						
// 						if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 							EmHandy::DebugTraceFile("c:/tcp.txt","content filesize:");
// 							EmHandy::DebugTraceFile("c:/tcp.txt",EmStlStr::Int2Str(iFileCurrentSize).c_str());
// 							EmHandy::DebugTraceFile("c:/tcp.txt",m_strHolderFileName.c_str());
// 						}

						if(iFileCurrentSize == m_iFileOffset + m_iHolderContentSize){

// 							if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 								EmHandy::DebugTraceFile("c:/tcp.txt","copy file:");
// 							}

							iResult = EmFile::AppendBuffer(m_strHolderFileName.c_str()
								,bufRecv,iCopySize);

// 							if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 								EmHandy::DebugTraceFile("c:/tcp.txt",EmStlStr::Int2Str(iResult).c_str());
// 							}

							OnProgressFile(m_strHolderFileName.c_str()
									,m_iFileTotalSize,iFileCurrentSize + iCopySize,iCopySize,iKey);
						}

						iFileCurrentSize = EmFile::Size(m_strHolderFileName.c_str());

// 						if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 							EmHandy::DebugTraceFile("c:/tcp.txt","content filesize2:");
// 							EmHandy::DebugTraceFile("c:/tcp.txt",EmStlStr::Int2Str(iFileCurrentSize).c_str());
// 						}
						
						
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_RAW_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_TEXT_STR) == 0){
						memcpy(m_bufHolderContent + m_iHolderContentSize,bufRecv,iCopySize);
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_BEAT_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_CLOSE_STR) == 0){
					}

					m_iHolderContentSize += iCopySize;

// 					if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 						EmHandy::DebugTraceFile("c:/tcp.txt","m_iPackContentSize,m_iHolderContentSize");
// 						EmHandy::DebugTraceFile("c:/tcp.txt"
// 							,(EmStlStr::Int2Str(m_iPackContentSize) + "," + EmStlStr::Int2Str(m_iHolderContentSize)).c_str());
// 					}

					m_iTempCompleteSize = iCopySize;
					return EM_TCP_ACTION_DEFAULT;
				}else{
					m_bContentContinue = false;
					m_iHolderContentSize = 0;
					m_bTailContinue = true;
					m_iHolderTailSize = 0;

					m_iTempCompleteSize = 0;
					return EM_TCP_ACTION_DEFAULT;
				}
 			}else if(m_bMetaContinue){

// 				if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 					EmHandy::DebugTraceFile("c:/tcp.txt","m_bMetaContinue");
// 				}

				iDiff = m_iPackMetaSize - m_iHolderMetaSize;
				if(iDiff > 0){
					iCopySize = iCount < iDiff ? iCount : iDiff;
					memcpy(m_bufHolderMeta + m_iHolderMetaSize,bufRecv,iCopySize);
					m_iHolderMetaSize += iCopySize;
					m_iTempCompleteSize = iCopySize;
					return EM_TCP_ACTION_DEFAULT;
				}else{
					strMetaInfo.append(m_bufHolderMeta,EM_TCP_PACK_FILE_META_SIZE);

					if(EmStlStr::Equal(m_strHolderCommand.c_str(),EM_TCP_CMD_FILE_STR)){
						int iPosDash1 = -1;
						int iPosDash2 = -1;
						int iPosDash3 = -1;
						iPosDash1 = EmStlStr::FindFrom(strMetaInfo.c_str(),"-",0);
						if(iPosDash1 > 0){
							iPosDash2 = EmStlStr::FindFrom(strMetaInfo.c_str(),"-",iPosDash1 + 1);
							if(iPosDash2 > 0){
								iPosDash3 = EmStlStr::FindFrom(strMetaInfo.c_str(),"-",iPosDash2 + 1);
							}
						}

						if(iPosDash1 == -1
							|| iPosDash1 == -1
							|| iPosDash1 == -1)
						{
							OnBrokenPack(bufRecv,iCount);
							m_iTempCompleteSize = iCount;
							ResetHolder();
							return EM_TCP_ACTION_RECEIVE_NEXT;
						}

						strMetaFileTotalSize = EmStlStr::SubStr(strMetaInfo.c_str(),0,iPosDash1);
						strMetaFileOffset = EmStlStr::SubStr(strMetaInfo.c_str(),iPosDash1 + 1,iPosDash2 - iPosDash1 - 1);
						strMetaFilePartSize = EmStlStr::SubStr(strMetaInfo.c_str(),iPosDash2 + 1,iPosDash3 - iPosDash2 - 1);
						strMetaFileName = EmStlStr::SubStr(strMetaInfo.c_str(),iPosDash3 + 1,-1);
						strMetaFileName = EmStlStr::Trim(strMetaFileName.c_str());
						
						iMetaFileTotalSize = EmStlStr::Str2Int(strMetaFileTotalSize.c_str());
						iMetaFileOffset = EmStlStr::Str2Int(strMetaFileOffset.c_str());
						iMetaFilePartSize = EmStlStr::Str2Int(strMetaFilePartSize.c_str());

						m_iFileTotalSize = iMetaFileTotalSize;
						m_iFileOffset = iMetaFileOffset;
						m_iFilePartSize = iMetaFilePartSize;

						
						if(strMetaFileName.length() == 0){
							if(m_iFileOffset == 0){
								m_strHolderFileName = EmHandy::GenerateFileName(m_strTempDir.c_str(),".tmp");
							}
						}else{
							m_strHolderFileName = strMetaFileName;
						}

						if(m_iFileOffset == 0){
// 							if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 								EmHandy::DebugTraceFile("c:/tcp.txt","MakeFile:");
// 								EmHandy::DebugTraceFile("c:/tcp.txt",m_strHolderFileName.c_str());
// 							}
							EmFile::MakeFile(m_strHolderFileName.c_str(),true);
						}
						
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_RAW_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_TEXT_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_BEAT_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_CLOSE_STR) == 0){
					}

					m_bMetaContinue = false;
					m_bContentContinue = true;
					m_iHolderMetaSize = 0;
					m_iHolderContentSize = 0;
					m_iTempCompleteSize = 0;
					return EM_TCP_ACTION_DEFAULT;

				}
			}else if(m_bHeadContinue){
// 				if(m_iWorkSide == EM_TCP_SIDE_SERVER){
// 					EmHandy::DebugTraceFile("c:/tcp.txt","m_bHeadContinue");
// 				}
				iDiff = EM_TCP_PACK_HEAD_SIZE - m_iHolderHeadSize;
				if(iDiff > 0){
					iCopySize = iCount < iDiff ? iCount : iDiff;
					memcpy(m_bufHolderHead + m_iHolderHeadSize,bufRecv,iCopySize);
					m_iHolderHeadSize += iCopySize;
					m_iTempCompleteSize = iCopySize;
					return EM_TCP_ACTION_DEFAULT;
					
				}else{
					if(memcmp(m_bufHolderHead,EM_TCP_PACK_BEGIN_STR,EM_TCP_PACK_BOUND_SIZE) != 0){
						OnBrokenPack(bufRecv,iCount);
						m_iTempCompleteSize = iCount;
						ResetHolder();
						return EM_TCP_ACTION_RECEIVE_NEXT;
					}

					strCommand.append(m_bufHolderHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 0
						,EM_TCP_PACK_SECTION_SIZE);
					strCommand = EmStlStr::Replace(strCommand.c_str(),EM_TCP_PACK_PAD_STR,"");
					
					strSecure.append(m_bufHolderHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 1
						,EM_TCP_PACK_SECTION_SIZE);
					strSecure = EmStlStr::Replace(strSecure.c_str(),EM_TCP_PACK_PAD_STR,"");
					
					strLength.append(m_bufHolderHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 2
						,EM_TCP_PACK_SECTION_SIZE);
					strLength = EmStlStr::Replace(strLength.c_str(),EM_TCP_PACK_PAD_STR,"");
					
					strCharset.append(m_bufHolderHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 3
						,EM_TCP_PACK_SECTION_SIZE);
					strCharset = EmStlStr::Replace(strCharset.c_str(),EM_TCP_PACK_PAD_STR,"");

					strMetaSize.append(m_bufHolderHead + EM_TCP_PACK_BOUND_SIZE + EM_TCP_PACK_SECTION_SIZE * 4
						,EM_TCP_PACK_SECTION_SIZE);
					strMetaSize = EmStlStr::Replace(strMetaSize.c_str(),EM_TCP_PACK_PAD_STR,"");
					
					if(strLength.length() > 0){
						iPackContentSize = EmSz::Str2Int(strLength.c_str());
					}

					if(strMetaSize.length() > 0){
						iPackMetaSize = EmSz::Str2Int(strMetaSize.c_str());
					}
					
					m_iPackContentSize = iPackContentSize;
					m_iPackMetaSize = iPackMetaSize;
					
					m_strHolderCommand = strCommand;
					m_strHolderCharset = strCharset;
					
					if(m_strHolderCommand.compare(EM_TCP_CMD_FILE_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_RAW_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_TEXT_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_BEAT_STR) == 0){
					}else if(m_strHolderCommand.compare(EM_TCP_CMD_CLOSE_STR) == 0){
					}

					m_bHeadContinue = false;
					m_iHolderHeadSize = 0;
					m_bMetaContinue = true;
					m_iHolderMetaSize = 0;

					m_iTempCompleteSize = 0;
					return EM_TCP_ACTION_DEFAULT;
				}
			}
		}else{
			ResetHolder();
			OnBrokenPack(bufRecv,iCount);
			m_iTempCompleteSize = iCount;
			return EM_TCP_ACTION_RECEIVE_NEXT;
		}
		
	}catch(...){
		m_iTempCompleteSize = iCount;
	}

	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnCmdText( const char* szText, INT64 iKey )
{
	
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnCmdFile( const char* szFileName, int iFileSize, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnCmdRaw( const char* szFileName, int iFileSize, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnCmdBeat( INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnCmdClose( INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnProgressFile( const char* szTempFile, int iTotalSize, int iCurrentSize, int iOnceSize, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnCmdUnknown( const char* szCmd, INT64 iKey )
{
	return EM_TCP_ACTION_DEFAULT;
}

int em::EmTcpPackReader::OnBrokenPack( const char* bufPack, int iPackSize )
{
	return EM_TCP_ACTION_DEFAULT;
}

void em::EmTcpPackReader::SetRunningStateNoticer( IEmStateNoticer* pRunningStateNoticer )
{
	m_pRunningStateNoticer = pRunningStateNoticer;
}




