#pragma once

#include <SmingCore.h>
#include "FileConfig.h"
#include "UsersConfig.h"

class UsersConfigProvider: FileConfig {
public:
	UsersConfigProvider(String fileName = "config/users.json");

	static UsersConfig jsonToConfig(JsonObject& doc);
	static void configToJson(UsersConfig& config, JsonObject& doc);

	UsersConfig load();
	void save(UsersConfig cfg);
};