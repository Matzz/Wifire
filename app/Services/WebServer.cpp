#include "WebServer.h"

#include <SmingCore.h>
#include "InfoProvider.h"
#include "../Actions/infoAction.h"
#include "../Actions/staticAction.h"
#include "../Actions/configActions.h"
#include "../Actions/gpioSwitchAction.h"


WebServer::WebServer(const int port) :
		port(port) {
}

void WebServer::start() {
	server.listen(port);
	bindActions();
}

void WebServer::bindActions() {
	server.paths.set("/info", infoAction);
	server.paths.set("/config/station/get", stationGetConfigAction);
	server.paths.set("/config/station/set", stationSetConfigAction);
	server.paths.set("/config/ap/get", apGetConfigAction);
	server.paths.set("/config/ap/set", apSetConfigAction);
	server.paths.set("/config/ota/get", otaGetConfigAction);
	server.paths.set("/config/ota/set", otaSetConfigAction);
	server.paths.set("/config/gpio/get", gpioGetConfigAction);
	server.paths.set("/config/gpio/set", gpioSetConfigAction);
	server.paths.set("/gpio/on", gpioSwitchAction);
	server.paths.setDefault(staticAction);
}
