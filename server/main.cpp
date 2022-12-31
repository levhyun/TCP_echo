#include <iostream>
#include "SERVER.h"
#include <string>

using namespace std;

int main() {
	server s;
	int port;

	cout << "PORT 번호를 입력하세요 : ";
	cin >> port;
	s.port_setting(port);
	cout << "IP : 192.168.35.212" << endl;

	s.winsock_start();

	s.socket_setting();

	s.binding();

	s.listening();

	s.accepting();

	if (s.echo() == -1) {
		s.server_end();
	}
}