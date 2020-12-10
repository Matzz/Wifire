#include <SmingCore.h>
#include "Services/Injector.h"
#include "Services/SerialHandler.h"
#include "Services/SpiffsManager.h"


void onReady() {
	Serial.println("System ready callback.");
	Injector &di = Injector::getInstance();
	di.getWebServer().start();
	// di.getGPIOStateManager().update();
	//di.getFtpServer().start();
}

void init() {
	SerialHandler::setup();
	SpiffsManager::setup();
	Injector &di = Injector::getInstance();
	di.getWiFiManager().startNetwork();

	System.onReady(onReady);
}
