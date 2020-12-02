#include "UserSessionManager.h"
#include "../Utils/utils.h"

Session::Session(String login, String sessionId, Vector<String> roles):
    login(login), sessionId(sessionId), roles(roles) { }

UserSessionManager::UserSessionManager(UsersConfigProvider& configProvider) :
		configProvider(configProvider) { }

Either<String, Session> UserSessionManager::signin(String login, String password) {
    UsersConfig usersConfig = configProvider.load();
    auto maybeUser = usersConfig.getUser(login);
    UserConfig user = UserConfig();
    bool userFound = maybeUser.match(user);
    if(userFound) {
        if(!user.enabled) {
            return Left<String, Session>("This user is disabled.");
        }
        if(!user.checkPassword(password)) {
            return Left<String, Session>("Invalid password.");
        }
        Session session(login, mkSessionId(login), user.roles);
        return Right<String, Session>(session);
    } else {
        return Left<String, Session>("User not found.");
    }
}

String UserSessionManager::mkSessionId(String login) {
    auto base = String(system_get_chip_id(), 10) + String(os_get_nanoseconds(), 10) + String(login);
    return getHash(base);
}