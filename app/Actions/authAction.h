#pragma once

#include <SmingCore.h>

void signInAction(HttpRequest &request, HttpResponse &response);
void signOutAction(HttpRequest &request, HttpResponse &response);

void setSessionId(HttpResponse &response, const String& sessionId);

const String getSessionId(HttpRequest& request);