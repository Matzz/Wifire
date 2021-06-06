#include "WebServer.h"

#include <SmingCore.h>
#include "StatusProvider.h"
#include "../Actions/statusAction.h"
#include "../Actions/authAction.h"
#include "../Actions/staticAction.h"
#include "../Actions/configActions.h"
#include "../Actions/gpioAction.h"
#include "../Actions/usersAction.h"
#include "../Actions/authAction.h"
#include "../Actions/actionsHelpers.h"


WebServer::WebServer(const int port, UserSessionManager& sessionManager) :
		port(port), sessionManager(sessionManager) {
	server.setBodyParser(MIME_JSON, bodyToStringParser);
	bindDefaultActions();
}

void WebServer::start() {
	debug_i("Starting web server. Port: %d", port);
	server.listen(port);
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
	auto session = sessionOrErr.getIfRight();
	if(session == nullptr) {
    	UserSessionManager::clearSessionCookie(response);
		String* err = sessionOrErr.getIfLeft();
		return *err;
	}
	if(session->roles.contains("admin") ) {
		return String::empty;
	}
	if(!session->roles.contains(requiredRole)) {
		return F("You don't have required permissions to see this page.");
	}
	return String::empty;
}

/**
 * Add web server action.
 * Returns true if action was added false if action already exists and overrideIfExists was set to false
 **/
bool WebServer::addAction(const String& path, const String& requiredRole, const HttpPathDelegate& callback, bool overrideIfExists) {
	bool exists = server.paths.contains(path);
	if(exists && !overrideIfExists) {
		return false;
	}
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
	return true;
}

void WebServer::bindDefaultActions() {
	addAction("/status", "show_status", statusAction);
	addAction("/config/station/get", "edit_station_config", stationGetConfigAction);
	addAction("/config/station/set", "edit_station_config", stationSetConfigAction);
	addAction("/config/networks/refresh", "edit_station_config", stationRefreshNetworks);
	addAction("/config/networks/get", "edit_station_config", stationGetNetworks);
	addAction("/config/ap/get", "edit_ap_config", apGetConfigAction);
	addAction("/config/ap/set", "edit_ap_config", apSetConfigAction);
	addAction("/config/ota/get", "edit_ota_config", otaGetConfigAction);
	addAction("/config/ota/set", "edit_ota_config", otaSetConfigAction);
	addAction("/config/gpio/get", "edit_gpio_config", gpioGetConfigAction);
	addAction("/config/gpio/set", "edit_gpio_config", gpioSetConfigAction);
	addAction("/config/users/list", "edit_users_config", userListAction);
	addAction("/config/users/add", "edit_users_config", userAddAction);
	addAction("/config/users/edit", "edit_users_config", userEditAction);
	addAction("/config/users/remove", "edit_users_config", userRemoveAction);
	addAction("/gpio/switch", "switch_gpio", gpioSwitchAction);
	addAction("/gpio/available", "switch_gpio", gpioSwitchGetAvailableAction);
	addAction("/signin", String::empty, signInAction);
	addAction("/signout", String::empty, signOutAction);

	server.paths.setDefault(staticAction);
}
