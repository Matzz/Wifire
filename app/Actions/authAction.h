#pragma once

#include <SmingCore.h>
#include "../Configuration/Users/UserSigninRequest.h"

void signInAction(HttpRequest &request, HttpResponse &response);
void signOutAction(HttpRequest &request, HttpResponse &response);