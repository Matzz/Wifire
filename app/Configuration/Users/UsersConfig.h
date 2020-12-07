#pragma once

#include <SmingCore.h>

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

private:
	static String mkSalt();
};

class UsersConfig {
public:
	static String adminLogin;
	
    bool addUser(UserConfig &user);
	bool editUser(bool enabled, String login, String password, const Vector<String> &roles);
	bool removeUser(String login);
	Vector<UserConfig> const getUsersList() { return users; } const
	UserConfig* getUser(String login) const;
	bool addAdminIfDoesntExist();

private:
	int findUser(String login) const;

private:
	Vector<UserConfig> users;
};