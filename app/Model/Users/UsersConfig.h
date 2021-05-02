#pragma once

#include <SmingCore.h>
#include "../Codec.h"
#include "../StringVectorCodec.h"
#include "UserEditRequest.h"
#include "UserDeleteRequest.h"


class UserConfig {
public:
    bool enabled;
	String login;
	String salt;
	String hash;
	Vector<String> roles;

	UserConfig();
    UserConfig(bool enabled, String login, String salt, String hash, const Vector<String> &roles);
    UserConfig(bool enabled, String login, String password, const Vector<String> &roles);

	bool checkPassword(String password) const;
	inline void setPassword(const String& password) {
    	hash = UserConfig::getHash(salt + password);
	}

	static String getHash(const String& base);
private:
	static String mkSalt();
};

class UsersConfig {
public:
	enum ModificationResult {
			ok,
			notEnoughMemory,
			userAlreadyExist,
			userDoesntExist,
			cannotRemoveAdmin
	};

	static String adminLogin;
	static String resultToMessage(ModificationResult result);
	
    ModificationResult addUser(UserEditRequest &userToAdd);
	ModificationResult addUser(UserConfig &user);
	ModificationResult editUser(UserEditRequest &userToEdit);
	ModificationResult removeUser(UserDeleteRequest &userToDelete);
	Vector<UserConfig> const getUsersList() const { return users; };
	UserConfig const * getUser(String login) const;
	bool addAdminIfDoesntExist();

private:
	int findUser(String login) const;
	static void forceActiveAdmin(UserConfig& cfg);

private:
	Vector<UserConfig> users;
};

template<> class Codec<UsersConfig> {
	public:
	static Codec<UsersConfig>& getInstance() {
		static Codec<UsersConfig> instance;
		return instance;
	}

	void encode(JsonObject& json, const UsersConfig &config) {
		JsonArray usersArr = json.createNestedArray("users");
		const Vector<UserConfig> users = config.getUsersList();
		for(int i=0; i<users.size(); i++) {
			auto user = users[i];
			JsonObject userObj = usersArr.createNestedObject();
			userObj["enabled"] = user.enabled;
			userObj["login"] = user.login;
			userObj["hash"] = user.hash;
			userObj["salt"] = user.salt;
			StringVectorCodec::encode(userObj, user.roles, "roles");
		}
	}

	Either<String, UsersConfig> decode(JsonObject& json) {
		UsersConfig cfg;
		JsonArray usersArr = json["users"].as<JsonArray>();
		int usersArrSize = usersArr.size();

		for(int i=0; i<usersArrSize; i++) {
			JsonObject userObj = usersArr[i].as<JsonObject>();
			UserConfig user(
				userObj["enabled"].as<bool>(),
				userObj["login"].as<String>(),
				userObj["salt"].as<String>(),
				userObj["hash"].as<String>(),
				StringVectorCodec::decode(userObj,  "roles")
			);

        	cfg.addUser(user);
		}

		cfg.addAdminIfDoesntExist();
		
		return {RightTagT(), std::move(cfg)};
	}
};