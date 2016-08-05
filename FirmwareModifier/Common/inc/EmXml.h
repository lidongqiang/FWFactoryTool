#ifndef _h_EmXml
#define _h_EmXml

#include "EmBase.h"

#include <string>
#include <map>
#include <vector>
#include "EmStlTypeDef.h"

class TiXmlDocument;
namespace em
{

class EmXml  
{
public:
	//////////////////////////////////////////////////////////////////////////
	//instance methods

	EmXml();
	virtual ~EmXml();

	int LoadFile(const char* szFileName, EmEncode eEnc);
	int LoadText(const char* szText);
	int Save();
	int SaveAs(const char* szFileName, EmEncode eEnc);
	std::string GetText();

	std::string ReadRootAttr(const char*  szAttrName);
	int ReadRootAttrMap(EmMapStr &rMapCs);
	int ReadRootChildAttrList(EmVecMapStr &rVecMapCs);
	std::string ReadRootChildAttr(const char*  szKeyName, const char*  szKeyValue, const char*  szAttrName);
	int ReadRootChildAttrMap(const char*  szKeyName, const char*  szKeyValue, EmMapStr &rMapCs);
	int WriteRootAttr(const char*  szAttrName, const char*  szAttrValue);
	int WriteRootAttrMap(EmMapStr &rMapCs);
	int WriteRootChildAttr(const char*  szKeyName, const char*  szKeyValue, const char*  szAttrName, const char*  szAttrValue);
	int WriteRootChildAttrMap(const char*  szKeyName, const char*  szKeyValue, EmMapStr &rMapCs);
	int AppendRootChild( const char*  szChildName, EmMapStr &rMapCs);
	int AppendRootChildList(const char*  szChildName, EmVecMapStr &rVecMapCs);
	int RemoveRootChild(const char*  szChildName, const char*  szKeyName, const char*  szKeyValue);
	int ClearRootChild();
	int ClearRootAttr();

public:
	static int SCreateEmptyDoc(const char*  szRootName, const char* szFileName, EmEncode enEnc);
	static std::string SReadRootAttr(const char*  szAttrName, const char* szFileName, EmEncode enEnc);
	static int SReadRootAttrMap(EmMapStr &rMapCs, const char* szFileName, EmEncode enEnc);
	static int SReadRootChildAttrList(EmVecMapStr &rVecMapCs, const char* szFileName, EmEncode enEnc);
	static std::string SReadRootChildAttr(const char*  szKeyName, const char*  szKeyValue, const char*  szAttrName, const char* szFileName, EmEncode enEnc);
	static int SReadRootChildAttrMap(const char*  szKeyName, const char*  szKeyValue, EmMapStr &rMapCs, const char* szFileName, EmEncode enEnc);
	
	static int SWriteRootAttr(const char*  szAttrName, const char*  szAttrValue, const char* szFileName, EmEncode enEnc);
	static int SWriteRootAttrMap(EmMapStr &rMapCs, const char* szFileName, EmEncode enEnc);
	static int SWriteRootChildAttr(const char*  szKeyName, const char*  szKeyValue, const char*  szAttrName, const char*  szAttrValue, const char* szFileName, EmEncode enEnc);
	static int SWriteRootChildAttrMap(const char*  szKeyName, const char*  szKeyValue, EmMapStr &rMapCs, const char* szFileName, EmEncode enEnc);
	static int SAppendRootChild( const char*  szChildName, EmMapStr &rMapCs, const char* szFileName, EmEncode enEnc);
	static int SAppendRootChildList(const char*  szChildName, EmVecMapStr &rVecMapCs, const char* szFileName, EmEncode enEnc);
	static int SRemoveRootChild(const char*  szChildName, const char*  szKeyName, const char*  szKeyValue, const char* szFileName, EmEncode enEnc);
	static int SClearRootChild(const char* szFileName, EmEncode enEnc);
	static int SClearRootAttr(const char* szFileName, EmEncode enEnc);

private:
	bool m_bUpdated;
	bool m_bLoaded;
	std::string m_strFileName;
	EmEncode m_eEncode;
	TiXmlDocument* m_pDoc;


};//class EmXml

}//namespace em

#endif // #ifndef EM_XML_H
