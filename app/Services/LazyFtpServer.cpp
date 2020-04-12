#include "LazyFtpServer.h"

LazyFtpServer::LazyFtpServer(String user, String password, int port) :
	port(port), user(user), password(password) {
}

void LazyFtpServer::start() {
	ftp.listen(port);
	ftp.addUser(user, password);
}
