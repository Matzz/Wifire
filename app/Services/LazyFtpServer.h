#pragma once

#include <SmingCore.h>
#include "../Utils/NonCopyable.h"

class LazyFtpServer : private NonCopyable {
protected:
	FtpServer ftp;
	String user;
	String password;
	int port;
public:
	LazyFtpServer(const String user, const String password, const int port);
	void start();
};
