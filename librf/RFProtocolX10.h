#pragma once
#include "RFProtocol.h"
class RFLIB_API CRFProtocolX10 :
	public CRFProtocol
{
public:
	CRFProtocolX10();
	virtual ~CRFProtocolX10();

	virtual string getName() { return "X10"; };
	virtual string DecodePacket(const string&);
	virtual string DecodeData(const string&); // �������������� ��� � ������

	char parseHouseCode(uint8_t data);


};

