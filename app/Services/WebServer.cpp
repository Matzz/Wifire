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
#include "../Actions/actionsHelpers.h"


WebServer::WebServer(const int port, UserSessionManager& sessionManager) :
		port(port), sessionManager(sessionManager) {
	server.setBodyParser(MIME_JSON, bodyToStringParser); 
}

void WebServer::start() {
	server.listen(port);
	bindActions();
}

String getAccessErrors(const String& requiredRole, HttpRequest &request, HttpResponse &response, UserSessionManager& sessionManager) {
	if(requiredRole == String::empty) {
		return String::empty;
	}
	String sessionId = getSessionId(request);
	if(sessionId == String::empty) {
		return F("You must signin to see this page.");
	}
	auto sessionOrErr = sessionManager.validateSession(sessionId);
	auto session = sessionOrErr.get_if_right();
	if(session == nullptr) {
    	UserSessionManager::clearSessionCookie(response);
		String* err = sessionOrErr.get_if_left();
		return *err;
	}
	if(session->login == "admin") {
		return String::empty;
	}
	if(!session->roles.contains(requiredRole)) {
		return F("You don't have required permissions to see this page.");
	}
	return String::empty;
}

void WebServer::authWrapper(const String& path, const String& requiredRole, const HttpPathDelegate& callback) {
	auto wrapper = [this, path, callback, requiredRole](HttpRequest &request, HttpResponse &response) {
		String maybeError = getAccessErrors(requiredRole, request, response, sessionManager);
		if(maybeError != String::empty) {
			returnFailure(response, maybeError);
			response.headers[HTTP_HEADER_LOCATION] = "/";
			response.code = HttpStatus::FORBIDDEN;
		} else {
			callback(request, response);
		}
	};
	server.paths.set(path, wrapper);
}

void WebServer::bindActions() {
	authWrapper("/info", "show_info", infoAction);
	authWrapper("/config/station/get", "edit_station_config", stationGetConfigAction);
	authWrapper("/config/station/set", "edit_station_config", stationSetConfigAction);
	authWrapper("/config/networks/refresh", "edit_station_config", stationRefreshNetworks);
	authWrapper("/config/networks/get", "edit_station_config", stationGetNetworks);
	authWrapper("/config/ap/get", "edit_ap_config", apGetConfigAction);
	authWrapper("/config/ap/set", "edit_ap_config", apSetConfigAction);
	authWrapper("/config/ota/get", "edit_ota_config", otaGetConfigAction);
	authWrapper("/config/ota/set", "edit_ota_config", otaSetConfigAction);
	authWrapper("/config/gpio/get", "edit_gpio_config", gpioGetConfigAction);
	authWrapper("/config/gpio/set", "edit_gpio_config", gpioSetConfigAction);
	authWrapper("/config/users/list", "edit_users_config", userListAction);
	authWrapper("/config/users/add", "edit_users_config", userAddAction);
	authWrapper("/config/users/edit", "edit_users_config", userEditAction);
	authWrapper("/config/users/remove", "edit_users_config", userRemoveAction);
	authWrapper("/gpio/switch", "switch_gpio", gpioSwitchAction);
	authWrapper("/signin", String::empty, signInAction);
	authWrapper("/signout", String::empty, signOutAction);

	server.paths.setDefault(staticAction);
}
