#include <JsonObjectStream.h>
#include "../Services/Injector.h"
#include "usersAction.h"
#include "utils.h"

void getUsersListAction(HttpRequest &request, HttpResponse &response) {
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