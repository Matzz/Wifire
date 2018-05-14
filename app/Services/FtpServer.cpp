#include "FtpServer.h"

FtpServer::FtpServer(String user, String password, int port) :
	port(port), user(user), password(password) {
}

void FtpServer::start() {
	ftp.listen(port);
	ftp.addUser(user, password);
}
