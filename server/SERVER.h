#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <stdlib.h>

using namespace std;

class server {
	public:
		WSADATA wsaData;
		// WSADATA : Windows ���� ������ ���� ���� ���� ����ü
		SOCKET server_socket, client_socket;
		SOCKADDR_IN server_address, client_address;
		/*
			SOCKADDR_IN : IPv4�� �ּ� ü�踦 ��Ÿ���� ����ü

			struct sockaddr_in{
				sa_family_t sin_family;		// �ּ� ü��
				uint16_t sin_port;			// 16��Ʈ TCP/UDP port
				struct in_addr sin_addr;	// 32��Ʈ IPv4 �ּ�
				char sin_zero[8];			// ������ ����
			}
		*/
		int server_port;
		char received[256];
		int recv_length;

		IN_ADDR ip_address = {0, };
		char localHostName[MAX_PATH];
		HOSTENT* ptr;

		void port_setting(int port_num);
		string get_IP();
		void ShowErrorMessage(string message);
		void winsock_start();
		void socket_setting();
		void binding();
		void listening();
		void accepting();
		int echo();
		void server_end();	
};

void server::port_setting(int port_num) { // port ���� �Լ�
	server_port = port_num;
	cout << "�� ��Ʈ ���� �Ϸ� ( PORT : " << port_num << " ) ��" << endl;
}

string server::get_IP() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	if (gethostname(localHostName, MAX_PATH) == SOCKET_ERROR) { // ���� ȣ��Ʈ �̸� �˻�0
		/*
			gethostname([���� ���� ���ڿ� ����],[���� ���� ���ڿ� ���� ũ��]);

			��ȯ
			- ���� : 0
			- ���� : -1, SOCKET_ERROR
		*/
		return inet_ntoa(ip_address);
	}

	ptr = gethostbyname(localHostName); // ȣ��Ʈ ���� �˻�

	while (ptr && ptr->h_name) {
		if (ptr->h_addrtype == PF_INET) {  // ȣ��Ʈ �ּ� Ÿ���� IPv4�϶� ��
			memcpy(&ip_address, ptr->h_addr_list[0], ptr->h_length); // �޸� ����
			break;
		}
		ptr++;
	}

	WSACleanup();
	return inet_ntoa(ip_address);
}

void server::ShowErrorMessage(string message) { // ���� �޼��� �Լ�
	cout << "[�����߻�]: " << message << '\n';
	system("pause"); // ���α׷� �Ͻ� ����
	exit(1);
}

void server::winsock_start() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Winsock �ʱ�ȭ
		/*
			WSAStartup : Winsock ���� ���� ���̺귯�� �ʱ�ȭ

			WSAStartup(
				_In_ WORD wVersionRequested,	// ���α׷��� �䱸�ϴ� �ֻ��� winsock ����
				_Out_ LPWSADATA lpWSAData		// WSADATA ������ ������ ���� ������
			);

			��ȯ
			- ���� : 0
			- ���� : ����
		*/
		ShowErrorMessage("WSAStartup()"); // ���� �޽��� �Լ� ȣ��
	}
	cout << "�� winsock �ʱ�ȭ �Ϸ� ��" << endl;
}

void server::socket_setting() {
	server_socket = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ����
	/*
		socket([��� ����],[�������� Ÿ��],[�������� ��]);
		* ������� : ��ǻ�� ����/���̿��� �������� ��ȯ ����� �����ϴ� ��Ģ ü��
		- PF_INET : IPv4 ���ͳ� ��������
		- SOCK_STREAM : TCP/IP ���������� �̿�

		��ȯ
		- ���� : 0
		- ���� : -1, INVALID_SOCKET
	*/
	if (server_socket == INVALID_SOCKET) {
		ShowErrorMessage("socket()"); // ���� �޽��� �Լ� ȣ��
	}
	cout << "�� ���� ���� �Ϸ� ��" << endl;

	memset(&server_address, 0, sizeof(server_address)); // server_address �޸� �ʱ�ȭ
	/*
		memset : �޸� �ʱ�ȭ

		void* memset(void* ptr, int value, size_t num);
		- void* ptr : �����ϰ��� �ϴ� �޸��� ���� �ּ�
		- int value : �޸𸮿� �����ϰ��� �ϴ� ��
		- size_t num : �޸��� ũ�� ������ ������ ����

		��ȯ
		- ���� : ptr
		- ���� : NULL
	*/
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // 4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	// * INADDR_ANY : ������ IP�ּҸ� �ڵ����� ã�Ƽ� �����ϴ� �Լ�
	// cout << &server_address.sin_addr << endl;
	// cout << inet_ntoa(server_address.sin_addr) << endl;
	server_address.sin_port = htons(server_port); // 2����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������

	cout << "�� ���� ���� �Ϸ� ��" << endl;
}

void server::binding() {
	if (bind(server_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR) { // ���Ͽ� �ּ� �Ҵ�
		/*
			bind([���� ����][�ּ� ����ü ������][�ּ� ����ü ũ��]);

			��ȯ
			- ���� : 0
			- ���� : -1, SOCKET_ERROR
		*/
		ShowErrorMessage("bind()"); // ���� �޽��� �Լ� ȣ��
	}

	cout << "�� bind �Ϸ� ��" << endl;
}

void server::listening() {
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) { // �����û ������ ����ϴ� ���� ��⿭�� ����
		/*
			listen([���� ����],[���� ��⿭�� ũ��]);
			* SOMAXCONN : ���� ���� �����ڰ� �˾Ƽ� ���� ��⿭�� ũ�⸦ �����Ѵ�.

			��ȯ
			- ���� : 0
			- ���� : -1, SOCKET_ERROR
		*/
		ShowErrorMessage("listen()");
	}

	cout << "�� listen ��.. ��" << endl;
}

void server::accepting() {
	int sizeClientAddress = sizeof(client_address);
	client_socket = accept(server_socket, (SOCKADDR*)&client_address, &sizeClientAddress); // Ŭ���̾�Ʈ ���� ��û ����
	/*
		accept([���� ����],[�ּ� ����ü ������],[�ּ� ����ü ũ��]);

		��ȯ
		- ���� : 0
		- ���� : -1, INVALID_SOCKET
	*/
	if (client_socket == INVALID_SOCKET) {
		ShowErrorMessage("accept()"); // ���� �޽��� �Լ� ȣ��
	}

	cout << "�� listen �Ϸ� ��" << endl;
	cout << "�� ���� ��û ���� ��" << endl;
	cout << "�� accept �Ϸ� ��" << endl;
}

int server::echo() {
	while (1) { // Ŭ���̾�Ʈ�� �޽����� �޾Ƽ� �״�� �ٽ� ����
		recv_length = recv(client_socket, received, sizeof(received), 0);
		/*
			recv([���� ����],[���������� ����],[���� ������ ũ��],[�ɼ�]);

			��ȯ
			- ���� : ������ �������� ����
			- ���� : -1
		*/
		received[recv_length] = NULL;

		cout << "[Ŭ���̾�Ʈ�޽���]: " << received << endl;
		cout << "[�޽�������]: " << received << endl;

		if (strcmp(received, "/exit") == 0) { // [strcmp] received ���ڿ��� "/exit"�� ��ġ -> 0 ��ȯ
			send(client_socket, received, sizeof(received) - 1, 0);
			/*
				send([���� ����],[���������� ����],[���� ������ ũ��],[�ɼ�]);

				��ȯ
				- ���� : ���۵� ������ ��
				- ���� : -1
			*/
			cout << "[��������]" << endl;
			break;
		}
		send(client_socket, received, sizeof(received) - 1, 0);
	}
	return -1;
}

void server::server_end() { // ���α׷� ���� ���� �Լ�
	closesocket(client_socket); // ���� ����
	closesocket(server_socket); // ���� ����
	WSACleanup(); // winsock ��� ����
	system("pause"); // ���α׷� �Ͻ� ����
	exit(1); // ���α׷� ����
}