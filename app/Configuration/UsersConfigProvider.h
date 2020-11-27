#pragma once

#include <SmingCore.h>
#include "FileConfig.h"

class UserConfig {
public:
    bool enabled;
	String login;
	String salt;
	String hash;
	Vector<String> roles;
};

class UsersConfig {
public:
	Vector<UserConfig> users;
	bool hasAdmin();
	void addAdminIfDoesntExist();

	static String adminLogin;
	static UserConfig newUser(String login, String password);

private:
	static String mkSalt();
};

class UsersConfigProvider: FileConfig {
public:
	UsersConfigProvider(String fileName = "config/users.json");

	static UsersConfig jsonToConfig(JsonObject& doc);
	static void configToJson(UsersConfig& config, JsonObject& doc);

	UsersConfig load();
	void save(UsersConfig cfg);
};