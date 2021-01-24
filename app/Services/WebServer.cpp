#include "WebServer.h"

#include <SmingCore.h>
#include "InfoProvider.h"
#include "../Actions/infoAction.h"
#include "../Actions/authAction.h"
#include "../Actions/staticAction.h"
#include "../Actions/configActions.h"
#include "../Actions/gpioSwitchAction.h"
#include "../Actions/usersAction.h"
#include "../Actions/authAction.h"


WebServer::WebServer(const int port) :
		port(port) {
}

void WebServer::start() {

	HttpServerSettings settings;
	settings.maxActiveConnections = 10;
	settings.keepAliveSeconds = 0;
	settings.minHeapSize = -1;
	settings.useDefaultBodyParsers = true;
	settings.closeOnContentError = true;
	server.configure(settings);

	server.listen(port);
	server.setTimeOut(1000);
	bindActions();
}

void WebServer::authWrapper(String path, const HttpPathDelegate& callback) {
	auto wrapper = [callback](HttpRequest &request, HttpResponse &response) {
		if(false) {
			response.headers[HTTP_HEADER_LOCATION] = "/";
		} else {
			callback(request, response);
		}
		
	};
	server.paths.set(path, wrapper);
}

void WebServer::bindActions() {
	authWrapper("/info", infoAction);
	authWrapper("/config/station/get", stationGetConfigAction);
	authWrapper("/config/station/set", stationSetConfigAction);
	authWrapper("/config/networks/refresh", stationRefreshNetworks);
	authWrapper("/config/networks/get", stationGetNetworks);
	authWrapper("/config/ap/get", apGetConfigAction);
	authWrapper("/config/ap/set", apSetConfigAction);
	authWrapper("/config/ota/get", otaGetConfigAction);
	authWrapper("/config/ota/set", otaSetConfigAction);
	authWrapper("/config/gpio/get", gpioGetConfigAction);
	authWrapper("/config/gpio/set", gpioSetConfigAction);
	authWrapper("/config/users/list", userListAction);
	authWrapper("/config/users/add", userAddAction);
	authWrapper("/config/users/edit", userEditAction);
	authWrapper("/config/users/remove", userRemoveAction);
	authWrapper("/gpio/on", gpioSwitchAction);
	authWrapper("/signin", signInAction);
	authWrapper("/signout", signOutAction);

	server.paths.setDefault(staticAction);
}
