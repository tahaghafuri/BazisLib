#include "stdafx.h"
#include "base64.h"

using namespace BazisLib;
using namespace BazisLib::Network;

template <class _CharType> static inline bool DecodeBase64Char(_CharType tch, unsigned char &value, bool &padding)
{
	padding = false;
	if ((tch > 0x7F) || (tch <= 0))
		return false;

	unsigned char ch = (unsigned char)tch;
	if ((ch >= 'A') && (ch <= 'Z'))
	{
		value = ch - 'A';
		return true;
	}
	if ((ch >= 'a') && (ch <= 'z'))
	{
		value = ch - 'a' + 26;
		return true;
	}
	if ((ch >= '0') && (ch <= '9'))
	{
		value = ch - '0' + 52;
		return true;
	}
	if (ch == '+')
	{
		value = 62;
		return true;
	}
	if (ch == '/')
	{
		value = 63;
		return true;
	}
	if (ch == '=')
	{
		value = 0;
		padding = true;
		return true;
	}
	return false;
}

template <class _CharType> static inline void Base64Encode(const void *pPtr, unsigned Size, _DynamicStringT<_CharType> &buffer)
{
	static const unsigned char Base64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static const _CharType crlf[3] = {'\r', '\n'};
	buffer.clear();
	if (!Size || !pPtr)
		return;
	_CharType szTmp[5] = {0,};
	buffer.reserve((Size * 4) / 3);
	const unsigned char *Data = (const unsigned char *)pPtr;
	for (unsigned i = 0; i < Size; i += 3)
	{
		//Note that every 19 symbols the "\r\n" should be concatenated
		//to the target string, so the maximum bytes count that should
		//be added to the output buffer during one iteration is 6 plus
		//null-terminator = 7.
		unsigned tempValue;
		if ((i + 2) < Size)
			tempValue = (Data[i] << 16) | (Data[i+1] << 8) | Data[i+2];
		else
		{
			tempValue = (Data[i] << 16);
			if ((i + 1) < Size)
				tempValue |= (Data[i+1] << 8);
			if ((i + 2) < Size)
				tempValue |= (Data[i+2] << 0);
		}

		szTmp[0] = Base64Alphabet[((tempValue>>18)&0x3F)];
		szTmp[1] = Base64Alphabet[((tempValue>>12)&0x3F)];
		szTmp[2] = Base64Alphabet[((tempValue>>6)&0x3F)];;
		szTmp[3] = Base64Alphabet[(tempValue&0x3F)];

		if ((i + 1) >= Size)
			szTmp[2] = '=';		
		if ((i + 2) >= Size)
			szTmp[3] = '=';		

		buffer.append(szTmp, 4);

		if ((i % 57) == 54)
			buffer.append(crlf, 2);
	}
}

template <class _CharType, class _BufferType> static inline bool Base64Decode(const _TempStringImplT<_CharType> &string, _BufferType &buffer)
{
	size_t size = string.length();
	unsigned nchar = 0;
	unsigned chopcnt = 0;
	unsigned tempValue = 0;
	unsigned char tmpBuf[3];
	buffer.reserve((size * 3) / 4);
	for (size_t i = 0; i < size; i++)
	{
		//BASE64 does not allow symbols with code less than 0x21 to
		//be used.
		_CharType tch = string[i];
		if ((tch > 0x7F) || (tch <= 0))
			continue;
		unsigned char ch = (unsigned char)tch;
		if (ch <= 0x0D)
			continue; //to filter 0x0d or 0x0a symbols (\r and \n)

		unsigned char decoded = 0;
		bool padding = false;
		if (!DecodeBase64Char(tch, decoded, padding))
			return false;

		if (padding)
			chopcnt++;
		else
		{
			if (chopcnt)
				return false;
		}

		switch (nchar++)
		{
		case 0:
			tempValue  = (decoded << 18);
			break;
		case 1:
			tempValue |= (decoded << 12);
			break;
		case 2:
			tempValue |= (decoded << 6);
			break;
		case 3:
			tempValue |= (decoded << 0);
			nchar = 0;
			tmpBuf[0] = (tempValue >> 16) & 0xFF;
			tmpBuf[1] = (tempValue >> 8) & 0xFF;
			tmpBuf[2] = (tempValue >> 0) & 0xFF;
			if (chopcnt > 2)
				return false;
			buffer.append(tmpBuf, 3 - chopcnt);

			break;
		}
	}
	if (nchar)
		return false;
	return true;
}

void Base64::Encode( const void *pPtr, unsigned Size, DynamicStringA &buffer )
{
	return Base64Encode(pPtr, Size, buffer);
}

void Base64::Encode( const void *pPtr, unsigned Size, DynamicStringW &buffer )
{
	return Base64Encode(pPtr, Size, buffer);
}

bool Base64::Decode( const _TempStringImplT<char> &string, BasicBuffer &buffer )
{
	return Base64Decode(string, buffer);
}

bool Base64::Decode( const _TempStringImplT<wchar_t> &string, BasicBuffer &buffer )
{
	return Base64Decode(string, buffer);
}

