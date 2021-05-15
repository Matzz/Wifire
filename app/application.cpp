#include <SmingCore.h>
#include "Services/Injector.h"
#include "Services/SerialHandler.h"
#include "Services/SpiffsManager.h"
#include "Model/GPIOConfig.h"

char chipname[] = "testtesttest";
void onReady() {
	debug_i("System ready callback.");
	Injector &di = Injector::getInstance();
	di.getWiFiManager().startNetwork();
	di.getWebServer().start();
	di.getGPIOStateManager().update();
	//di.getFtpServer().start();
	debug_i("System ready callback finished.");
}

void init() {
	SerialHandler::setup();
	SpiffsManager::setup();
	Injector &di = Injector::getInstance();
	di.getWiFiManager().initNetwork();
	System.onReady(onReady);
}
