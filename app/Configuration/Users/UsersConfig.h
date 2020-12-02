#pragma once

#include <SmingCore.h>
#include "optional"
#include "../../Utils/Option.h"

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

	bool checkPassword(String password);

private:
	static String mkSalt();
};

class UsersConfig {
public:
	static String adminLogin;
	
    bool addUser(UserConfig &user);
	bool editUser(bool enabled, String login, String password, const Vector<String> &roles);
	bool removeUser(String login);
	const Vector<UserConfig> getUsersList() { return users; }
	const Option<UserConfig> getUser(String login);
	bool addAdminIfDoesntExist();

private:
	int findUser(String login);

private:
	Vector<UserConfig> users;
};