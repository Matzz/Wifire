#include "UsersConfigProvider.h"

#include <SmingCore.h>
#include "../../Utils/utils.h"

#define MAX_USERS 100
#define MAX_ROLES 10

UsersConfigProvider::UsersConfigProvider(String fileName) :
			FileConfig(fileName) { }

UsersConfig UsersConfigProvider::jsonToConfig(JsonObject& doc) {
	UsersConfig cfg;
	JsonArray usersArr = doc["users"].as<JsonArray>();
	int usersArrSize = usersArr.size();

    if(usersArrSize>MAX_USERS) {
        debug_e("Too many users! Loading only first %d.", MAX_USERS);
        usersArrSize = MAX_USERS;
    }
	for(int i=0; i<usersArrSize; i++) {
        JsonObject userObj = usersArr[i].as<JsonObject>();
        JsonArray rolesArr = userObj["roles"].as<JsonArray>();
        unsigned int rolesArrSize = rolesArr.size();
        if(rolesArrSize>MAX_ROLES) {
            debug_e("Too many roles! Loading only first %d.", MAX_ROLES);
            rolesArrSize = MAX_ROLES;
        }
        Vector<String> roles(rolesArrSize,  10);
        for(int j=0; j<rolesArrSize; j++) {
            String role = rolesArr[j].as<String>();
            if(role.length() > 0) {
                roles.addElement(role);
            }
        }
        UserConfig user(
            userObj["enabled"].as<bool>(),
            userObj["login"].as<String>(),
            userObj["hash"].as<String>(),
            userObj["salt"].as<String>(),
            roles
        );

        cfg.addUser(user);
	}

    cfg.addAdminIfDoesntExist();
    
	return cfg;
}

void UsersConfigProvider::configToJson(UsersConfig& config, JsonObject& doc) {
	JsonArray usersArr = doc.createNestedArray("users");
    const Vector<UserConfig> users = config.getUsersList();
	for(int i=0; i<users.size(); i++) {
        auto user = users[i];
		JsonObject userObj = usersArr.createNestedObject();
		userObj["enabled"] = user.enabled;
		userObj["login"] = user.login;
		userObj["hash"] = user.hash;
		userObj["salt"] = user.salt;
        JsonArray rolesArr = userObj.createNestedArray("roles");
        for(int j=0; j<user.roles.size(); j++) {
		    rolesArr.add(user.roles[j]);
        }
	}
}

UsersConfig UsersConfigProvider::load() {
	debug_i("Loading users config.");

	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	JsonObject obj = doc.as<JsonObject>();
	auto config = jsonToConfig(obj);
    return config;
}

void UsersConfigProvider::save(UsersConfig config) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	JsonObject obj = doc.to<JsonObject>();
	configToJson(config, obj);
	saveJsonObject(doc);
}