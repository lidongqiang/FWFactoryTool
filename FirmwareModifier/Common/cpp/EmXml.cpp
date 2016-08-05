#include "EmXml.h"
#include "EmSz.h"
#include "EmStlStr.h"
#include "EmFile.h"
#include "tinyxml.h"


using namespace em;


em::EmXml::EmXml()
{
	m_pDoc = new TiXmlDocument();
	m_bUpdated = false;
	m_bLoaded = false;
	m_eEncode = EmEncodeNone;
	m_strFileName = "";
}

em::EmXml::~EmXml()
{
	delete m_pDoc;
}

int em::EmXml::LoadFile( const char* szFileName, EmEncode eEnc )
{
	m_bUpdated = false;
	m_bLoaded = false;
	m_eEncode = EmEncodeNone;
	m_strFileName.empty();
	m_pDoc->Clear();

	if(!EmFile::IsFile(szFileName))
	{
		return EM_ERR_FILE_LOST;
	}

	std::string strFileContent = EmFile::ReadUtf8Str(szFileName,eEnc);
	
	strFileContent.append("\r\n");

	const char* szParseResult = m_pDoc->Parse(strFileContent.c_str(),0,TIXML_ENCODING_UTF8);
	if(szParseResult == NULL)
	{
		return EM_ERR_XML_PARSE;
	}

	m_strFileName = szFileName;
	m_eEncode = eEnc;
	m_bLoaded = true;

	return 1;

}

int em::EmXml::LoadText( const char* szText )
{
	m_bUpdated = false;
	m_bLoaded = false;
	m_eEncode = EmEncodeNone;
	m_strFileName.empty();
	m_pDoc->Clear();

	std::string strFileContent = szText;
	strFileContent.append("\r\n");
	const char* szParseResult = m_pDoc->Parse(strFileContent.c_str(),0,TIXML_ENCODING_UTF8);
	if(szParseResult == NULL)
	{
		return EM_ERR_XML_PARSE;
	}

	m_bLoaded = true;

	return 1;
}

int em::EmXml::Save()
{
	if(!m_bUpdated || !m_bLoaded || m_strFileName.length() == 0)
	{
		return 0;
	}

	TiXmlPrinter docPrinter;
	m_pDoc->Accept(&docPrinter);
	int iResult = EmFile::WriteUtf8(m_strFileName.c_str(),docPrinter.CStr(),m_eEncode);
	if(iResult < 0)
	{
		return iResult;
	}

	m_bUpdated = false;

	return 1;
}

int em::EmXml::SaveAs( const char* szFileName, EmEncode eEnc )
{
	TiXmlPrinter docPrinter;
	m_pDoc->Accept(&docPrinter);
	
	int iResult = EmFile::WriteUtf8(szFileName,docPrinter.CStr(),eEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	
	return 1;
}

std::string em::EmXml::GetText()
{
	std::string strResult;
	TiXmlPrinter docPrinter;
	m_pDoc->Accept(&docPrinter);
	strResult = docPrinter.CStr();
	return strResult;
}

std::string em::EmXml::ReadRootAttr( const char* szAttrName )
{
	std::string strResult;
	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return "";
	}
	
	const char* szAttrVal = pElemRoot->Attribute( szAttrName );
	if(szAttrVal == NULL)
	{
		return "";
	}
	
	strResult = szAttrVal;
	return strResult;

}

int em::EmXml::ReadRootAttrMap( EmMapStr &rMapStr )
{
	int iResult = 0;
	rMapStr.clear();

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	TiXmlAttribute *pAttr = pElemRoot->FirstAttribute();
	while(true)
	{
		if(pAttr == NULL)
		{
			break;
		}
		rMapStr[  pAttr->Name() ] =  pAttr->Value() ;
		pAttr = pAttr->Next();
	}
	iResult = rMapStr.size();
	return iResult;
}

int em::EmXml::ReadRootChildAttrList( EmVecMapStr &rVecMapStr )
{
	int iResult = 0;
	rVecMapStr.clear();

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	TiXmlElement *pElemChild = pElemRoot->FirstChildElement();
	TiXmlAttribute *pAttrChild = NULL;
	EmMapStr MapStr;
	while(true)
	{
		if(pElemChild == NULL)
		{
			break;
		}
		
		if(pElemChild->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			continue;
		}
		
		MapStr.clear();
		
		pAttrChild = pElemChild->FirstAttribute();
		while(true)
		{
			if(pAttrChild == NULL)
			{
				break;
			}
			
			MapStr[ pAttrChild->Name() ] = pAttrChild->Value();
			
			pAttrChild = pAttrChild->Next();
		}
		
		rVecMapStr.push_back(MapStr);
		pElemChild = pElemChild->NextSiblingElement();
	}
	
	iResult = rVecMapStr.size();
	
	return iResult;
}

std::string em::EmXml::ReadRootChildAttr( const char* szKeyName, const char* szKeyValue, const char* szAttrName )
{
	std::string strResult;
	int iResult = 0;

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return "";
	}
	
	TiXmlElement *pElemChild = pElemRoot->FirstChildElement();
	TiXmlAttribute *pAttrChild = NULL;
	
	while(true)
	{
		if(pElemChild == NULL)
		{
			break;
		}
		
		if(pElemChild->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			continue;
		}
		
		pAttrChild = pElemChild->FirstAttribute();
		
		if( EmSz::Equal(pElemChild->Attribute( szKeyName ), szKeyValue) )
		{
			strResult = pElemChild->Attribute( szAttrName);
			break;
		}
		
		pElemChild = pElemChild->NextSiblingElement();
	}
	return strResult;
}

int em::EmXml::ReadRootChildAttrMap( const char* szKeyName, const char* szKeyValue, EmMapStr &rMapStr )
{
	int iResult = 0;
	rMapStr.clear();

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	TiXmlElement *pElemChild = pElemRoot->FirstChildElement();
	TiXmlAttribute *pAttrChild = NULL;
	while(true)
	{
		if(pElemChild == NULL)
		{
			break;
		}
		
		if(pElemChild->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			continue;
		}
		
		if(EmSz::Equal( pElemChild->Attribute( szKeyName) , szKeyValue))
		{
			pAttrChild = pElemChild->FirstAttribute();
			while(true)
			{
				if(pAttrChild == NULL)
				{
					break;
				}
				
				rMapStr[  pAttrChild->Name() ] = pAttrChild->Value();
				pAttrChild = pAttrChild->Next();
			}
			break;
		}
		
		pElemChild = pElemChild->NextSiblingElement();
	}
	iResult = rMapStr.size();
	
	return iResult;
}

int em::EmXml::WriteRootAttr( const char* szAttrName, const char* szAttrValue )
{
	int iResult = 0;
	
	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return EM_ERR_XML_LOST_ROOT;
	}
	pElemRoot->SetAttribute(szAttrName,szAttrValue);

	m_bUpdated = true;
	
	return iResult;
}

int em::EmXml::WriteRootAttrMap( EmMapStr &rMapStr )
{
	int iResult = 0;	
	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return EM_ERR_XML_LOST_ROOT;
	}
	
	EmMapStr::const_iterator iterMap;
	for(iterMap = rMapStr.begin(); iterMap != rMapStr.end(); iterMap++)
	{
		pElemRoot->SetAttribute(iterMap->first, iterMap->second);
	}

	m_bUpdated = true;

	return iResult;
}

int em::EmXml::WriteRootChildAttr( const char* szKeyName, const char* szKeyValue, const char* szAttrName, const char* szAttrValue )
{
	std::string strResult;
	bool bUpdated = false;
	int iResult = 0;

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	TiXmlElement *pElemChild = pElemRoot->FirstChildElement();
	TiXmlAttribute *pAttrChild = NULL;
	
	while(true)
	{
		if(pElemChild == NULL)
		{
			break;
		}
		
		if(pElemChild->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			continue;
		}
		
		pAttrChild = pElemChild->FirstAttribute();
		
		if(EmSz::Equal( pElemChild->Attribute(szKeyName), szKeyValue))
		{
			pElemChild->SetAttribute(szAttrName,szAttrValue);
			bUpdated = true;
			break;
		}
		
		pElemChild = pElemChild->NextSiblingElement();
	}

	m_bUpdated = bUpdated;
	return iResult;
}

int em::EmXml::WriteRootChildAttrMap( const char* szKeyName, const char* szKeyValue, EmMapStr &rMapStr )
{
	std::string strResult;
	bool bUpdated = false;
	int iResult = 0;

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	TiXmlElement *pElemChild = pElemRoot->FirstChildElement();
	TiXmlAttribute *pAttrChild = NULL;
	
	while(true)
	{
		if(pElemChild == NULL)
		{
			break;
		}
		
		if(pElemChild->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			continue;
		}
		
		pAttrChild = pElemChild->FirstAttribute();
		
		if(EmSz::Equal( pElemChild->Attribute( szKeyName), szKeyValue))
		{
			EmMapStr::const_iterator iterMap;
			for(iterMap = rMapStr.begin(); iterMap != rMapStr.end(); iterMap++)
			{
				pElemChild->SetAttribute(iterMap->first , iterMap->second);
			}
			bUpdated = true;
			break;
		}
		
		pElemChild = pElemChild->NextSiblingElement();
	}

	m_bUpdated = bUpdated;
	return iResult;
}

int em::EmXml::AppendRootChild( const char* szChildName, EmMapStr &rMapStr )
{
	std::string strResult;
	bool bUpdated = false;
	int iResult = 0;

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	TiXmlElement *pElemChildNew =  new TiXmlElement(szChildName);
	EmMapStr::const_iterator iterMap;
	for(iterMap = rMapStr.begin(); iterMap != rMapStr.end(); iterMap++)
	{
		pElemChildNew->SetAttribute( iterMap->first, iterMap->second );
	}
	
	pElemRoot->LinkEndChild(pElemChildNew);

	m_bUpdated = true;

	return iResult;
}

int em::EmXml::AppendRootChildList( const char* szChildName, EmVecMapStr &rVecMapStr )
{
	std::string strResult;
	bool bUpdated = false;
	int iResult = 0;
	int i = 0;

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	if(rVecMapStr.size() == 0)
	{
		return 0;
	}
	
	for(i = 0; i < rVecMapStr.size(); i++)
	{
		TiXmlElement *pElemChildNew = new TiXmlElement(szChildName);
		EmMapStr::const_iterator iterMap;
		for(iterMap = rVecMapStr[i].begin(); iterMap != rVecMapStr[i].end(); iterMap++)
		{
			pElemChildNew->SetAttribute(iterMap->first,iterMap->second);
		}
		
		pElemRoot->LinkEndChild(pElemChildNew);
	}

	m_bUpdated = true;
	return iResult;
}

int em::EmXml::RemoveRootChild( const char* szChildName, const char* szKeyName, const char* szKeyValue )
{
	bool bUpdated = false;
	int iResult = 0;

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	TiXmlElement *pElemChild = pElemRoot->FirstChildElement();
	TiXmlAttribute *pAttrChild = NULL;
	
	while(true)
	{
		if(pElemChild == NULL)
		{
			break;
		}
		
		if(pElemChild->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			continue;
		}
		
		pAttrChild = pElemChild->FirstAttribute();
		
		if(EmSz::Equal( pElemChild->Attribute( szKeyName ), szKeyValue))
		{
			if(EmSz::Equal(pElemChild->Value(),szChildName))
			{
				pElemRoot->RemoveChild(pElemChild);
				bUpdated = true;
				break;
			}
			
		}
		
		pElemChild = pElemChild->NextSiblingElement();
	}

	m_bUpdated = bUpdated;

	return iResult;
}

int em::EmXml::ClearRootChild()
{
	bool bUpdated = false;
	int iResult = 0;

	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	
	while(pElemRoot->FirstChildElement())
	{
		pElemRoot->RemoveChild(pElemRoot->FirstChildElement());
		bUpdated = true;
	}

	m_bUpdated = bUpdated;
	return iResult;
	
}

int em::EmXml::ClearRootAttr()
{
	bool bUpdated = false;
	int iResult = 0;
	TiXmlElement *pElemRoot = m_pDoc->RootElement();
	if(pElemRoot == NULL)
	{
		return 0;
	}
	
	
	while(pElemRoot->FirstAttribute())
	{
		pElemRoot->RemoveAttribute(pElemRoot->FirstAttribute()->Name());
		bUpdated = true;
	}

	m_bUpdated = bUpdated;
	return iResult;
}


int em::EmXml::SCreateEmptyDoc(const char* szRootName ,  const char* szFileName, EmEncode enEnc)
{
	EmFile::DelFile(szFileName);
	std::string strDoc;
	strDoc.append("<?xml ");
	strDoc.append(" version=\"1.0\" ");
	if(enEnc == EmEncodeUtf8){
		strDoc.append(" encoding=\"utf-8\" ");
	}else if(enEnc == EmEncodeUnicode){
		strDoc.append(" encoding=\"unicode\" ");
	}else if(enEnc == EmEncodeAnsi){
		strDoc.append(" encoding=\"gb2312\" ");
	}
	strDoc.append("?>");
	strDoc.append("<");
	strDoc.append(szRootName);
	strDoc.append("/>");
	EmFile::WriteUtf8(szFileName,strDoc.c_str(),enEnc);
	return 0;
}

std::string em::EmXml::SReadRootAttr( const char* szAttrName, const char* szFileName, EmEncode enEnc)
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return "";
	}
	strResult = xXml.ReadRootAttr(szAttrName);
	return strResult;
}

int em::EmXml::SReadRootAttrMap( EmMapStr &rMapStr, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.ReadRootAttrMap(rMapStr);
	return iResult;
}

int em::EmXml::SReadRootChildAttrList( EmVecMapStr &rVecMapStr, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.ReadRootChildAttrList(rVecMapStr);
	return iResult;
}

std::string em::EmXml::SReadRootChildAttr( const char* szKeyName, const char* szKeyValue, const char* szAttrName
									  , const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return "";
	}
	strResult = xXml.ReadRootChildAttr(szKeyName,szKeyValue,szAttrName);
	return strResult;
}

int em::EmXml::SReadRootChildAttrMap( const char* szKeyName, const char* szKeyValue, EmMapStr &rMapStr, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.ReadRootChildAttrMap(szKeyName,szKeyValue,rMapStr);
	return iResult;
}

int em::EmXml::SWriteRootAttr( const char* szAttrName, const char* szAttrValue, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.WriteRootAttr(szAttrName,szAttrValue);
	xXml.Save();
	return iResult;
}

int em::EmXml::SWriteRootAttrMap( EmMapStr &rMapStr, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.WriteRootAttrMap(rMapStr);
	xXml.Save();
	return iResult;
}

int em::EmXml::SWriteRootChildAttr( const char* szKeyName, const char* szKeyValue, const char* szAttrName, const char* szAttrValue, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.WriteRootChildAttr(szKeyName,szKeyValue,szAttrName,szAttrValue);
	xXml.Save();
	return iResult;
}

int em::EmXml::SWriteRootChildAttrMap( const char* szKeyName, const char* szKeyValue, EmMapStr &rMapStr, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.WriteRootChildAttrMap(szKeyName,szKeyValue,rMapStr);
	xXml.Save();
	return iResult;
}

int em::EmXml::SAppendRootChild( const char* szChildName, EmMapStr &rMapStr, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.AppendRootChild(szChildName,rMapStr);
	xXml.Save();
	return iResult;
}

int em::EmXml::SAppendRootChildList( const char* szChildName, EmVecMapStr &rVecMapStr, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.AppendRootChildList(szChildName,rVecMapStr);
	xXml.Save();
	return iResult;
}

int em::EmXml::SRemoveRootChild( const char* szChildName, const char* szKeyName, const char* szKeyValue, const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.RemoveRootChild(szChildName,szKeyName,szKeyValue);
	xXml.Save();
	return iResult;
}

int em::EmXml::SClearRootChild( const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.ClearRootChild();
	xXml.Save();
	return iResult;
}

int em::EmXml::SClearRootAttr( const char* szFileName, EmEncode enEnc )
{
	int iResult = 0;
	std::string strResult;
	EmXml xXml;
	iResult = xXml.LoadFile(szFileName,enEnc);
	if(iResult < 0)
	{
		return iResult;
	}
	iResult = xXml.ClearRootAttr();
	xXml.Save();
	return iResult;
}
