#pragma once

#include <SmingCore.h>
#include "../Utils/NonCopyable.h"
#include "UserSessionManager.h"

class WebServer : private NonCopyable {
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
