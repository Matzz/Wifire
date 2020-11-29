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
	static String adminLogin;
	
	Vector<UserConfig> users;
	void addAdminIfDoesntExist();
	int findUser(String login);
	bool removeUser(String login);
	bool newUser(String login, bool enabled, String password, Vector<String> roles);
	bool editUser(String login, bool enabled, String password, Vector<String> roles);


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