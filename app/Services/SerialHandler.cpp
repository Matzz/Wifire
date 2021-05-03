#include "SerialHandler.h"
#include <Debug.h>
#include "Injector.h"
#include "StatusProvider.h"

void SerialHandler::setup() {
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	commandHandler.registerSystemCommands();
	Debug.setDebug(Serial);
	Serial.systemDebugOutput(true);
	Serial.commandProcessing(true);

	String group = F("Wifire commands");
	commandHandler.setCommandPrompt(_F("Wifire>"));
	commandHandler.registerCommand(CommandDelegate(F("ls"), F("List files for provided paths."), group, lsCommand));
	commandHandler.registerCommand(CommandDelegate(F("cat"), F("Concatenate files and print."), group, catCommand));
	commandHandler.registerCommand(CommandDelegate(F("rm"), F("Remove files."), group, rmCommand));
	commandHandler.registerCommand(CommandDelegate(F("restart"), F("Restart ESP chip."), group, restartCommand));
	commandHandler.registerCommand(CommandDelegate(F("info"), F("Display system information."), group, statusCommand));
	commandHandler.registerCommand(CommandDelegate(F("ota"), F("Run ota updater."), group, otaCommand));
	commandHandler.registerCommand(CommandDelegate(F("switch_slot"), F("Switch firmware slot."), group, switchSlotCommand));

	Serial.println("Serial handler set.");
}

Vector<String> getParams(String commandLine) {
	Vector<String> parameters;
	splitString(commandLine, ' ', parameters);
	parameters.removeElementAt(0);
	return parameters;
}

void fileNotExist(const String &fileName, CommandOutput* commandOutput) {
	commandOutput->printf(_F("File '%s' does not exist.\r\n"), fileName.c_str());
}

void SerialHandler::lsCommand(String commandLine, CommandOutput* commandOutput) {
	commandOutput->println();

	Vector<String> parameters = getParams(commandLine);
	
	if(parameters.size()==0) {
		parameters.add("/");
	}

	for(auto& dirName: parameters) {
		Serial.printf("Directory %s:\r\n", dirName.c_str());
		Directory dir;
		if(dir.open(dirName)) {
			while(dir.next()) {
				commandOutput->println(dir.stat().name);
			}
		}
	}
}

void SerialHandler::catCommand(String commandLine, CommandOutput* commandOutput) {
	commandOutput->println();

	Vector<String> parameters = getParams(commandLine);
	if(parameters.size()==0) {
		commandOutput->println("Please provide file name.");
		return;
	}
	
	for(auto& fileName: parameters) {
		if(fileExist(fileName)) {
			commandOutput->println(fileGetContent(fileName));
		} else {
			fileNotExist(fileName, commandOutput);
		}
	}

}

void SerialHandler::rmCommand(String commandLine, CommandOutput* commandOutput) {
	commandOutput->println();

	Vector<String> parameters = getParams(commandLine);
	if(parameters.size()==0) {
		commandOutput->println(_F("Please provide file name."));
		return;
	}
	
	for(auto& fileName: parameters) {
		if(fileExist(fileName)) {
			fileDelete(fileName);
			commandOutput->printf(_F("File '%s' removed.\r\n"), fileName.c_str());
		} else {
			fileNotExist(fileName, commandOutput);
		}
	}
}

void SerialHandler::restartCommand(String commandLine, CommandOutput* commandOutput) {
	commandOutput->println();

	commandOutput->println(_F("Restarting..."));
	System.restart();
}

void SerialHandler::statusCommand(String commandLine, CommandOutput* commandOutput) {
	Status status = StatusProvider::getStatus(true);
	DynamicJsonDocument doc(JSON_MAX_SIZE);
	CodecHelpers::encodeDoc<Status>(doc, status);
	String jsonString;
	serializeJsonPretty(doc, jsonString);
	commandOutput->println(jsonString);
}

void SerialHandler::otaCommand(String commandLine, CommandOutput* commandOutput) {
	commandOutput->println();

	Injector &injector = Injector::getInstance();
	commandOutput->println(_F("OTA update..."));
	injector.getOtaUpdater().update();
}

void SerialHandler::switchSlotCommand(String commandLine, CommandOutput* commandOutput) {
	commandOutput->println();
	
	Injector &injector = Injector::getInstance();
	commandOutput->println(_F("Switching firmeware slot..."));
	injector.getOtaUpdater().switchSlot();
}