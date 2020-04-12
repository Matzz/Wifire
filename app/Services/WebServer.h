#pragma once

#include <SmingCore.h>

class WebServer {
public:
	HttpServer server;
	int port;

	WebServer(int port = 80);
	void start();
	void bindActions();
};
