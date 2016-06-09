#pragma once
#include "stdafx.h"
#include "rflib.h"
#include "RFProtocol.h"

typedef vector<CRFProtocol*> CRFProtocolList;
class CRFAnalyzer;

class RFLIB_API CRFParser
{
	CLog *m_Log;
	CRFProtocolList m_Protocols;
	bool b_RunAnalyzer;
	CRFAnalyzer *m_Analyzer;
	string m_SavePath;
public:
	CRFParser(CLog *log, string SavePath="");
	virtual ~CRFParser();

	void AddProtocol(CRFProtocol*);
	void AddProtocol(string protocol="all");
	string Parse(base_type*, size_t len);
	void EnableAnalyzer();
	void SaveFile(base_type* data, size_t size);
	void SetSavePath(string SavePath);
};
