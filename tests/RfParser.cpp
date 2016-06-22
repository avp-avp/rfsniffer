
#include "stdafx.h"
#include "../libs/libutils/logging.h"
#include "../libs/libutils/Exception.h"
#include "../libs/librf/RFParser.h"
#include "../libs/librf/RFProtocolLivolo.h"
#include "../libs/librf/RFProtocolX10.h"
#include "../libs/librf/RFProtocolRST.h"
#include "../libs/librf/RFProtocolRaex.h"

#define MAX_SIZE 10000

string DecodeFile(const char *File)
{
	CLog *log = CLog::GetLog("Main");
	CRFParser parser(log);
	//parser.EnableAnalyzer();

	//parser.AddProtocol(new CRFProtocolRST());
	//parser.AddProtocol(new CRFProtocolLivolo());
	//parser.AddProtocol(new CRFProtocolX10());
	//parser.AddProtocol(new CRFProtocolRaex());
	parser.AddProtocol("All");

	FILE *f = fopen(File, "rb");
	if (!f)
		return "File not found";
	
	base_type *data = new base_type[MAX_SIZE];
	size_t dataSize = fread(data, sizeof(base_type), MAX_SIZE, f);
	fclose(f);

	string result = parser.Parse(data, dataSize);
	delete []data;
	return result;
}

typedef const char* pstr;
typedef const pstr pstr_pair[2];
typedef pstr_pair *ppstr_pair;


static const pstr_pair Tests[] = {
//	{ "capture-0906-212933.rcf", "" },
	{ "capture-0906-214352.rcf", "Oregon:type=1d20 id=15 ch=1 t=22.6 h=41" },
	{ "capture-0906-214234.rcf", "Oregon:type=1d20 id=15 ch=1 t=22.6 h=41" },
	{ "capture-0906-212618.rcf", "Oregon:type=1d20 id=15 ch=1 t=22.7 h=41" },
	{ "capture-0906-183444.rcf", "Oregon:type=1d20 id=15 ch=1 t=23.3 h=39" },
	{ "capture-0906-165011.rcf", "Oregon:type=1d20 id=15 ch=1 t=24.2 h=40" },
	{ "capture-0906-164649.rcf", "Oregon:type=1d20 id=15 ch=1 t=23.9 h=40" },
	{ "capture-0706-211823.rcf", "Oregon:type=1d20 id=15 ch=1 t=24.7 h=46" },
	{ "capture-0906-210412.rcf", "Oregon:type=1d20 id=15 ch=1 t=23.0 h=40" },
	

	
	//*
	{ "capture-1303-212826.rcf", "X10:D2ON" },
	{ "capture-1303-204025.rcf", "X10:D2ON" },
	{ "capture-2902-213735.rcf", "X10:D2ON" },
	//{ "capture-2902-214441.rcf", "Livolo:00110011110100010001000" }, FAILED INVERTED
	//*
	{ "capture-1304-214720.rcf", "Livolo:00110011110100010001000" },
	{ "capture-1304-214730.rcf", "Livolo:00110011110100010010000" },
	{ "capture-1304-214741.rcf", "Livolo:00110011110100010010000" },
	{ "capture-1304-214753.rcf", "Livolo:00110011110100010111000" },
	{ "capture-1304-214758.rcf", "Livolo:00110011110100010101010" },
	{ "capture-1304-222352.rcf", "Livolo:00000110101001000110000" },
	{ "capture-1304-222343.rcf", "Livolo:00000110101001001010000" },


	{ "capture-0604-212458.rcf", "Livolo:00110011110100010001000" },
	{ "capture-0604-212505.rcf", "Livolo:00110011110100010001000" },  
	{ "capture-0604-212552.rcf", "Livolo:00110011110100010001000" },
	//*/

	{ "capture-1604-081847.rcf", "Raex:111110000011110111111000001000000011111111111111111111111" },
	{ "capture-1604-080728.rcf", "Raex:111110000011110111111000001000000011111111111111111111111" },
	{ "capture-1404-083759.rcf", "Raex:111110000011110111111000001000000011111111111111111111111" },
	{ "capture-1404-083803.rcf", "Raex:111110000011110111111000001000000011111111111111111111111" },
	{ "capture-1404-083811.rcf", "Raex:100001000011110111111000010000000011111111111111111111111" },
//	{ "capture-1404-083816.rcf", "Raex:100001000011110111111000010000000011111111111111111111111" },  // FAIL
//	{ "capture-1404-083823.rcf", "Raex:100001000011110111111000010000000011111111111111111111111" },  // FAIL

	//*/
	//	{ "capture-0804-094607.rcf", "?" },
		{ "capture-0904-091533.rcf", "RST:id=1b10 h=20 t=26.7" },
		{ "capture-0904-091545.rcf", "RST:id=1b10 h=20 t=26.9" },
		{ "capture-1004-105819.rcf", "RST:id=1b10 h=82 t=29.1" },
		{ "capture-1004-121901.rcf", "RST:id=1b10 h=34 t=27.2"},
		{"capture-1004-122939.rcf", ""},  //FAILED
		{"capture-1004-125748.rcf","X10:D2OFF" },


		{"capture-1506-164227.rcf", "X10:D2ON"},
		{"capture-1506-164315.rcf", "??"},
		//*/
	NULL
};

void RfParserTest()
{
	ppstr_pair test = Tests;

	while (**test)
	{
		try
		{
			string type, value;
			SplitPair((*test)[1], ':', type, value);
			if (value.find(' ') != value.npos)
			{
				string_map values;
				SplitValues(value, values);
			}
		}
		catch (CHaException ex)
		{
		}

		string res = DecodeFile((*test)[0]);
		if (res != (*test)[1])
		{
			printf("Failed! File:%s, result:%s, Expected: %s\n", (*test)[0], res.c_str(), (*test)[1]); 
		}

		test++;
	}

	//_finddata_t findData;
	//int handle = _findfirst("*rcf", &findData);
	
	/*
	//FILE *f = fopen("capture-2902-213735.rcf", "r");
	FILE *f = fopen("capture-2902-214441.rcf", "r");
	base_type *data = new base_type[MAX_SIZE];
	size_t dataSize = fread(data, sizeof(base_type), MAX_SIZE, f);
	fclose(f);

	string result = parser.Parse(data, dataSize);

	printf("Parser result:%s\n", result.c_str());*/
}