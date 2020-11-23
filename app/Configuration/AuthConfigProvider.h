#pragma once

#include <SmingCore.h>
#include "FileConfig.h"

class UserConfig {
public:
    bool enabled;
	String login;
	String hash;
	Vector<String> roles;
};

class AuthConfig {
public:
	String salt;
	Vector<UserConfig> users;
};

class AuthConfigProvider: FileConfig {
public:
	AuthConfigProvider(String fileName = "config/auth.json");

	static AuthConfig jsonToConfig(JsonObject& doc);
	static void configToJson(AuthConfig& config, JsonObject& doc);

	AuthConfig load();
	void save(AuthConfig cfg);
};
