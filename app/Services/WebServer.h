#ifndef APP_SERVICES_WEBSERVER_H_
#define APP_SERVICES_WEBSERVER_H_

#include <SmingCore/SmingCore.h>

class WebServer {
public:
	HttpServer server;
	int port;

	WebServer(int port = 80);
	void start();
	void bindActions();
};

#endif
