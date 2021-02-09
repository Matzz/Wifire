#include "usersAction.h"

#include <JsonObjectStream.h>
#include "actionsHelpers.h"
#include "../Services/Injector.h"
#include "../Configuration/Users/UsersConfig.h"
#include "../Configuration/Users/UserEditRequest.h"

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

void userAddAction(HttpRequest &request, HttpResponse &response) {
	Either<String, UserEditRequest> userAddOrError = decodeJson<UserEditRequest>(request);
	if(userAddOrError.is_left()) {
		return returnFailure(response, *userAddOrError.get_if_left());
	}
	UserEditRequest userToAdd = *userAddOrError.get_if_right();
	
	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();

	UsersConfig usersConfig;
	if(configOrError.is_left()) {
		debug_e("Invalid users config json. Creating new empty configuration.");
	} else {
		usersConfig = *configOrError.get_if_right();
	}
	bool added = usersConfig.addUser(userToAdd);

	if(added) {
		auto& sessionManager = Injector::getInstance().getUserSessionManager();
		sessionManager.signOutByLogin(userToAdd.login);
		provider.save(usersConfig);
		returnOk(response, "User added.");
	} else {
		JsonObjectStream* stream = new JsonObjectStream();
		JsonObject json = stream->getRoot();
		returnFailure(response, "User alredy exists.");
	}

}

void userEditAction(HttpRequest &request, HttpResponse &response) {
	Either<String, UserEditRequest> userEditOrError = decodeJson<UserEditRequest>(request);
	if(userEditOrError.is_left()) {
		return returnFailure(response, *userEditOrError.get_if_left());
	}
	UserEditRequest userToEdit = *userEditOrError.get_if_right();

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();

	UsersConfig usersConfig;
	if(configOrError.is_left()) {
		debug_e("Invalid users config json. Creating new empty configuration.");
	} else {
		usersConfig = *configOrError.get_if_right();
	}

	bool modified = usersConfig.editUser(userToEdit);

	if(modified) {
		auto& sessionManager = Injector::getInstance().getUserSessionManager();
		sessionManager.signOutByLogin(userToEdit.login);
		provider.save(usersConfig);
		returnOk(response, "User modified.");
	} else {
		returnFailure(response, "User doesn't exist.");
	}
}

void userRemoveAction(HttpRequest &request, HttpResponse &response) {
	Either<String, UserDeleteRequest> userDeleteOrError = decodeJson<UserDeleteRequest>(request);
	if(userDeleteOrError.is_left()) {
		return returnFailure(response, *userDeleteOrError.get_if_left());
	}
	UserDeleteRequest userToDelete = *userDeleteOrError.get_if_right();

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();

	if(configOrError.is_left()) {
		returnFailure(response, F("Invalid users config json."));
	}
	UsersConfig usersConfig = *configOrError.get_if_right();

	bool modified = usersConfig.removeUser(userToDelete);

	if(modified) {
		auto& sessionManager = Injector::getInstance().getUserSessionManager();
		sessionManager.signOutByLogin(userToDelete.login);
		provider.save(usersConfig);
		returnOk(response, "User removed.");
	} else {
		returnFailure(response, "User doesn't exist.");
	}
}