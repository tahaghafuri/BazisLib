#include "stdafx.h"
#include <bzsnet/base64.h>

using namespace BazisLib;
using namespace BazisLib::Network;

#define TEST_ASSERT(x) {if (!(x)) {__debugbreak(); return false;}}

static bool BufferEquals(const BasicBuffer &buffer, const char *expected)
{
	size_t length = strlen(expected);
	return (buffer.GetSize() == length) && !memcmp(buffer.GetConstData(), expected, length);
}

bool TestBase64()
{
	DynamicStringA encoded;
	Base64::Encode("Hello", 5, encoded);
	TEST_ASSERT(!strcmp(encoded.c_str(), "SGVsbG8="));

	BasicBuffer decoded;
	TEST_ASSERT(Base64::Decode(ConstStringA(encoded.c_str()), decoded));
	TEST_ASSERT(BufferEquals(decoded, "Hello"));

	BasicBuffer decodedWithWhitespace;
	TEST_ASSERT(Base64::Decode(ConstStringA("SGVs\r\nbG8="), decodedWithWhitespace));
	TEST_ASSERT(BufferEquals(decodedWithWhitespace, "Hello"));

	BasicBuffer invalidDecoded;
	TEST_ASSERT(!Base64::Decode(ConstStringA("SGVsbG8$"), invalidDecoded));
	TEST_ASSERT(!Base64::Decode(ConstStringA("SG=VsbG8="), invalidDecoded));

	DynamicStringW encodedWide;
	Base64::Encode("Hi", 2, encodedWide);
	TEST_ASSERT(!wcscmp(encodedWide.c_str(), L"SGk="));

	BasicBuffer decodedWide;
	TEST_ASSERT(Base64::Decode(ConstStringW(encodedWide.c_str()), decodedWide));
	TEST_ASSERT(BufferEquals(decodedWide, "Hi"));

	return true;
}
