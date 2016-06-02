#include "stdafx.h"
#include "RFProtocol.h"


CRFProtocol::CRFProtocol(range_array_type zeroLengths, range_array_type pulseLengths, int bits, int minRepeat, char PacketDelimeter)
:m_ZeroLengths(zeroLengths), m_PulseLengths(pulseLengths), m_Bits(bits), m_MinRepeat(minRepeat), m_PacketDelimeter(PacketDelimeter)
{
	m_Debug = false;
}


CRFProtocol::~CRFProtocol()
{
}

string c2s(char c)
{
	char tmp[2];
	tmp[0] = c;
	tmp[1] = 0;
	return tmp;
}

string CRFProtocol::Parse(base_type* data, size_t dataLen)
{
	string decodedRaw = DecodeRaw(data, dataLen);

	if (!decodedRaw.length())
		return "";

	string_vector rawPackets;
	if (!SplitPackets(decodedRaw, rawPackets))
		return "";

	string bits = DecodeBits(rawPackets);

	if(bits.length())
		return getName()+":"+ DecodeData(bits);

	return "";
}

string CRFProtocol::tryDecode(string data)
{
	if (data.length() >= m_Bits)
		return getName() + ":" + data;
	else
		return "";
}


string CRFProtocol::DecodeRaw(base_type* data, size_t dataLen)
{
	string decodedRaw, decodedRawRev;

	for (size_t i = 0; i < dataLen; i++)
	{
		base_type len = getLengh(data[i]);

		if (isPulse(data[i]))
		{
			int pos = 0;
			for (; m_PulseLengths[pos][0]; pos++)
			{
				if (len >= m_PulseLengths[pos][0] && len <= m_PulseLengths[pos][1])
				{
					decodedRaw += c2s('A' + pos);
					break;
				}
			}

			if (!m_PulseLengths[pos][0])
			{
				if (m_Debug)
					decodedRaw += string("[") + itoa(len) + "}";
				else
					decodedRaw += "?";
			}

			pos = 0;
			for (; m_ZeroLengths[pos][0]; pos++)
			{
				if (len >= m_ZeroLengths[pos][0] && len <= m_ZeroLengths[pos][1])
				{
					decodedRawRev += c2s('a' + pos);
					break;
				}
			}

			if (!m_ZeroLengths[pos][0])
				decodedRawRev += "?";
		}
		else
		{
			int pos = 0;
			for (; m_ZeroLengths[pos][0]; pos++)
			{
				if (len >= m_ZeroLengths[pos][0] && len <= m_ZeroLengths[pos][1])
				{
					decodedRaw += c2s('a' + pos);
					break;
				}
			}

			if (!m_ZeroLengths[pos][0])
			{
				if (m_Debug)
					decodedRaw += string("[") + itoa(len) + "]";
				else
					decodedRaw += "?";
			}

			pos = 0;
			for (; m_PulseLengths[pos][0]; pos++)
			{
				if (len >= m_PulseLengths[pos][0] && len <= m_PulseLengths[pos][1])
				{
					decodedRawRev += c2s('A' + pos);
					break;
				}
			}

			if (!m_PulseLengths[pos][0])
				decodedRawRev += "?";
		}
	}

	return decodedRaw+"?"+decodedRawRev;
}

bool CRFProtocol::SplitPackets(const string &rawData, string_vector& rawPackets)
{
	SplitString(rawData, m_PacketDelimeter, rawPackets);

	return rawPackets.size() > 1;
}

string CRFProtocol::DecodeBits(string_vector&rawPackets)
{
	string res; int count = 0;

	for_each(string_vector, rawPackets, s)
	{
		string packet;
		size_t pos = s->find(m_Debug ? '[' : '?');
		if (pos != string::npos)
			packet = s->substr(0, pos);
		else
			packet = *s;

		if (!packet.length())
			continue;

		string decoded = DecodePacket(packet);

		if (decoded.length() != m_Bits)
			continue;

		if (res.length())
		{
			if (res == decoded)
			{
				if (++count >= m_MinRepeat)
					break;
			}
			else
			{
				res = decoded;
				count = 1;
				if (m_MinRepeat == 1)
					break;
			}
		}
		else
		{
			res = decoded;
			count = 1;
		}
	}

	if (res.length() && count >= m_MinRepeat)
		return res;
	else
		return "";
}

string CRFProtocol::DecodePacket(const string &raw)
{
	return raw;
}

string CRFProtocol::DecodeData(const string &raw)
{
	return raw;
}

unsigned long CRFProtocol::bits2long(const string &raw)
{
	unsigned long res=0;
	
	for_each_const(string, raw, i)
	{
		res = res << 1;

		if (*i=='1')
			res++;
	}

	return res;
}

string CRFProtocol::reverse(const string&s)
{
	string res;

	for_each_const(string, s, i)
	{
		res = *i + res;
	}

	return res;
}