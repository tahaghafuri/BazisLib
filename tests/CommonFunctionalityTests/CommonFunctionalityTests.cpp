// CommonFunctionalityTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

bool TestBufferedSockets();
bool TestBase64();

int _tmain(int argc, _TCHAR* argv[])
{
	if (!TestBufferedSockets())
		__asm int 3;
	if (!TestBase64())
		__asm int 3;


	return 0;
}

