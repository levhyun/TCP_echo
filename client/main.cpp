#include <iostream>
#include "CLIENT.h"
#include <string>

using namespace std;

int main() {
	client c;
	int port;

	cout << "PORT ��ȣ�� �Է��ϼ��� : ";
	cin >> port;
	c.port_setting(port);

	c.winsock_start();

	c.socket_setting();

	c.connecting();

	if(c.echo() == -1){
		c.client_end();
	}
}