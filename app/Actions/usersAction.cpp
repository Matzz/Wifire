#include <JsonObjectStream.h>
#include "usersAction.h"
#include "../Services/Injector.h"
#include "../Utils/utils.h"
#include "../Configuration/Users/UsersConfig.h"

void userListAction(HttpRequest &request, HttpResponse &response) {
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject doc = stream->getRoot();

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto config = provider.load();

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
	UsersConfig usersConfig = provider.load();
	bool added = usersConfig.addUser(user);

	if(added) {
		provider.save(usersConfig);
	} else {
		JsonObjectStream* stream = new JsonObjectStream();
		JsonObject json = stream->getRoot();
		returnFailure(response, json, "User alredy exists.");
		response.sendNamedStream(stream);
	}

}

void userEditAction(HttpRequest &request, HttpResponse &response) {

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	UsersConfig usersConfig = provider.load();

	bool modified = usersConfig.editUser(
		getBool(request, "enabled"),
		getString(request, "login"),
		getString(request, "password"),
		parseRoles(request)
	);

	if(modified) {
		provider.save(usersConfig);
	} else {
		JsonObjectStream* stream = new JsonObjectStream();
		JsonObject json = stream->getRoot();
		returnFailure(response, json, "User doesn't exist.");
		response.sendNamedStream(stream);
	}
}

void userRemoveAction(HttpRequest &request, HttpResponse &response) {

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	UsersConfig usersConfig = provider.load();
	bool removed = usersConfig.removeUser(getString(request, "login"));

	if(removed) {
		provider.save(usersConfig);
	} else {
		JsonObjectStream* stream = new JsonObjectStream();
		JsonObject json = stream->getRoot();
		returnFailure(response, json, "User doesn't exist.");
		response.sendNamedStream(stream);
	}
}