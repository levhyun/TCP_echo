#include <iostream>
#include "CLIENT.h"
#include <string>

using namespace std;

int main() {
	client c;
	int port;
	char ip[20];

	cout << "PORT ��ȣ�� �Է��ϼ��� : ";
	cin >> port;
	c.port_setting(port);

	c.winsock_start();

	cout << "IP �ּҸ� �Է��ϼ��� : ";
	cin >> ip;
	c.socket_setting(ip);

	c.connecting();

	if(c.echo() == -1){
		c.client_end();
	}
}