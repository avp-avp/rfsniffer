#pragma once
#include "stdafx.h"
#include "rflib.h"
#include "RFProtocol.h"

typedef vector<CRFProtocol*> CRFProtocolList;

class RFLIB_API CRFParser
{
	CRFProtocolList m_Protocols;
public:
	CRFParser();
	virtual ~CRFParser();

	void AddProtocol(CRFProtocol*);
	string Parse(base_type*, size_t len);
};
