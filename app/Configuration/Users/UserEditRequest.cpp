#include "UserEditRequest.h"

UserEditRequest::UserEditRequest(bool enabled, String login, String password, const Vector<String> &roles):
    enabled(enabled), login(login), password(password), roles(roles) {}