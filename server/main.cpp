#include <iostream>
#include "SERVER.h"
#include <string>
#include <time.h>

using namespace std;

int main() {
	server s;
	int port;

	s.winsock_start();

	cout << "PORT ��ȣ�� �Է��ϼ��� : ";
	cin >> port;
	s.port_setting(port);
	cout << "IP �ּ� �˻���.." << endl;
	Sleep(2000);
	cout << "IP �ּ� : " << s.get_IP() << endl;

	s.socket_setting();

	s.binding();

	s.listening();

	s.accepting();

	if (s.echo() == -1) {
		s.server_end();
	}
}