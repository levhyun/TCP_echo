#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<string>
#include<winsock2.h>

using namespace std;

class client{
	public:
		WSADATA wsaData;
		// WSADATA : Windows ���� ������ ���� ���� ���� ����ü
		SOCKET client_socket;
		SOCKADDR_IN server_address;
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
		string sent;
		int recv_length;

		void port_setting(int port_num);
		void ShowErrorMessage(string message);
		void winsock_start();
		void socket_setting();
		void connecting();
		int echo();
		void client_end();
};

void client::port_setting(int port_num) { // port ���� �Լ�
	server_port = port_num;
}

void client::ShowErrorMessage(string message) { // ���� �޼��� �Լ�
	cout << "[�����߻�]: " << message << '\n';
	system("pause"); // ���α׷� �Ͻ� ����
	exit(1);
}

void client::winsock_start() {
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
}

void client::socket_setting() {
	client_socket = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ����
	/*
		socket([��� ����],[�������� Ÿ��],[�������� ��]);
		* ������� : ��ǻ�� ����/���̿��� �������� ��ȯ ����� �����ϴ� ��Ģ ü��
		- PF_INET : IPv4 ���ͳ� ��������
		- SOCK_STREAM : TCP/IP ���������� �̿�

		��ȯ
		- ���� : 0
		- ���� : -1, INVALID_SOCKET
	*/
	if (client_socket == INVALID_SOCKET) {
		ShowErrorMessage("socket()"); // ���� �޽��� �Լ� ȣ��
	}

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
	server_address.sin_addr.s_addr = inet_addr("192.168.35.212"); // ���ڿ�IP�� ��Ʈ��ũ ����Ʈ ��������
	server_address.sin_port = htons(server_port); // 2����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
}

void client::connecting(){
	if (connect(client_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR) { // ���� ��û
		/*
			connect([���� ����][�ּ� ����ü ������][�ּ� ����ü ũ��]);

			��ȯ
			- ���� : 0
			- ���� : -1, SOCKET_ERROR
		*/
		ShowErrorMessage("connect()");
	}
	cout << "�� ���� �Ϸ� ( " << inet_ntoa(server_address.sin_addr) << ":" << server_port << " ) ��" << endl;
}

int client::echo() {
	while (1) { 
		// �ݺ������� ������ �޽����� �����ϰ� ���ڸ޽����� ���޹޴´�.
		getline(cin, sent);

		if (sent == ""){
			continue;
		}

		send(client_socket, sent.c_str(), sent.length(), 0);
		/*
			send([���� ����],[���������� ����],[���� ������ ũ��],[�ɼ�]);
		*/
		recv_length = recv(client_socket, received, sizeof(received), 0);
		/*
			recv([���� ����],[���������� ����],[���� ������ ũ��],[�ɼ�]);

			��ȯ
			- ���� : ������ �������� ����
			- ���� : -1
		*/
		received[recv_length] = 'NULL';

		if (strcmp(received, "/exit") == 0){ // [strcmp] received ���ڿ��� "/exit"�� ��ġ -> 0 ��ȯ
			cout << "[��������]" << endl;
			break;
		}
		cout << "[�����޽���]: " << received << endl;
	}
	return -1;
}

void client::client_end(){ // ���α׷� ���� ���� �Լ�
	closesocket(client_socket); // ���� ����
	WSACleanup(); // winsock ��� ����
	system("pause"); // ���α׷� �Ͻ� ����
	exit(1); // ���α׷� ����
}