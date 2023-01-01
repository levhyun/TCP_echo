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
		// WSADATA : Windows 소켓 구현에 대한 정보 포함 구조체
		SOCKET server_socket, client_socket;
		SOCKADDR_IN server_address, client_address;
		/*
			SOCKADDR_IN : IPv4의 주소 체계를 나타내는 구조체

			struct sockaddr_in{
				sa_family_t sin_family;		// 주소 체계
				uint16_t sin_port;			// 16비트 TCP/UDP port
				struct in_addr sin_addr;	// 32비트 IPv4 주소
				char sin_zero[8];			// 사용되지 않음
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

void server::port_setting(int port_num) { // port 설정 함수
	server_port = port_num;
	cout << "※ 포트 설정 완료 ( PORT : " << port_num << " ) ※" << endl;
}

string server::get_IP() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	if (gethostname(localHostName, MAX_PATH) == SOCKET_ERROR) { // 로컬 호스트 이름 검색0
		/*
			gethostname([값을 받을 문자열 변수],[값을 받을 문자열 변수 크기]);

			반환
			- 성공 : 0
			- 실패 : -1, SOCKET_ERROR
		*/
		return inet_ntoa(ip_address);
	}

	ptr = gethostbyname(localHostName); // 호스트 정보 검색

	while (ptr && ptr->h_name) {
		if (ptr->h_addrtype == PF_INET) {  // 호스트 주소 타입이 IPv4일때 참
			memcpy(&ip_address, ptr->h_addr_list[0], ptr->h_length); // 메모리 복사
			break;
		}
		ptr++;
	}

	WSACleanup();
	return inet_ntoa(ip_address);
}

void server::ShowErrorMessage(string message) { // 오류 메세지 함수
	cout << "[오류발생]: " << message << '\n';
	system("pause"); // 프로그램 일시 정지
	exit(1);
}

void server::winsock_start() {
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { // Winsock 초기화
		/*
			WSAStartup : Winsock 동적 연결 라이브러르 초기화

			WSAStartup(
				_In_ WORD wVersionRequested,	// 프로그램이 요구하는 최상위 winsock 버전
				_Out_ LPWSADATA lpWSAData		// WSADATA 데이터 구조에 대한 포인터
			);

			반환
			- 성공 : 0
			- 실패 : 에러
		*/
		ShowErrorMessage("WSAStartup()"); // 오류 메시지 함수 호출
	}
	cout << "※ winsock 초기화 완료 ※" << endl;
}

void server::socket_setting() {
	server_socket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성
	/*
		socket([통신 영역],[프로토콜 타입],[프로토콜 값]);
		* 프로토롤 : 컴퓨터 내부/사이에서 데이터의 교환 방식을 정의하는 규칙 체계
		- PF_INET : IPv4 인터넷 프로토콜
		- SOCK_STREAM : TCP/IP 프로토콜을 이용

		반환
		- 성공 : 0
		- 실패 : -1, INVALID_SOCKET
	*/
	if (server_socket == INVALID_SOCKET) {
		ShowErrorMessage("socket()"); // 오류 메시지 함수 호출
	}
	cout << "※ 소켓 생성 완료 ※" << endl;

	memset(&server_address, 0, sizeof(server_address)); // server_address 메모리 초기화
	/*
		memset : 메모리 초기화

		void* memset(void* ptr, int value, size_t num);
		- void* ptr : 세팅하고자 하는 메모리의 시작 주소
		- int value : 메모리에 세팅하고자 하는 값
		- size_t num : 메모리의 크기 한조각 단위의 길이

		반환
		- 성공 : ptr
		- 실패 : NULL
	*/
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // 4바이트 정수를 네트워크 바이트 형식으로
	// * INADDR_ANY : 서버의 IP주소를 자동으로 찾아서 대입하는 함수
	// cout << &server_address.sin_addr << endl;
	// cout << inet_ntoa(server_address.sin_addr) << endl;
	server_address.sin_port = htons(server_port); // 2바이트 정수를 네트워크 바이트 형식으로

	cout << "※ 소켓 설정 완료 ※" << endl;
}

void server::binding() {
	if (bind(server_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR) { // 소켓에 주소 할당
		/*
			bind([소켓 변수][주소 구조체 포인터][주소 구조체 크기]);

			반환
			- 성공 : 0
			- 실패 : -1, SOCKET_ERROR
		*/
		ShowErrorMessage("bind()"); // 오류 메시지 함수 호출
	}

	cout << "※ bind 완료 ※" << endl;
}

void server::listening() {
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) { // 연결요청 소켓이 대기하는 연결 대기열을 생성
		/*
			listen([소켓 변수],[연결 대기열의 크기]);
			* SOMAXCONN : 소켓 서비스 제공자가 알아서 연결 대기열의 크기를 설정한다.

			반환
			- 성공 : 0
			- 실패 : -1, SOCKET_ERROR
		*/
		ShowErrorMessage("listen()");
	}

	cout << "※ listen 중.. ※" << endl;
}

void server::accepting() {
	int sizeClientAddress = sizeof(client_address);
	client_socket = accept(server_socket, (SOCKADDR*)&client_address, &sizeClientAddress); // 클라이언트 연결 요청 수락
	/*
		accept([소켓 변수],[주소 구조체 포인터],[주소 구조체 크기]);

		반환
		- 성공 : 0
		- 실패 : -1, INVALID_SOCKET
	*/
	if (client_socket == INVALID_SOCKET) {
		ShowErrorMessage("accept()"); // 오류 메시지 함수 호출
	}

	cout << "※ listen 완료 ※" << endl;
	cout << "※ 연결 요청 수락 ※" << endl;
	cout << "※ accept 완료 ※" << endl;
}

int server::echo() {
	while (1) { // 클라이언트의 메시지를 받아서 그대로 다시 전달
		recv_length = recv(client_socket, received, sizeof(received), 0);
		/*
			recv([소켓 변수],[수신저장할 변수],[읽을 데이터 크기],[옵션]);

			반환
			- 성공 : 수신한 데이터의 길이
			- 실패 : -1
		*/
		received[recv_length] = NULL;

		cout << "[클라이언트메시지]: " << received << endl;
		cout << "[메시지전송]: " << received << endl;

		if (strcmp(received, "/exit") == 0) { // [strcmp] received 문자열과 "/exit"이 일치 -> 0 반환
			send(client_socket, received, sizeof(received) - 1, 0);
			/*
				send([소켓 변수],[수신저장할 변수],[읽을 데이터 크기],[옵션]);

				반환
				- 성공 : 전송된 데이터 량
				- 실패 : -1
			*/
			cout << "[서버종료]" << endl;
			break;
		}
		send(client_socket, received, sizeof(received) - 1, 0);
	}
	return -1;
}

void server::server_end() { // 프로그램 실행 종료 함수
	closesocket(client_socket); // 소켓 종료
	closesocket(server_socket); // 소켓 종료
	WSACleanup(); // winsock 사용 종료
	system("pause"); // 프로그램 일시 정지
	exit(1); // 프로그램 종료
}