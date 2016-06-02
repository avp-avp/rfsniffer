#pragma once
#include "stdafx.h"
#include "rflib.h"

typedef unsigned long base_type;

#define PULSE_BIT       0x01000000
#define PULSE_MASK      0x00FFFFFF

typedef  base_type range_type[2];
typedef  range_type *range_array_type;
string c2s(char c);

class RFLIB_API CRFProtocol
{
protected:
	range_array_type m_ZeroLengths, m_PulseLengths;
	int m_MinRepeat, m_Bits;
	char m_PacketDelimeter;
	bool m_Debug;
public:
	CRFProtocol(range_array_type zeroLengths, range_array_type pulseLengths, int bits, int minRepeat, char PacketDelimeter );
	virtual ~CRFProtocol();

	virtual string Parse(base_type*, size_t len);

	virtual string DecodeRaw(base_type* data, size_t dataLen);  // Декодироавние строки по длинам
	virtual bool SplitPackets(const string &rawData, string_vector& rawPackets); // Нарезка по пакетам
	virtual string DecodeBits(string_vector&rawPackets); // Сборка бит по массиву пакетов
	virtual string DecodePacket(const string&); // Преобразование строки длин в биты
	virtual string DecodeData(const string&); // Преобразование бит в данные

	virtual string tryDecode(string data);

	inline bool isPulse(base_type v) { return (v&PULSE_BIT) != 0; };
	inline base_type getLengh(base_type v) { return v&PULSE_MASK; };
	virtual string getName() = 0;

	unsigned long bits2long(const string&);
	string reverse(const string&);
};

