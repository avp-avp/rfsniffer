
#include "stdafx.h"
#include <sys/types.h>
#include <dirent.h>
#include "../libs/libutils/logging.h"
#include "../libs/libutils/Exception.h"
#include "../libs/librf/RFParser.h"
#include "../libs/librf/RFProtocolLivolo.h"
#include "../libs/librf/RFProtocolX10.h"
#include "../libs/librf/RFProtocolRST.h"
#include "../libs/librf/RFProtocolRaex.h"
#include "../libs/librf/RFProtocolNooLite.h"

#define MAX_SIZE 10000

string DecodeFile(CRFParser* parser, CLog *log, const char *File)
{
	FILE *f = fopen(File, "rb");
	if (!f)
		return "File not found";
	
	base_type *data = new base_type[MAX_SIZE];
	size_t dataSize = fread(data, sizeof(base_type), MAX_SIZE, f);
	fclose(f);

	string result = parser->Parse(data, dataSize);
	delete []data;
	return result;
}

typedef const char* pstr;
typedef const pstr pstr_pair[2];
typedef pstr_pair *ppstr_pair;


static const pstr_pair Tests[] = {
	{ "capture-1311-190209.rcf", "Rubitek:0110111010111110000010110" },
	{ "capture-1311-190019.rcf", "Rubitek:0110111010111110000011100" },
	{ "capture-1311-190013.rcf", "Rubitek:0110111010111110000001110" },
	{ "capture-1311-190018.rcf", "Rubitek:0110111010111110000011100" },
	{ "capture-1311-190202.rcf", "" },
	{ "capture-1311-190206.rcf", "" },
	{ "capture-1311-190014.rcf", "Rubitek:0110111010111110000001110" },
	{ "capture-1311-190019.rcf", "Rubitek:0110111010111110000011100" },
	{ "capture-1311-190204.rcf", "Rubitek:0110111010111110000010110" },
	{ "capture-1311-190209.rcf", "Rubitek:0110111010111110000010110" },

	{ "capture-0307-215444.rcf", "nooLite:flip=1 cmd=4 addr=9a13 fmt=00 crc=76" },
	{ "capture-0307-215449.rcf", "nooLite:flip=0 cmd=4 addr=9a13 fmt=00 crc=6a" },
	{ "capture-2706-190620.rcf", "nooLite:flip=0 cmd=21 type=2 t=29.6 h=39 s3=ff bat=0 addr=1492 fmt=07 crc=ec"},
	{ "capture-2706-143835.rcf", "nooLite:flip=1 cmd=21 type=2 t=30.3 h=43 s3=ff bat=0 addr=1492 fmt=07 crc=d3"},
	{ "capture-2706-093124.rcf", "nooLite:flip=1 cmd=5 addr=9a12 fmt=00 crc=e5" },
	{ "capture-2706-093129.rcf", "nooLite:flip=0 cmd=10 addr=9a12 fmt=00 crc=88" },
	{ "capture-2706-093217.rcf", "nooLite:flip=0 cmd=15 addr=9a11 fmt=00 crc=b4" },
	{ "capture-2706-093128.rcf", "nooLite:flip=1 cmd=5 addr=9a12 fmt=00 crc=e5" },
	{ "capture-2706-093221.rcf", "nooLite:flip=1 cmd=15 addr=9a11 fmt=00 crc=a8" },
	{ "capture-2706-093205.rcf", "nooLite:flip=0 cmd=15 addr=9a13 fmt=00 crc=fb" },
	//{ "","" },
	{ "capture-2506-120004.rcf", "nooLite:flip=0 cmd=5 addr=9a13 fmt=00 crc=52"},
	{ "capture-2506-115944.rcf", "nooLite:flip=1 cmd=5 addr=9a13 fmt=00 crc=4e" },
	
	//	{ "capture-0906-212933.rcf", "" },* /
	{ "capture-0906-214352.rcf", "Oregon:type=1D20 id=15 ch=1 t=22.6 h=41" },
	{ "capture-0906-214234.rcf", "Oregon:type=1D20 id=15 ch=1 t=22.6 h=41" },
	{ "capture-0906-212618.rcf", "Oregon:type=1D20 id=15 ch=1 t=22.7 h=41" },
	{ "capture-0906-183444.rcf", "Oregon:type=1D20 id=15 ch=1 t=23.3 h=39" },
	{ "capture-0906-165011.rcf", "Oregon:type=1D20 id=15 ch=1 t=24.2 h=40" },
	{ "capture-0906-164649.rcf", "Oregon:type=1D20 id=15 ch=1 t=23.9 h=40" },
	{ "capture-0706-211823.rcf", "Oregon:type=1D20 id=15 ch=1 t=24.7 h=46" },
	{ "capture-0906-210412.rcf", "Oregon:type=1D20 id=15 ch=1 t=23.0 h=40" },
	

	
	{ "capture-2708-132748.rcf", "X10:D2ON" },
	{ "capture-2708-132756.rcf", "X10:D2ON" },
	{ "capture-2708-132933.rcf", "X10:D2ON" },

	//*
	{ "capture-1303-212826.rcf", "" }, // Inverted X10:D2ON" },
	{ "capture-1303-204025.rcf", "" }, // Inverted X10:D2ON" },
	{ "capture-2902-213735.rcf", "" }, // Inverted X10:D2ON" },
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
	//* /

	{ "capture-1604-081847.rcf", "Raex:raw=F07BF0407FFFFF ch=F0 btn=1" },
	{ "capture-1604-080728.rcf", "Raex:raw=F07BF0407FFFFF ch=F0 btn=1" },
	{ "capture-1404-083759.rcf", "Raex:raw=F07BF0407FFFFF ch=F0 btn=1" },
	{ "capture-1404-083803.rcf", "Raex:raw=F07BF0407FFFFF ch=F0 btn=1" },
	{ "capture-1404-083811.rcf", "Raex:raw=087BF0807FFFFF ch=08 btn=2" },

//File: capture-1408-204138.rcf, decoded: RST:id=1b00 h=46 t=14.1
		{ "capture-1408-204138.rcf", "RST:id=1b00 h=46 t=14.1" },

//File: capture-1004-105819.rcf, decoded: RST:id=1b10 h=82 t=29.1
		{ "capture-1004-105819.rcf", "RST:id=1b10 h=82 t=29.1" },

	// Inverted
	//	{ "capture-0804-094607.rcf", "?" },
		{ "capture-0904-091533.rcf", ""}, //RST:id=1b10 h=20 t=26.7" },
		{ "capture-0904-091545.rcf", ""}, //RST:id=1b10 h=20 t=26.9" },
		{ "capture-1004-121901.rcf", ""}, //RST:id=1b10 h=34 t=27.2"},
	
		{"capture-1004-122939.rcf", ""},  //FAILED
		{"capture-1004-125748.rcf","" }, // Inverted X10:D2ON" },X10:D2OFF" },

		//*/
	NULL
};

void getAllTestFiles( string path, string_vector &result )
{
   DIR* dirFile = opendir( path.c_str() );
   if ( dirFile ) 
   {
      struct dirent* hFile;
      errno = 0;
      while (( hFile = readdir( dirFile )) != NULL ) 
      {
         if ( !strcmp( hFile->d_name, "."  )) continue;
         if ( !strcmp( hFile->d_name, ".." )) continue;

         // in linux hidden files all start with '.'
         bool gIgnoreHidden = true;
         if ( gIgnoreHidden && ( hFile->d_name[0] == '.' )) continue;

         // dirFile.name is the name of the file. Do whatever string comparison 
         // you want here. Something like:
         if ( strstr( hFile->d_name, ".rcf" ))
         	result.push_back(hFile->d_name);
      } 
      closedir( dirFile );
   }
}

void RfParserTest(string path)
{
	bool allPassed = true;
	CLog *log = CLog::GetLog("Main");
	CRFParser parser(log);
	//parser.EnableAnalyzer();

	//parser.AddProtocol(new CRFProtocolRST());
	//parser.AddProtocol(new CRFProtocolLivolo());
	//parser.AddProtocol(new CRFProtocolX10());
	//parser.AddProtocol(new CRFProtocolRaex());
	parser.AddProtocol("All");

	ppstr_pair test = Tests;

	if (path.length())
	{
		string_vector files;
		getAllTestFiles(".", files);
		for_each(string_vector, files, f)
		{
			string res = DecodeFile(&parser, log, f->c_str());
			printf("File: %s, decoded: %s\n", f->c_str(), res.c_str());
		}
		return;
	}

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

		string res = DecodeFile(&parser, log, (string("tests/testfiles/")+(*test)[0]).c_str());
		if (res != (*test)[1])
		{
			printf("Failed! File:%s, result:%s, Expected: %s\n", (*test)[0], res.c_str(), (*test)[1]); 
			allPassed = false;
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

	if (!allPassed)
		    exit(1);
}