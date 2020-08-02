#pragma once

#include <SmingCore.h>

class LazyFtpServer {
protected:
	FtpServer ftp;
	String user;
	String password;
	int port;
public:
	LazyFtpServer(const String user, const String password, const int port);
	void start();
};
