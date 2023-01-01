#include <iostream>
#include "CLIENT.h"
#include <string>

using namespace std;

int main() {
	client c;
	int port;
	char ip[20];

	cout << "PORT 번호를 입력하세요 : ";
	cin >> port;
	c.port_setting(port);

	c.winsock_start();

	cout << "IP 주소를 입력하세요 : ";
	cin >> ip;
	c.socket_setting(ip);

	c.connecting();

	if(c.echo() == -1){
		c.client_end();
	}
}