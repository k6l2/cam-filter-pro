#include "Z85.h"
#include <cassert>
#include <iostream>
const char* const Z85::LOOKUP_TABLE = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.-:+=^!/*?&<>()[]{}@%$#";
const std::map<char, unsigned> Z85::ASCII_TO_85_DIGIT = {
	{'0',0 },
	{'1',1 },
	{'2',2 },
	{'3',3 },
	{'4',4 },
	{'5',5 },
	{'6',6 },
	{'7',7 },
	{'8',8 },
	{'9',9 },
	{'a',10},
	{'b',11},
	{'c',12},
	{'d',13},
	{'e',14},
	{'f',15},
	{'g',16},
	{'h',17},
	{'i',18},
	{'j',19},
	{'k',20},
	{'l',21},
	{'m',22},
	{'n',23},
	{'o',24},
	{'p',25},
	{'q',26},
	{'r',27},
	{'s',28},
	{'t',29},
	{'u',30},
	{'v',31},
	{'w',32},
	{'x',33},
	{'y',34},
	{'z',35},
	{'A',36},
	{'B',37},
	{'C',38},
	{'D',39},
	{'E',40},
	{'F',41},
	{'G',42},
	{'H',43},
	{'I',44},
	{'J',45},
	{'K',46},
	{'L',47},
	{'M',48},
	{'N',49},
	{'O',50},
	{'P',51},
	{'Q',52},
	{'R',53},
	{'S',54},
	{'T',55},
	{'U',56},
	{'V',57},
	{'W',58},
	{'X',59},
	{'Y',60},
	{'Z',61},
	{'.',62},
	{'-',63},
	{':',64},
	{'+',65},
	{'=',66},
	{'^',67},
	{'!',68},
	{'/',69},
	{'*',70},
	{'?',71},
	{'&',72},
	{'<',73},
	{'>',74},
	{'(',75},
	{')',76},
	{'[',77},
	{']',78},
	{'{',79},
	{'}',80},
	{'@',81},
	{'%',82},
	{'$',83},
	{'#',84}
};
unsigned long long Z85::encodedFileSize(unsigned long long rawFileByteCount)
{
	unsigned long long binaryFrameCount = rawFileByteCount / 4;
	if (rawFileByteCount % 4)
	{
		// add an extra frame if we can't perfectly divide the raw file
		//	into groups of 4 bytes per frame
		++binaryFrameCount;
	}
	// 5 ascii characters per frame!
	return binaryFrameCount * 5;
}
unsigned long long Z85::decodedFileSize(unsigned long long asciiByteCount)
{
	/// ASSUMPTION: all encoded files % 5 == 0
	return (asciiByteCount / 5) * 4;
}
void Z85::encode(char * input, unsigned long long inputByteSize, char * output)
{
	for (unsigned long long b = 0; b < inputByteSize; b += 4)
	{
		// build the next unsigned 32-bit # from the input
		unsigned __int32 intInput = 0;
		for (size_t c = 0; c < 4; c++)
		{
			intInput <<= 8;
			intInput |= (unsigned char)input[b + c];
		}
		// find the 5 base-85 remainders, and write them to the output
		for (int c = 4; c >= 0; c--)
		{
			const int remainder = intInput % 85;
			intInput /= 85;
			assert(remainder < 85);
			output[c] = LOOKUP_TABLE[remainder];
		}
		// increment the output pointers,
		//	we assume that output array is properly sized //
		output += 5;
	}
}
void Z85::decode(char * input, char * output)
{
	while (*input)
	{
		// first, we need to translate from ascii char => base-85 # //
		// then, build the 32-bit ouput # from the base-85 digits //
		unsigned long intOutput = 0;
		for (size_t c = 0; c < 5; c++)
		{
			auto digitIt = ASCII_TO_85_DIGIT.find(input[c]);
			if (digitIt == ASCII_TO_85_DIGIT.end())
			{
				std::cerr << "ERROR: Z85 could not decode character '" << input[c] << "'\n";
				assert(false);
				return;
			}
			const unsigned base85Digit = digitIt->second;
			const size_t base85Place = 4 - c;
			intOutput += base85Digit*(unsigned __int32)(pow(85,base85Place));
		}
		// output each digit byte to output array //
		for (int c = 3; c >= 0; c--)
		{
			output[c] = intOutput & 0xFF;
			intOutput >>= 8;
		}
		// increment input & output //
		// 5 bytes of input == 4 bytes of output data
		input += 5;
		output += 4;
	}
}
