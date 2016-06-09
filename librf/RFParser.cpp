#include "stdafx.h"
#include <fcntl.h>

#include "RFParser.h"
#include "RFProtocolLivolo.h"
#include "RFProtocolX10.h"
#include "RFProtocolRST.h"
#include "RFProtocolRaex.h"
#include "RFProtocolOregon.h"
#include "RFAnalyzer.h"

CRFParser::CRFParser(CLog *log, string SavePath)
	:b_RunAnalyzer(false), m_Analyzer(NULL), m_Log(log), m_SavePath(SavePath)
{
}


CRFParser::~CRFParser()
{
	for_each(CRFProtocolList, m_Protocols, i)
	{
		delete *i;
	}

	delete m_Analyzer;
}

void CRFParser::AddProtocol(string protocol)
{
	if (protocol == "X10")
		AddProtocol(new CRFProtocolX10());
	else if (protocol == "RST")
		AddProtocol(new CRFProtocolRST());
	else if (protocol == "Raex")
		AddProtocol(new CRFProtocolRaex());
	else if (protocol == "Livolo")
		AddProtocol(new CRFProtocolLivolo());
	else if (protocol == "Oregon")
		AddProtocol(new CRFProtocolOregon());
	else if (protocol == "All")
	{
		AddProtocol(new CRFProtocolX10());
		AddProtocol(new CRFProtocolRST());
		AddProtocol(new CRFProtocolRaex());
		AddProtocol(new CRFProtocolLivolo());
		AddProtocol(new CRFProtocolOregon());
	}
	else
		throw CHaException(CHaException::ErrBadParam, protocol);
}

void CRFParser::AddProtocol(CRFProtocol* p)
{
	p->setLog(m_Log);
	m_Protocols.push_back(p);
}

string CRFParser::Parse(base_type* data, size_t len)
{
	for_each(CRFProtocolList, m_Protocols, i)
	{
		string retval = (*i)->Parse(data, len);
		if (retval.length())
			return retval;
	}

	if (b_RunAnalyzer)
	{
		if (!m_Analyzer)
			m_Analyzer = new CRFAnalyzer(m_Log);

		m_Analyzer->Analyze(data, len);
	}

	for_each(CRFProtocolList, m_Protocols, i)
	{
		if ((*i)->needDumpPacket())
		{
			m_Log->Printf(3, "Dump packet for %s", (*i)->getName().c_str());
			SaveFile(data, len);
			return "";
		}
	}
	

	return "";
}

void CRFParser::EnableAnalyzer()
{ 
	b_RunAnalyzer = true;
}

void CRFParser::SaveFile(base_type* data, size_t size)
{
#ifndef WIN32
	if (m_SavePath.length())
	{
		time_t Time = time(NULL);
		char DateStr[100], FileName[1024];
		strftime(DateStr, sizeof(DateStr), "%d%m-%H%M%S", localtime(&Time));
		snprintf(FileName, sizeof(FileName),  "%s/capture-%s.rcf", m_SavePath.c_str(), DateStr);
		m_Log->Printf(3, "Write to file %s %ld signals\n", FileName, size);
		int of = open(FileName, O_WRONLY | O_CREAT, S_IRUSR|S_IRGRP);
		
		if (of == -1) {
			m_Log->Printf(3, "error opening %s\n", FileName);
			return;
		};

		write(of, data, sizeof(data[0])*size);
		close(of);
	}
#endif
}

void CRFParser::SetSavePath(string SavePath)
{
	m_SavePath = SavePath;
}
