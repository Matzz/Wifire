#include "UsersConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"

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
	for(int i=0; i<usersArr; i++) {
        auto userObj = usersArr[i].as<JsonObject>();
        auto user = UserConfig();
        user.enabled = userObj["enabled"].as<bool>();
        user.login = userObj["login"].as<String>();
        user.hash = userObj["hash"].as<String>();
        user.salt = userObj["salt"].as<String>();


        auto rolesArr = userObj["roles"].as<JsonArray>();
        int rolesArrSize = rolesArr.size();
        if(rolesArrSize>MAX_ROLES) {
            debug_e("Too many roles! Loading only first %d.", MAX_ROLES);
            rolesArrSize = MAX_ROLES;
        }
        for(int j=0; j<rolesArrSize; j++) {
            auto role = rolesArr[j].as<String>();
            if(!role.empty) {
                user.roles.addElement(rolesArr[j].as<String>());
            }
        }
        cfg.users.addElement(user);
	}
	return cfg;
}

void UsersConfigProvider::configToJson(UsersConfig& config, JsonObject& doc) {
	JsonArray usersArr = doc.createNestedArray("users");
	for(int i=0; i<=config.users.size(); i++) {
        auto user = config.users[i];
		JsonObject userObj = usersArr.createNestedObject();
		userObj["login"] = user.login;
		userObj["hash"] = user.hash;
		userObj["salt"] = user.salt;
        JsonArray rolesArr = userObj.createNestedArray("roles");
        for(int j=0; j<=user.roles.size(); j++) {
		    rolesArr.add(user.roles[j]);
        }
	}
}

UsersConfig UsersConfigProvider::load() {
	debug_i("Loading users config.");

	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	JsonObject obj = doc.as<JsonObject>();
	return jsonToConfig(obj);
}

void UsersConfigProvider::save(UsersConfig config) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	JsonObject obj = doc.to<JsonObject>();
	configToJson(config, obj);
	saveJsonObject(doc);
}
