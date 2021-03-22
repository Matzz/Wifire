#pragma once

#include <SmingCore.h>
#include "Services/CommandProcessing/CommandProcessingIncludes.h"

class SerialHandler {
public:
	static void setup();

	static void lsCommand(String commandLine, CommandOutput* commandOutput);
	static void catCommand(String commandLine, CommandOutput* commandOutput);
	static void rmCommand(String commandLine, CommandOutput* commandOutput);
	static void restartCommand(String commandLine, CommandOutput* commandOutput);
	static void statusCommand(String commandLine, CommandOutput* commandOutput);
	static void otaCommand(String commandLine, CommandOutput* commandOutput);
	static void switchSlotCommand(String commandLine, CommandOutput* commandOutput);

	// static void callback(Stream& stream, char arrivedChar,
	// 		unsigned short availableCharsCount);
};
