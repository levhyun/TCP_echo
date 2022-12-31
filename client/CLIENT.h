#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<iostream>
#include<string>
#include<winsock2.h>

using namespace std;

class client{
	public:
		WSADATA wsaData;
		// WSADATA : Windows 소켓 구현에 대한 정보 포함 구조체
		SOCKET client_socket;
		SOCKADDR_IN server_address;
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

void client::port_setting(int port_num) { // port 설정 함수
	server_port = port_num;
}

void client::ShowErrorMessage(string message) { // 오류 메세지 함수
	cout << "[오류발생]: " << message << '\n';
	system("pause"); // 프로그램 일시 정지
	exit(1);
}

void client::winsock_start() {
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
}

void client::socket_setting() {
	client_socket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성
	/*
		socket([통신 영역],[프로토콜 타입],[프로토콜 값]);
		* 프로토롤 : 컴퓨터 내부/사이에서 데이터의 교환 방식을 정의하는 규칙 체계
		- PF_INET : IPv4 인터넷 프로토콜
		- SOCK_STREAM : TCP/IP 프로토콜을 이용

		반환
		- 성공 : 0
		- 실패 : -1, INVALID_SOCKET
	*/
	if (client_socket == INVALID_SOCKET) {
		ShowErrorMessage("socket()"); // 오류 메시지 함수 호출
	}

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
	server_address.sin_addr.s_addr = inet_addr("192.168.35.212"); // 문자열IP를 네트워크 바이트 형식으로
	server_address.sin_port = htons(server_port); // 2바이트 정수를 네트워크 바이트 형식으로
}

void client::connecting(){
	if (connect(client_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR) { // 연결 요청
		/*
			connect([소켓 변수][주소 구조체 포인터][주소 구조체 크기]);

			반환
			- 성공 : 0
			- 실패 : -1, SOCKET_ERROR
		*/
		ShowErrorMessage("connect()");
	}
	cout << "※ 연결 완료 ( " << inet_ntoa(server_address.sin_addr) << ":" << server_port << " ) ※" << endl;
}

int client::echo() {
	while (1) { 
		// 반복적으로 서버로 메시지를 전송하고 에코메시지를 전달받는다.
		getline(cin, sent);

		if (sent == ""){
			continue;
		}

		send(client_socket, sent.c_str(), sent.length(), 0);
		/*
			send([소켓 변수],[수신저장할 변수],[읽을 데이터 크기],[옵션]);
		*/
		recv_length = recv(client_socket, received, sizeof(received), 0);
		/*
			recv([소켓 변수],[수신저장할 변수],[읽을 데이터 크기],[옵션]);

			반환
			- 성공 : 수신한 데이터의 길이
			- 실패 : -1
		*/
		received[recv_length] = 'NULL';

		if (strcmp(received, "/exit") == 0){ // [strcmp] received 문자열과 "/exit"이 일치 -> 0 반환
			cout << "[서버종료]" << endl;
			break;
		}
		cout << "[서버메시지]: " << received << endl;
	}
	return -1;
}

void client::client_end(){ // 프로그램 실행 종료 함수
	closesocket(client_socket); // 소켓 종료
	WSACleanup(); // winsock 사용 종료
	system("pause"); // 프로그램 일시 정지
	exit(1); // 프로그램 종료
}