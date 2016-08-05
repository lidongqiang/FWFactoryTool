#ifndef _h_EmIniFile
#define _h_EmIniFile

#include "EmBase.h"

#include <map>
#include <vector>





namespace em
{

class EmIniFile 
{
public:

	EmIniFile();
	virtual ~EmIniFile();

	int Load(const char* szFileName, EmEncode enc);
	int Save();
	int SaveAs(const char* szFileName, EmEncode enc);
	int GetSectionNameList(std::vector<std::string> &rVec);
	int GetSectionMap(std::map<std::string,std::string> &rMap, const char* szSection = NULL);
	int GetKeyNameList(std::vector<std::string> &rVec, const char* szSection = NULL);
	std::string GetValue(const char* szKey, const char* szSection = NULL);
	int SetValue(const char* szKey, const char* szValue, const char* szSection = NULL);
	int AddSection(const char* szSection, std::map<std::string,std::string> &rMapNewSection);
	int DeleteSection(const char* szSection);
	std::string ToString();
	void Trace();
	void Clear();

private:
	typedef std::map<std::string, std::map<std::string,std::string> > t_ini_model;
	typedef std::map<std::string, std::string> t_ini_pair;

	std::string m_strFileName;
	EmEncode m_Encode;
	t_ini_model m_oModel;

};//class

}//namespace
#endif // !defined
