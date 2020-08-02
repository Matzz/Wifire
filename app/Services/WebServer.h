#pragma once

#include <SmingCore.h>

class WebServer {
public:
	HttpServer server;
	int port;

	WebServer(const int port);
	void start();
	void bindActions();
};
