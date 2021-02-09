#include <JsonObjectStream.h>
#include "usersAction.h"
#include "../Services/Injector.h"
#include "../Utils/utils.h"
#include "../Configuration/Users/UsersConfig.h"

void userListAction(HttpRequest &request, HttpResponse &response) {
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject doc = stream->getRoot();

	ConfigProvider<UsersConfig>& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	UsersConfig config = *configOrError.get_if_right();
	
	JsonArray usersArr = doc.createNestedArray("users");
	auto users = config.getUsersList();
	for(int i=0; i<users.size(); i++) {
        UserConfig& user = users[i];
		JsonObject userObj = usersArr.createNestedObject();
		userObj["enabled"] = user.enabled;
		userObj["login"] = user.login;
        JsonArray rolesArr = userObj.createNestedArray("roles");
        for(int j=0; j<user.roles.size(); j++) {
		    rolesArr.add(user.roles[j]);
        }
	}

	response.sendNamedStream(stream);
}

Vector<String> parseRoles(HttpRequest &request) {
	String rolesString = getString(request, "roles");
	Vector<String> roles;
	splitString(rolesString, ',', roles);
	for(int j=0; j<roles.size(); j++) {
		roles[j].trim();
	}
	// TODO drop empty roles;
	return roles;
}

void userAddAction(HttpRequest &request, HttpResponse &response) {

	UserConfig user(
		getBool(request, "enabled"),
		getString(request, "login"),
		getString(request, "password"),
		parseRoles(request)
	);

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	UsersConfig config = *configOrError.get_if_right();
	bool added = config.addUser(user);

	if(added) {
		auto& sessionManager = Injector::getInstance().getUserSessionManager();
		sessionManager.signOutByLogin(user.login);
		provider.save(config);
	} else {
		JsonObjectStream* stream = new JsonObjectStream();
		JsonObject json = stream->getRoot();
		returnFailure(response, "User alredy exists.");
	}

}

void userEditAction(HttpRequest &request, HttpResponse &response) {

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	UsersConfig config = *configOrError.get_if_right();
	String login = getString(request, "login");

	bool modified = config.editUser(
		getBool(request, "enabled"),
		login,
		getString(request, "password"),
		parseRoles(request)
	);

	if(modified) {
		auto& sessionManager = Injector::getInstance().getUserSessionManager();
		sessionManager.signOutByLogin(login);
		provider.save(config);
	} else {
		returnFailure(response, "User doesn't exist.");
	}
}

void userRemoveAction(HttpRequest &request, HttpResponse &response) {

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.is_left()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	UsersConfig config = *configOrError.get_if_right();
	bool removed = config.removeUser(getString(request, "login"));

	if(removed) {
		provider.save(config);
	} else {
		returnFailure(response, "User doesn't exist.");
	}
}