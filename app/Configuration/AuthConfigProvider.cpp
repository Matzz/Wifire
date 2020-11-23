#include "AuthConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"


AuthConfigProvider::AuthConfigProvider(String fileName) :
			FileConfig(fileName) { }

AuthConfig AuthConfigProvider::jsonToConfig(JsonObject& doc) {
	AuthConfig cfg;
    cfg.salt = doc["salt"].as<String>();
	JsonArray usersArr = doc["users"].as<JsonArray>();
	int usersArrSize = usersArr.size();
    if(usersArr>100) {
        debug_e("Too many users! Loading only first 100.");
        usersArrSize = 100;
    }
	for(int i=0; i<=usersArr; i++) {
        auto userObj = usersArr[i].as<JsonObject>();
        auto user = UserConfig();
        user.enabled = userObj["enabled"].as<bool>();
        user.login = userObj["login"].as<String>();
        user.hash = userObj["hash"].as<String>();


        auto rolesArr = userObj["roles"].as<JsonArray>();
        int rolesArrSize = rolesArr.size();
        if(rolesArrSize>10) {
            debug_e("Too many roles! Loading only first 10.");
            rolesArrSize = 10;
        }
        for(int j=0; j<=rolesArrSize; j++) {
            auto role = rolesArr[j].as<String>();
            if(!role.empty) {
                user.roles.addElement(rolesArr[j].as<String>());
            }
        }
        cfg.users[i] = user;
	}
	return cfg;
}

void AuthConfigProvider::configToJson(AuthConfig& config, JsonObject& doc) {
    doc["salt"] = config.salt;
	JsonArray usersArr = doc.createNestedArray("users");
	for(int i=0; i<=config.users.size(); i++) {
        auto user = config.users[i];
		JsonObject userObj = usersArr.createNestedObject();
		userObj["login"] = user.login;
		userObj["hash"] = user.hash;
        JsonArray rolesArr = userObj.createNestedArray("roles");
        for(int j=0; j<=user.roles.size(); j++) {
		    rolesArr.add(user.roles[j]);
        }
	}
}

AuthConfig AuthConfigProvider::load() {
	debug_i("Loading GPIO config.");

	StaticJsonDocument<JSON_MAX_SIZE> doc;
	loadJsonObject(doc);
	JsonObject obj = doc.as<JsonObject>();
	return jsonToConfig(obj);
}

void AuthConfigProvider::save(AuthConfig config) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	JsonObject obj = doc.to<JsonObject>();
	configToJson(config, obj);
	saveJsonObject(doc);
}
