#ifndef APP_SERVICES_SERIALHANDLER_H_
#define APP_SERVICES_SERIALHANDLER_H_

#include <SmingCore/SmingCore.h>

class SerialHandler {
public:
	static void setup();

	static void callback(Stream& stream, char arrivedChar,
			unsigned short availableCharsCount);
};

#endif /* APP_SERVICES_SERIALHANDLER_H_ */
