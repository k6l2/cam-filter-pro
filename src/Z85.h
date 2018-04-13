#pragma once
#include <map>
/*
	Z85 converts binary data to ascii data,
		in a format which can be saved as a 
		const char* const variable in source code.

	Formal specification of Z85 encode/decode:
		https://rfc.zeromq.org/spec:32/Z85/
*/
class Z85
{
private:
	static const std::map<char, unsigned> ASCII_TO_85_DIGIT;
	static const char* const LOOKUP_TABLE;
public:
	static unsigned long long encodedFileSize(unsigned long long rawFileByteCount);
	static unsigned long long decodedFileSize(unsigned long long asciiByteCount);
	static void encode(char* input, unsigned long long inputByteSize, char* output);
	static void decode(char* input, char* output);
};
