#ifndef APP_SERVICES_FTPSERVER_H_
#define APP_SERVICES_FTPSERVER_H_

#include <SmingCore/SmingCore.h>

class FtpServer {
protected:
	FTPServer ftp;
	String user;
	String password;
	int port;
public:
	FtpServer(String user, String password, int port);
	void start();
};

#endif
