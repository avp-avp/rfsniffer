#include "stdafx.h"

#include "../libs/libutils/strutils.h"
//#include "../lua/liblua.h"

void LogTest();
void RfParserTest();
void Rfm69Test();
void SnifferTest();
void MqttTest();

int main()
{
	srand(time(NULL));
	//LogTest();
	//RfParserTest();
	Rfm69Test();
	//SnifferTest();
	//MqttTest();
}
