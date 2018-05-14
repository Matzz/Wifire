#include "SerialHandler.h"

#include <SmingCore/SmingCore.h>
#include "Injector.h"
#include "InfoProvider.h"

void SerialHandler::setup() {
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Debug output to serial
	Serial.setCallback(SerialHandler::callback);
}

void SerialHandler::callback(Stream& stream, char arrivedChar,
		unsigned short availableCharsCount) {

	auto injector = Injector::getInstance();

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
			Serial.printf("filecount %d\r\n", files.count());
			for (unsigned int i = 0; i < files.count(); i++) {
				Serial.println(files[i]);
			}
		} else if (!strncmp(str, "cat ", 4)) {

			Vector<String> files = fileList();
			int idx = -1;
			String fileName = String(str).substring(4);
			Serial.println("Searching for "+fileName);
			for (unsigned int i = 0; i < files.count() && idx == -1; i++) {
				if (files[i].equals(fileName)) {
					idx = i;
				}
			}
			if (idx >= 0) {
				Serial.printf("dumping file %s:\r\n", files[idx].c_str());
				Serial.println(fileGetContent(files[idx]));
			} else {
				Serial.println("File not found.");
			}
		} else if (!strcmp(str, "info")) {
			auto info = InfoProvider::getInfo(true);
			auto infoSize = info->count();
			for (int i = 0; i < infoSize; i++) {
				Serial.printf("%s - %s\r\n", info->keyAt(i).c_str(),
						info->valueAt(i).c_str());
			}
			delete info;
		} else if (!strcmp(str, "help")) {
			Serial.println();
			Serial.println("available commands:");
			Serial.println("  help - display this message");
			Serial.println("  connect - connect to wifi");
			Serial.println("  restart - restart the esp8266");
			Serial.println("  switch - switch to the other rom and reboot");
			Serial.println("  ota - perform ota update, switch rom and reboot");
			Serial.println("  info - show esp8266 and wifi info");
			Serial.println("  ls - list files in spiffs");
			Serial.println("  cat - show first file in spiffs");
			Serial.println();
		} else {
			Serial.println("unknown command");
		}
	}
}
