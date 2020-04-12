#pragma once

#include <SmingCore.h>

class LazyFtpServer {
protected:
	FtpServer ftp;
	String user;
	String password;
	int port;
public:
	LazyFtpServer(String user, String password, int port);
	void start();
};
