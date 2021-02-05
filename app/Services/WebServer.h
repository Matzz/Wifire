#pragma once

#include <SmingCore.h>
#include "UserSessionManager.h"

class WebServer {
public:

	WebServer(const int port, UserSessionManager& sessionManager);
	void start();
	void bindActions();

private:
	HttpServer server;
	int port;
	UserSessionManager& sessionManager;
	void authWrapper(const String& path, const String& requiredRole, const HttpPathDelegate& callback);
};
