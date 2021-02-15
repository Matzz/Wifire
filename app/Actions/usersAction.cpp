#include "usersAction.h"

#include <JsonObjectStream.h>
#include "actionsHelpers.h"
#include "../Services/Injector.h"
#include "../Model/StringVectorCodec.h"
#include "../Model/Users/UsersConfig.h"
#include "../Model/Users/UserEditRequest.h"

void userListAction(HttpRequest &request, HttpResponse &response) {
	JsonObjectStream* stream = new JsonObjectStream();
	JsonObject doc = stream->getRoot();

	ConfigProvider<UsersConfig>& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();
	if(configOrError.isLeft()) {
		return returnFailure(response, F("Invalid json. Please save configration again."));
	}
	UsersConfig config = *configOrError.getIfRight();
	
	JsonArray usersArr = doc.createNestedArray("users");
	auto users = config.getUsersList();
	for(int i=0; i<users.size(); i++) {
        UserConfig& user = users[i];
		JsonObject userObj = usersArr.createNestedObject();
		userObj["enabled"] = user.enabled;
		userObj["login"] = user.login;
        StringVectorCodec::encode(userObj, user.roles, "roles");
	}

	response.sendNamedStream(stream);
}

void userAddAction(HttpRequest &request, HttpResponse &response) {
	Either<String, UserEditRequest> userAddOrError = decodeJson<UserEditRequest>(request);
	if(userAddOrError.isLeft()) {
		return returnFailure(response, *userAddOrError.getIfLeft());
	}
	UserEditRequest userToAdd = *userAddOrError.getIfRight();
	
	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();

	UsersConfig usersConfig;
	if(configOrError.isLeft()) {
		debug_e("Invalid users config json. Creating new empty configuration.");
	} else {
		usersConfig = *configOrError.getIfRight();
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
	if(userEditOrError.isLeft()) {
		return returnFailure(response, *userEditOrError.getIfLeft());
	}
	UserEditRequest userToEdit = *userEditOrError.getIfRight();

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();

	UsersConfig usersConfig;
	if(configOrError.isLeft()) {
		debug_e("Invalid users config json. Creating new empty configuration.");
	} else {
		usersConfig = *configOrError.getIfRight();
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
	if(userDeleteOrError.isLeft()) {
		return returnFailure(response, *userDeleteOrError.getIfLeft());
	}
	UserDeleteRequest userToDelete = *userDeleteOrError.getIfRight();

	auto& provider = Injector::getInstance().getUsersConfigProvider();
	auto configOrError = provider.load();

	if(configOrError.isLeft()) {
		returnFailure(response, F("Invalid users config json."));
	}
	UsersConfig usersConfig = *configOrError.getIfRight();

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