#include "WebServer.h"

#include <SmingCore.h>
#include "InfoProvider.h"
#include "../Actions/infoAction.h"
#include "../Actions/staticAction.h"
#include "../Actions/configActions.h"


WebServer::WebServer(int port) :
		port(port) {
}
void WebServer::start() {
	server.listen(port);
	bindActions();
}
void WebServer::bindActions() {
	server.addPath("/info", infoAction);
	server.addPath("/config/station/get", stationGetConfigAction);
	server.addPath("/config/station/set", stationSetConfigAction);
	server.addPath("/config/ap/get", apGetConfigAction);
	server.addPath("/config/ap/set", apSetConfigAction);
	server.addPath("/config/ota/get", otaGetConfigAction);
	server.addPath("/config/ota/set", otaSetConfigAction);
	server.setDefaultHandler(staticAction);
}
