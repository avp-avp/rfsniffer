#include "stdafx.h"
#include "RFParser.h"


CRFParser::CRFParser()
{
}


CRFParser::~CRFParser()
{
	for_each(CRFProtocolList, m_Protocols, i)
	{
		delete *i;
	}
}

void CRFParser::AddProtocol(CRFProtocol* p)
{
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

	return "";
}
