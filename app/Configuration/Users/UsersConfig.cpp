#include "UsersConfig.h"

#include <SmingCore.h>
#include <Crypto/Sha1.h>


UserConfig::UserConfig(): enabled(false), login(""), salt(""), hash(""), roles(Vector<String>(1, 1)) {
    
}
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
    this->roles = roles;
    setPassword(password);
}

bool UserConfig::checkPassword(String password) const {
    return UserConfig::getHash(salt + password) == hash;
}

String UserConfig::getHash(const String& base) {
		auto hash = Crypto::Sha1().calculate(base);
		return Crypto::toString(hash);
}

String UserConfig::mkSalt() {
    auto base = String(system_get_chip_id(), 10) + String(micros(), 10);
    return getHash(base);
}

String UsersConfig::adminLogin = "admin";

bool UsersConfig::addUser(UserConfig &user) {
    if(findUser(user.login) == -1) {
        return users.addElement(user);
    }
    return false;
}

bool UsersConfig::addUser(UserEditRequest &userToAdd) {
    if(findUser(userToAdd.login) == -1) {
        UserConfig user(userToAdd.enabled, userToAdd.login, userToAdd.password, userToAdd.roles);
        return users.addElement(user);
    }
    return false;
}

bool UsersConfig::editUser(UserEditRequest &userToEdit) {
    int idx = findUser(userToEdit.login);
    if(idx >= 0) {
        UserConfig &cfg = users[idx];
        cfg.enabled = userToEdit.enabled;
        if(userToEdit.password.length() > 0) {
            cfg.setPassword(userToEdit.password);
        }
        cfg.roles = userToEdit.roles;
        return true;
    }
    return false;
}

bool UsersConfig::removeUser(UserDeleteRequest &userToDelete) {
    if(userToDelete.login == UsersConfig::adminLogin) {
        return false;
    }
    int idx = findUser(userToDelete.login);
    if(idx>=0) {
        users.remove(idx);
        return true;
    }
    return false;
}

 const UserConfig* UsersConfig::getUser(String login) const {
    int idx = findUser(login);
    if(idx > -1) {
        return &(users[idx]);
    } else {
        return nullptr;
    }
}

bool UsersConfig::addAdminIfDoesntExist() {
    Vector<String> roles = Vector<String>(1);
    roles.addElement(UsersConfig::adminLogin);
    UserConfig user(true, UsersConfig::adminLogin, UsersConfig::adminLogin, roles);
    return addUser(user);
}

int UsersConfig::findUser(String login) const {
    for(int i=0; i<users.size(); i++) {
        if(users[i].login==login) {
            return i;
        }
    }
    return -1;
}