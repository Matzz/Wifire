#pragma once

#include <SmingCore.h>
#include "../Codec.h"
#include "UserEditRequest.h"
#include "UserDeleteRequest.h"

#define MAX_USERS 100
#define MAX_ROLES 10

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
	static String adminLogin;
	
    bool addUser(UserEditRequest &userToAdd);
	bool addUser(UserConfig &user);
	bool editUser(UserEditRequest &userToEdit);
	bool removeUser(UserDeleteRequest &userToDelete);
	Vector<UserConfig> const getUsersList() { return users; } const
	UserConfig* getUser(String login) const;
	bool addAdminIfDoesntExist();

private:
	int findUser(String login) const;

private:
	Vector<UserConfig> users;
};

template<> class Codec<UsersConfig> {
	public:
        static Codec<UsersConfig>& getInstance() {
            static Codec<UsersConfig> instance;
            return instance;
        }

	void encode(JsonObject& json, UsersConfig config) {
		JsonArray usersArr = json.createNestedArray("users");
		const Vector<UserConfig> users = config.getUsersList();
		for(int i=0; i<users.size(); i++) {
			auto user = users[i];
			JsonObject userObj = usersArr.createNestedObject();
			userObj["enabled"] = user.enabled;
			userObj["login"] = user.login;
			userObj["hash"] = user.hash;
			userObj["salt"] = user.salt;
			JsonArray rolesArr = userObj.createNestedArray("roles");
			for(int j=0; j<user.roles.size(); j++) {
				rolesArr.add(user.roles[j]);
			}
		}
	}

	Either<String, UsersConfig> decode(JsonObject& json) {
		UsersConfig cfg;
		JsonArray usersArr = json["users"].as<JsonArray>();
		int usersArrSize = usersArr.size();

		if(usersArrSize>MAX_USERS) {
			debug_e("Too many users! Loading only first %d.", MAX_USERS);
			usersArrSize = MAX_USERS;
		}
		for(int i=0; i<usersArrSize; i++) {
			JsonObject userObj = usersArr[i].as<JsonObject>();
			JsonArray rolesArr = userObj["roles"].as<JsonArray>();
			unsigned int rolesArrSize = rolesArr.size();
			if(rolesArrSize>MAX_ROLES) {
				debug_e("Too many roles! Loading only first %d.", MAX_ROLES);
				rolesArrSize = MAX_ROLES;
			}
			Vector<String> roles(rolesArrSize,  10);
			for(int j=0; j<rolesArrSize; j++) {
				String role = rolesArr[j].as<String>();
				if(role.length() > 0) {
					roles.addElement(role);
				}
			}
			UserConfig user(
				userObj["enabled"].as<bool>(),
				userObj["login"].as<String>(),
				userObj["salt"].as<String>(),
				userObj["hash"].as<String>(),
				roles
			);

        	cfg.addUser(user);
		}

		cfg.addAdminIfDoesntExist();
		
		return {right_tag_t(), std::move(cfg)};
	}
};