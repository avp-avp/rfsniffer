#include "stdafx.h"
#include "RFProtocolRaex.h"

static range_type g_timing_pause[7] =
{
	{ 2500, 2700 },
	{ 550, 750 },
	{ 1200, 1400 },
	{ 0,0 }
};

static range_type g_timing_pulse[8] =
{
	{ 2400, 2600 },
	{ 450, 650 },
	{ 110, 1300 },
	{ 0,0 }
};


CRFProtocolRaex::CRFProtocolRaex()
	:CRFProtocol(g_timing_pause, g_timing_pulse, 57, 2, "A")
{
}


CRFProtocolRaex::~CRFProtocolRaex()
{
}


string CRFProtocolRaex::DecodePacket(const string&raw)
{
	if (raw.length() < 10 || raw[0] != 'a')
		return "";

	return ManchesterDecode(raw.substr(1), true, 'b', 'c', 'B', 'C');
}


