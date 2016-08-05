#include "stdafx.h"
#include "../inc/EmUrl.h"

#include <string.h>
#include <ctype.h>



using namespace em;

static int HexPairValue(const char * code) {
	int value = 0;
	const char * pch = code;
	for (;;) {
		int digit = *pch++;
		if (digit >= '0' && digit <= '9') {
			value += digit - '0';
		}
		else if (digit >= 'A' && digit <= 'F') {
			value += digit - 'A' + 10;
		}
		else if (digit >= 'a' && digit <= 'f') {
			value += digit - 'a' + 10;
		}
		else {
			return -1;
		}
		if (pch == code + 2)
			return value;
		value <<= 4;
	}
}

static int UrlDecode(const char *source, char *dest)
{
	char * start = dest;
	
	while (*source) {
		switch (*source) {
		case '+':
			*(dest++) = ' ';
			break;
		case '%':
			if (source[1] && source[2]) {
				int value = HexPairValue(source + 1);
				if (value >= 0) {
					*(dest++) = value;
					source += 2;
				}
				else {
					*dest++ = '?';
				}
			}
			else {
				*dest++ = '?';
			}
			break;
		default:
			*dest++ = *source;
		}
		source++;
	}
	
	*dest = 0;
	return dest - start;
}  

static int UrlEncode(const char *source, char *dest, unsigned int max, bool bEncodeAll)  
{
	static const char *digits = "0123456789ABCDEF";
	unsigned char ch;
	unsigned len = 0;
	char *start = dest;
	
	while (len < max - 4 && *source)
	{
		ch = (unsigned char)*source;
		if (*source == ' ') {
			*dest++ = '+';
		}
		else if (isalnum(ch) || (strchr("!@#$&*()=:/;?+'.", ch) && !bEncodeAll)) {
			*dest++ = *source;
		}
		else {
			*dest++ = '%';
			*dest++ = digits[(ch >> 4) & 0x0F];
			*dest++ = digits[       ch & 0x0F];
		}  
		source++;
	}
	*dest = 0;
	return start - dest;
}


char * EmUrl::Encode(char *szDest, const char *szSrc, bool bEncodeAll)
{
	UrlEncode(szSrc,szDest,strlen(szSrc) * 4,bEncodeAll);
	return szDest;
}


char * EmUrl::Decode(char *szDest, const char *szSrc, bool bEncodeAll)
{
	UrlDecode(szSrc,szDest);
	return szDest;
}