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

template<>
void Codec<UsersConfig>::encode(JsonObject& json, const UsersConfig &config);

template<>
Either<String, UsersConfig> Codec<UsersConfig>::decode(JsonObject& json);