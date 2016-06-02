#pragma once
#include "RFProtocol.h"
class RFLIB_API CRFProtocolLivolo :
	public CRFProtocol
{
public:
	CRFProtocolLivolo();
	virtual ~CRFProtocolLivolo();

	//virtual string Parse(base_type*, size_t len);
	virtual string getName() { return "Livolo"; };
	virtual string DecodePacket(const string&);

};

