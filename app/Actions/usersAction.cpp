#include <JsonObjectStream.h>
#include "../Services/Injector.h"
#include "usersAction.h"
#include "../Utils/utils.h"

void userListAction(HttpRequest &request, HttpResponse &response) {
	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto config = provider.load();

	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject doc = stream->getRoot();

	JsonArray usersArr = doc.createNestedArray("users");
	for(int i=0; i<config.users.size(); i++) {
        auto user = config.users[i];
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


// void userEditAction(HttpRequest &request, HttpResponse &response) {
// 	auto& provider = Injector::getInstance().getUsersConfigProvider();
// 	UsersConfig usersConfig = provider.load();
// 	String login = request.getPostParameter("login");


// 	bool enabled = getBool(request, "enabled");
// 	String password = getString(request, "password");
// 	String rolesString = getString(request, "roles");
// 	Vector<String> roles = Vector<String>();
// 	splitString(rolesString, ',', roles);
// 	auto modifiedUser = usersConfig.editUser(login, enabled, password, roles);

// 	JsonObjectStream* stream = new JsonObjectStream();
// 	JsonObject json = stream->getRoot();

// 	if(modifiedUser != nullptr) {
// 		provider.save(usersConfig);
// 		json["status"] = "successful";
// 		response.code = HttpStatus::BAD_REQUEST;
// 		debug_i("User %s modified.", login);
// 	} else {
// 		String msg = String("User ") + login + String(" doesn't exsist.");
// 		json["status"] = msg;
// 		debug_i(msg);
// 	}

// 	response.sendNamedStream(stream);
// }