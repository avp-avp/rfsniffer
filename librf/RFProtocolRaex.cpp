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
	:CRFProtocol(g_timing_pause, g_timing_pulse, 57, 2, 'A')
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


string CRFProtocolRaex::ManchesterDecode(const string&raw, bool expectPulse, char shortPause, char longPause, char shortPulse, char longPulse)
{
	enum t_state { expectStartPulse, expectStartPause, expectMiddlePulse, expectMiddlePause};

	t_state state = expectPulse? expectStartPulse: expectStartPause;
	string res;

	for_each_const(string, raw, c)
	{
		switch (state)
		{
		case expectStartPulse:   // ќжидаем короткий пульс, всегда 1
			if (*c == shortPulse)
			{
				res += "1";
				state = expectMiddlePause;
			}
			else
			{ 
				return "";
			}
			break;
		case expectStartPause:  // ќжидаем короткую паузу, всегда 0
			if (*c == shortPause)
			{
				res += "0";
				state = expectMiddlePulse;
			}
			else
			{
				return "";
			}
			break;
		case expectMiddlePulse:  // ќжидаем пульс. ≈сли короткий - пара закончилась, ждем короткую стартовую паузу. ≈сли длинный, получили начало след пары и ждем среднюю паузу
			if (*c == shortPulse)
			{
				state = expectStartPause;
			}
			else if (*c == longPulse)
			{
				state = expectMiddlePause;
				res += "1";
			}
			else
			{
				return "";
			}
			break;
		case expectMiddlePause:  // ќжидаем паузу. ≈сли коротка€ - пара закончилась, ждем короткую стартовый пульс. ≈сли длинна€, получили начало след пары и ждем средний пульс
			if (*c == shortPause)
			{
				state = expectStartPulse;
			}
			else if (*c == longPause)
			{
				state = expectMiddlePulse;
				res += "0";
			}
			else
			{
				return "";
			}
			break;
		default:
			return "";
		}
	}

	return res;
}
