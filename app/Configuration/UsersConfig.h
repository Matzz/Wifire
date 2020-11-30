#pragma once

#include <SmingCore.h>

class UserConfig {
public:
    bool enabled;
	String login;
	String salt;
	String hash;
	Vector<String> roles;

    UserConfig(bool enabled, String login, String salt, String hash, const Vector<String> &roles);
    UserConfig(bool enabled, String login, String password, const Vector<String> &roles);
private:
	static String mkSalt();
};

class UsersConfig {
public:
	static String adminLogin;
	
    bool addUser(UserConfig &user);
	bool editUser(bool enabled, String login, String password, const Vector<String> &roles);
	bool removeUser(String login);
	const Vector<UserConfig> getUsersList();
	bool addAdminIfDoesntExist();

private:
	int findUser(String login);
	Vector<UserConfig> users;
};