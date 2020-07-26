#include <SmingCore.h>
#include "Services/Injector.h"
#include "Services/SerialHandler.h"
#include "Services/SpiffsManager.h"


void onReady() {
	Injector &di = Injector::getInstance();
	di.getWiFiManager().startNetwork();
	di.getWebServer().start();
	di.getGPIOStateManager().update();;
	//di.getFtpServer().start();
}

void init() {
	SerialHandler::setup();
	SpiffsManager::setup();

	System.onReady(onReady);
}
