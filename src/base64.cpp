#include "carinfo-manager/base64.hpp"
#include <cstring>

const char Base64::encodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char Base64::decodeTable[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	62, // '+'
	0, 0, 0,
	63, // '/'
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
	0, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
	0, 0, 0, 0, 0, 0,
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
};

std::string Base64::encode(const unsigned char* data, size_t data_bytes_length) {
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i<(int)(data_bytes_length / 3); i++) {
		Tmp[1] = *data++;
		Tmp[2] = *data++;
		Tmp[3] = *data++;
		strEncode += encodeTable[Tmp[1] >> 2];
		strEncode += encodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += encodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += encodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) 
            strEncode += "\r\n"; LineLength = 0;
	}

	int Mod = data_bytes_length % 3;
	if (Mod == 1) {
		Tmp[1] = *data++;
		strEncode += encodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += encodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2) {
		Tmp[1] = *data++;
		Tmp[2] = *data++;
		strEncode += encodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += encodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += encodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}

	return strEncode;
}

std::string Base64::decode(const char* data, int data_bytes_length, size_t& output_bytes_length) {
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < data_bytes_length) {
		if (*data != '\r' && *data != '\n') {
			nValue = decodeTable[*data++] << 18;
			nValue += decodeTable[*data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			output_bytes_length++;
			if (*data != '=') {
				nValue += decodeTable[*data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				output_bytes_length++;
				if (*data != '=') {
					nValue += decodeTable[*data++];
					strDecode += nValue & 0x000000FF;
					output_bytes_length++;
				}
			}
			i += 4;
		}
		else {
			data++;
			i++;
		}
	}
	return strDecode;
}