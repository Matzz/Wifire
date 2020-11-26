#pragma once

#include <SmingCore.h>
#include "FileConfig.h"
#include "../Actions/utils.h"

class UserConfig {
public:
    bool enabled;
	String login;
	String salt;
	String hash;
	Vector<String> roles;

	static UserConfig newUser(String login, String password, Vector<String> roles) {
		auto salt = mkSalt();
		auto cfg = UserConfig();
		cfg.login = login;
		cfg.enabled = true;
		cfg.hash = getHash(salt + password);
		cfg.salt = salt;
		cfg.roles = roles;
		return cfg;
	};

private:
	static String mkSalt() {
		auto base = String(system_get_chip_id(), 10) + String(os_get_nanoseconds(), 10);
		return getHash(base);
	};
};

class UsersConfig {
public:
	Vector<UserConfig> users;
};

class UsersConfigProvider: FileConfig {
public:
	UsersConfigProvider(String fileName = "config/users.json");

	static UsersConfig jsonToConfig(JsonObject& doc);
	static void configToJson(UsersConfig& config, JsonObject& doc);

	UsersConfig load();
	void save(UsersConfig cfg);
};