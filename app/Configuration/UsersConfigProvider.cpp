#include "UsersConfigProvider.h"

#include <SmingCore.h>
#include "FileConfig.h"
#include "../Utils/utils.h"

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
    cfg.addAdminIfDoesntExist();
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
	auto config = jsonToConfig(obj);
    return config;
}

void UsersConfigProvider::save(UsersConfig config) {
	StaticJsonDocument<JSON_MAX_SIZE> doc;
	JsonObject obj = doc.to<JsonObject>();
	configToJson(config, obj);
	saveJsonObject(doc);
}

String UsersConfig::adminLogin = "admin";

void UsersConfig::addAdminIfDoesntExist() {
    if(!hasAdmin()) {
        auto admin = UsersConfig::newUser(
            UsersConfig::adminLogin,
            UsersConfig::adminLogin);
        admin.roles.addElement(UsersConfig::adminLogin);
        users.addElement(admin);
    }
}

bool UsersConfig::hasAdmin() {
    bool found = false;
    auto size = users.size();
    for(int i=0; i<size && !found; i++) {
        if(users[i].login == UsersConfig::adminLogin) {
            found = true;
        }
    }
    return found;
}


UserConfig UsersConfig::newUser(String login, String password) {
    auto salt = mkSalt();
    auto cfg = UserConfig();
    cfg.login = login;
    cfg.enabled = true;
    cfg.hash = getHash(salt + password);
    cfg.salt = salt;
    return cfg;
}

String UsersConfig::mkSalt() {
    auto base = String(system_get_chip_id(), 10) + String(os_get_nanoseconds(), 10);
    return getHash(base);
}