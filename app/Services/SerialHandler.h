#pragma once

#include <SmingCore.h>

class SerialHandler {
public:
	static void setup();

	static void callback(Stream& stream, char arrivedChar,
			unsigned short availableCharsCount);
};
