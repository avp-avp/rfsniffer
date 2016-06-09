#include "stdafx.h"
#include "RFProtocolLivolo.h"
#include "../libutils/strutils.h"

/*V0
static range_type g_timing[4] =
{
	{450,550},
	{60,205},
	{250, 380},
	{0,0}
};

static range_type g_timing[4] =
{
{ 400,550 },
{ 60,230 },
{ 250, 380 },
{ 0,0 }
};


static range_type g_timing_pause[7] =
{
{ 300,650 },
{ 100,270 },
{ 10, 99 },
{1,9},
{271,299},
{651, 1000},
{ 0,0 }
};

static range_type g_timing_pulse[8] =
{
{ 450,600 },
{ 30,180	 },
{ 200, 330 },
{1,29},
{181, 199},
{331, 449},
{600, 1000},
{ 0,0 }
};



static range_type g_timing_pause[7] =
{
	{ 420,570 },
	{ 60, 230 },
	{ 260, 370 },
	{ 0,0 }
};

static range_type g_timing_pulse[8] =
{
	{ 1000, 1001 },
	{ 80, 220 },
	{ 260, 380	 },
	{ 0,0 }
};

*/

static range_type g_timing_pause[7] =
{
	{ 420,570 },
	{ 140, 280 },
	{ 290, 440 },
	{ 0,0 }
};

static range_type g_timing_pulse[8] =
{
	{ 380, 500 },
	{ 20, 170 },
	{ 190, 300 },
	{ 0,0 }
};


CRFProtocolLivolo::CRFProtocolLivolo()
	:CRFProtocol(g_timing_pause, g_timing_pulse, 23, 2, "A")
{
}


CRFProtocolLivolo::~CRFProtocolLivolo()
{
}
/*

string CRFProtocolLivolo::Parse(base_type*data, size_t len)
{
	string_vector v;
	string raw = CRFProtocol::Parse(data, len);
	SplitString(raw, 'a', v);

	string res; int count = 0;

	for_each (string_vector, v, s)
	{
		if (s->find('?') != string::npos)
			continue;

		if (res.length())
		{
			if (res == *s)
			{
				if (++count >= m_MinRepeat)
					break;
			}
			else
			{
				res = *s;
				count=1;
				if (m_MinRepeat == 1)
					break;
			}
		}
		else
		{
			res = *s;
			count=1;
		}
	}

	if (res.length() && count >= m_MinRepeat)
	{
		string binRes; bool waitSecondShort=false;
		for_each(string, res, s)
		{
			if (waitSecondShort)
			{
				waitSecondShort = false;
				if (*s == 'b' || *s == 'B')
					continue;
				else
					return "";

			}

			if (*s == 'b' || *s == 'B')
			{
				binRes += "0";
				waitSecondShort = true;
			}
			else if (*s == 'c' || *s == 'C')
			{
				binRes += "1";
			}
		}
		return getName()+":"+ binRes;
	}

	return "";
}
*/
string CRFProtocolLivolo::DecodePacket(const string&packet)
{
	string bits; bool waitSecondShort = false;

	for_each_const(string, packet, s)
	{
		if (waitSecondShort)
		{
			waitSecondShort = false;
			if (*s == 'b' || *s == 'B')
				continue;
			else
				return "";

		}

		if (*s == 'b' || *s == 'B')
		{
			bits += "0";
			waitSecondShort = true;
		}
		else if (*s == 'c' || *s == 'C')
		{
			bits += "1";
		}
	}

	return bits;
}