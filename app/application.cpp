#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "Services/Injector.h"
#include "Services/SerialHandler.h"
#include "Services/SpiffsManager.h"

void init() {
	SerialHandler::setup();
	SpiffsManager::setup();

	Injector di = Injector::getInstance();

	di.getWiFiManager().startNetwork();
	di.getWebServer().start();
	di.getFtpServer().start();
}
