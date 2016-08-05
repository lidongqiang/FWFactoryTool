#include "stdafx.h"
#include "../inc/EmIniFile.h"
#include "../inc/EmStlStr.h"
#include "../inc/EmFile.h"
#include "../inc/EmStr.h"

#include "../inc/EmHandy.h"

using namespace em;
using namespace std;


EmIniFile::EmIniFile()
{
}

EmIniFile::~EmIniFile()
{
}

int em::EmIniFile::Load( const char* szFileName, EmEncode enc)
{
	if(!EmFile::IsExisted(szFileName))
	{
		return -1;
	}

	m_oModel.clear();
	m_strFileName = szFileName;
	m_Encode = enc;
	std::string strContent = EmFile::ReadUtf8Str(szFileName,enc);

	if(strContent.length() == 0)
	{
		return 1;
	}


	strContent = EmStlStr::Replace(strContent.c_str(),"\r\n","\n");


	vector<std::string> vecStrLine;
	int nLines = EmStlStr::Split(vecStrLine,strContent.c_str(),"\n");

	std::string strLine;
	int nLineLen = 0;
	int nPosEqual = -1;
	int nCountEqual = 0;
	std::string strKey;
	std::string strValue;
	std::string strSection = "";


	t_ini_pair mapDefault;
	m_oModel[strSection] = mapDefault;

	for(int i = 0; i < nLines; i++)
	{
		
		strKey.empty();
		strValue.empty();
		strLine = vecStrLine[i];
		strLine = EmStlStr::Trim(strLine.c_str());

		nLineLen = strLine.length();
		

		if(strLine.length() < 3)
		{
			continue;
		}


		nPosEqual = EmStlStr::Find(strLine.c_str(),"=");
		nCountEqual = EmStlStr::CountStr(strLine.c_str(),"=");

		//comment
		if(EmStlStr::SubStr(strLine.c_str(),0,1).compare(";") == 0
			|| EmStlStr::SubStr(strLine.c_str(),0,1).compare("#") == 0)
		{
		}
		// key=value
		else if(nCountEqual == 1
			&& EmStlStr::SubStr(strLine.c_str(),0,1).compare("=") != 0
			&& EmStlStr::SubStr(strLine.c_str(),nLineLen - 1,1).compare("=") != 0)
		{
			
			strKey = EmStlStr::Trim(EmStlStr::SubStr(strLine.c_str(),0,nPosEqual).c_str());
			strValue = EmStlStr::Trim(EmStlStr::SubStr(strLine.c_str(),nPosEqual + 1,-1).c_str());
			m_oModel[strSection][strKey] = strValue;

		}
		//new section
		else if(EmStlStr::SubStr(strLine.c_str(),0,1).compare("[") == 0
			&& EmStlStr::SubStr(strLine.c_str(),nLineLen - 1,1).compare("]") == 0)
		{
			strSection = EmStlStr::Trim( EmStlStr::SubStr(strLine.c_str(),1,nLineLen - 2).c_str() );
			t_ini_pair mapTemp;
			m_oModel[strSection] = mapTemp;
		}


	}

	return 1;
}

int em::EmIniFile::Save()
{
	return SaveAs(m_strFileName.c_str(),m_Encode);
}

int em::EmIniFile::SaveAs( const char* szFileName, EmEncode enc)
{
	std::string strContent = ToString();
	return EmFile::WriteUtf8(szFileName,strContent.c_str(),enc);
}

int em::EmIniFile::GetSectionNameList(vector<std::string> &rVec)
{
	rVec.clear();
	t_ini_model::iterator itModel;
	for(itModel = m_oModel.begin(); itModel != m_oModel.end(); itModel++ )
	{
		std::string strSection = (std::string)(itModel->first);
		rVec.push_back(strSection);
	}
	return rVec.size();
}

int em::EmIniFile::GetSectionMap(map<std::string,std::string> &rMap, const char* szSection /*=NULL*/ )
{
	rMap.clear();
	if(szSection == NULL)
	{
		szSection = "";
	}
	rMap = m_oModel[szSection];
	return rMap.size();
}

int em::EmIniFile::GetKeyNameList(vector<std::string> &rVec, const char* szSection /*=NULL*/ )
{
	rVec.clear();
	if(szSection == NULL)
	{
		szSection = "";
	}

	t_ini_pair tempPair = m_oModel[szSection];
	if(tempPair.size() == 0)
	{
		return 0;
	}

	t_ini_pair::iterator itPair;
	for(itPair = tempPair.begin(); itPair != tempPair.end(); itPair++ )
	{
		std::string strKey = (std::string)(itPair->first);
		rVec.push_back(strKey);
	}
	return rVec.size();
}

std::string em::EmIniFile::GetValue(  const char* szKey, const char* szSection /*=NULL*/)
{
	if(szSection == NULL)
	{
		szSection = "";
	}
	std::string ret;

	t_ini_pair tempPair = m_oModel[szSection];
	if(tempPair.size() == 0)
	{
		return ret;
	}
	ret = tempPair[szKey];

	return ret;
}

int em::EmIniFile::SetValue(const char* szKey, const char* szValue,  const char* szSection /*=NULL*/)
{
	if(szSection == NULL)
	{
		szSection = "";
	}
	int ret = 1;
	if(m_oModel[szSection].size() == 0)
	{
		t_ini_pair tempPair;
		m_oModel[szSection] = tempPair;
	}

	m_oModel[szSection][szKey] = szValue;
	
	return ret;
}

int em::EmIniFile::AddSection( const char* szSection, map<std::string,std::string> &rMapNewSection)
{
	m_oModel[szSection] = rMapNewSection;
	return 0;
}

int em::EmIniFile::DeleteSection( const char* szSection )
{
	m_oModel.erase(szSection);
	return 0;
}

void em::EmIniFile::Trace()
{
	std::string strTrace = ToString();
}

std::string em::EmIniFile::ToString()
{
	std::string strContent;
	t_ini_model::iterator itModel;
	for(itModel = m_oModel.begin(); itModel != m_oModel.end(); itModel++ )
	{
		std::string strSection = (std::string)(itModel->first);
		t_ini_pair tempPair = (t_ini_pair)(itModel->second);
		if(strSection.length() != 0)
		{
			strContent += "\r\n";
			strContent += "[";
			strContent += strSection;
			strContent += "]";
		}
		
		t_ini_pair::iterator itPair;
		for(itPair = tempPair.begin(); itPair != tempPair.end(); itPair++ )
		{
			std::string strKey = (std::string)(itPair->first);
			std::string strValue = (std::string)(itPair->second);
			strContent += "\r\n";
			strContent += strKey;
			strContent += "=";
			strContent += strValue;
		}
	}

	return strContent;
}

void em::EmIniFile::Clear()
{
	m_oModel.clear();
}
