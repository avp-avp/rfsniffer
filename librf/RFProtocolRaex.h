#pragma once
#include "RFProtocol.h"
class RFLIB_API CRFProtocolRaex :
	public CRFProtocol
{
public:
	CRFProtocolRaex();
	virtual ~CRFProtocolRaex();

	virtual string getName() { return "Raex"; };
	virtual string DecodePacket(const string&);


	string ManchesterDecode(const string&, bool expectPulse, char shortPause, char longPause, char shortPulse, char longPulse);
};

