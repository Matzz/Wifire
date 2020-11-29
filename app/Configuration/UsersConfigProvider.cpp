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


// --- Users Config

String UsersConfig::adminLogin = "admin";

void UsersConfig::addAdminIfDoesntExist() {
    Vector<String> roles = Vector<String>();
    roles.addElement(UsersConfig::adminLogin);
    newUser(UsersConfig::adminLogin, true, UsersConfig::adminLogin, roles);
}

int UsersConfig::findUser(String login) {
    for(int i=0; i<users.size(); i++) {
        if(users[i].login==login) {
            return i;
        }
    }
    return -1;
}

bool UsersConfig::removeUser(String login) {
    int idx = findUser(login);
    if(idx>=0) {
        users.remove(idx);
        return true;
    }
    return false;
}

bool UsersConfig::newUser(String login, bool enabled, String password, Vector<String> roles) {
    if(findUser(login) == -1) {
        auto salt = mkSalt();
        auto cfg = UserConfig();
        cfg.login = login;
        cfg.enabled = enabled;
        cfg.hash = getHash(salt + password);
        cfg.salt = salt;
        cfg.roles = roles;
        users.addElement(cfg);
        return true;
    }
    return false;
}

bool UsersConfig::editUser(String login, bool enabled, String password, Vector<String> roles) {
    int idx = findUser(login);
    if(idx >= 0) {
        auto cfg = users[idx];
        cfg.enabled = enabled;
        if(!password.empty) {
            cfg.hash = getHash(cfg.salt + password);
        }
        cfg.roles = roles;
        return true;
    }
    return false;
}

String UsersConfig::mkSalt() {
    auto base = String(system_get_chip_id(), 10) + String(os_get_nanoseconds(), 10);
    return getHash(base);
}

// --- END Users Config