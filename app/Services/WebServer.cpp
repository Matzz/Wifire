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
#include "../Utils/utils.h"


WebServer::WebServer(const int port, UserSessionManager& sessionManager) :
		port(port), sessionManager(sessionManager) {
}

void WebServer::start() {
	server.listen(port);
	bindActions();
}

bool hasAccess(const String& requiredRole, HttpRequest &request, HttpResponse &response, UserSessionManager& sessionManager) {
	debug_i("Requested role: %s", requiredRole.c_str());
	if(requiredRole == String::empty) {
		return true;
	debug_i("Getting session");
	}
	String sessionId = getSessionId(request);
	if(sessionId == String::empty) {
		debug_i("Session id is empty");
		return false;
	}
	auto sessionOrErr = sessionManager.validateSession(sessionId);
	auto session = sessionOrErr.get_if_right();
	if(session == nullptr) {
		debug_i("Removing session.");
    	UserSessionManager::clearSessionCookie(response);
		return false;
	}
	debug_i("Session %s.", session->login);
	if(session->login == "admin") {
		debug_i("Admin, return true.");
		return true;
	}
		debug_i("Calling contains.");
	return session->roles.contains(requiredRole);
}

void WebServer::authWrapper(const String& path, const String& requiredRole, const HttpPathDelegate& callback) {
	auto wrapper = [this, path, callback, requiredRole](HttpRequest &request, HttpResponse &response) {
		bool accessGranted = hasAccess(requiredRole, request, response, sessionManager);
		if(!accessGranted) {
			response.headers[HTTP_HEADER_LOCATION] = "/";
			response.code = HttpStatus::FORBIDDEN;
			response.sendString("{\"status\": \"access_refused\"}");
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
