#include "UsersConfig.h"

#include <SmingCore.h>
#include "../../Utils/utils.h"

UserConfig::UserConfig(bool enabled, String login, String salt, String hash, const Vector<String> &roles) {
    this->enabled = enabled;
    this->login = login;
    this->salt = salt;
    this->hash = hash;
    this->roles = roles;
}

UserConfig::UserConfig(bool enabled, String login, String password, const Vector<String> &roles) {
    String salt = mkSalt();
    this->enabled = enabled;
    this->login = login;
    this->salt = salt;
    this->hash = getHash(salt + password);
    this->roles = roles;
}

String UsersConfig::adminLogin = "admin";

String UserConfig::mkSalt() {
    auto base = String(system_get_chip_id(), 10) + String(os_get_nanoseconds(), 10);
    return getHash(base);
}

bool UsersConfig::addUser(UserConfig &user) {
    if(findUser(user.login) == -1) {
        return users.addElement(user);
    }
    return false;
}

bool UsersConfig::editUser(bool enabled, String login, String password, const Vector<String> &roles) {
    int idx = findUser(login);
    if(idx >= 0) {
        UserConfig &cfg = users[idx];
        cfg.enabled = enabled;
        if(password.length() > 0) {
            cfg.hash = getHash(cfg.salt + password);
        }
        cfg.roles = roles;
        return true;
    }
    return false;
}

bool UsersConfig::removeUser(String login) {
    if(login == UsersConfig::adminLogin) {
        return false;
    }
    int idx = findUser(login);
    if(idx>=0) {
        users.remove(idx);
        return true;
    }
    return false;
}

const Vector<UserConfig> UsersConfig::getUsersList() {
    return users;
}

bool UsersConfig::addAdminIfDoesntExist() {
    Vector<String> roles = Vector<String>(1);
    roles.addElement(UsersConfig::adminLogin);
    UserConfig user(true, UsersConfig::adminLogin, UsersConfig::adminLogin, roles);
    return addUser(user);
}

int UsersConfig::findUser(String login) {
    for(int i=0; i<users.size(); i++) {
        if(users[i].login==login) {
            return i;
        }
    }
    return -1;
}