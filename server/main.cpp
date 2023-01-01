#include <iostream>
#include "SERVER.h"
#include <string>
#include <time.h>

using namespace std;

int main() {
	server s;
	int port;

	s.winsock_start();

	cout << "PORT 번호를 입력하세요 : ";
	cin >> port;
	s.port_setting(port);
	cout << "IP 주소 검색중.." << endl;
	Sleep(2000);
	cout << "IP 주소 : " << s.get_IP() << endl;

	s.socket_setting();

	s.binding();

	s.listening();

	s.accepting();

	if (s.echo() == -1) {
		s.server_end();
	}
}