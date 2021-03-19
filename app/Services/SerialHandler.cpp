#include "SerialHandler.h"

#include <SmingCore.h>
#include "Injector.h"
#include "StatusProvider.h"

void SerialHandler::setup() {
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Debug output to serial
	Serial.onDataReceived(SerialHandler::callback);
	Serial.println("Serial handler set.");
}

void SerialHandler::callback(Stream& stream, char arrivedChar,
		unsigned short availableCharsCount) {

	Injector &injector = Injector::getInstance();

	if (arrivedChar == '\n') {
		char str[availableCharsCount];
		for (int i = 0; i < availableCharsCount; i++) {
			str[i] = stream.read();
			if (str[i] == '\r' || str[i] == '\n') {
				str[i] = '\0';
			}
		}

		if (!strcmp(str, "ota")) {
			injector.getOtaUpdater().update();
		} else if (!strcmp(str, "switch")) {
			injector.getOtaUpdater().switchSlot();
		} else if (!strcmp(str, "restart")) {
			System.restart();
		} else if (!strcmp(str, "ls")) {
			Vector<String> files = fileList();
			for(auto& f: files) {
				Serial.println(f);
			}
		} else if (!strncmp(str, "cat ", 4)) {
			Vector<String> files = fileList();
			Serial.printf("filecount %d\r\n", files.count());
			String fileName = String(str).substring(4);
			if(fileExist(fileName)) {
				Serial.println(fileGetContent(fileName));
			} else {
				Serial.printf("File '%s' does not exist.\r\n", fileName.c_str());
			}
		} else if (!strncmp(str, "rm ", 3)) {
			String fileName = String(str).substring(3);
			if(fileExist(fileName)) {
				fileDelete(fileName);
				Serial.printf("Deleted '%s'\r\n", fileName.c_str());
			} else {
				Serial.printf("File '%s' does not exist.\r\n", fileName.c_str());
			}
		} else if (!strcmp(str, "status")) {
			auto status = StatusProvider::getStatus(true);
			auto statusSize = status->count();
			for (int i = 0; i < statusSize; i++) {
				Serial.printf("%s - %s\r\n", status->keyAt(i).c_str(),
						status->valueAt(i).c_str());
			}
			delete status;
		} else if (!strcmp(str, "help")) {
			Serial.println();
			Serial.println("available commands:");
			Serial.println("  help - display this message");
			Serial.println("  restart - restart the esp8266");
			Serial.println("  switch - switch to the other rom and reboot");
			Serial.println("  ota - perform ota update, switch rom and reboot");
			Serial.println("  status - show esp8266 and wifi status");
			Serial.println("  ls - list files in spiffs");
			Serial.println("  cat - show first file in spiffs");
			Serial.println();
		} else if (!strcmp(str, "")) {
			// No command
			Serial.println();
		} else {
			Serial.println("unknown command");
		}
	}
}
