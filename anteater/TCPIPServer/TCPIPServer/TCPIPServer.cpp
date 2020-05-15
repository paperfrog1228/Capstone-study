// Ref1. https://www.binarytides.com/winsock-socket-programming-tutorial/
// Ref2. https://docs.microsoft.com/ko-kr/windows/win32/api/winsock2/
/*
게임서버가.. 어떻게 돌아가는지 잘 모르겠으니... 일단 Socket 통신 복습.
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include "MyHeader.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888

int TCPIPServer::run()
{
	printf("Hello World!\n");

	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	const char* message;

	printf("\n윈솤 초기화...\n");
	/*
		WSAStartup() : winsock 라이브러리 초기화 함수.
			첫번째 파라미터는 로드할 버전
			두번째 파라미터는 정보가 담길 WSADATA 구조체
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("실패. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("초기화됨.\n");

	/*
		socket() : 소켓 생성.
		AF_INET (address family) : IPv4 사용하겠다.
		SOCK_STREAM (type) : 연결지향형(스트림) 프로토콜 사용하겠다.
			0 (protocol) :
			If a value of 0 is specified, the caller does not wish to specify a protocol and the service provider will choose the protocol to use.
			0은 책임전가, 그 외에 IPPROTO_TCP, IPPROTO_UDP 등이 있음.
	*/
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("[INVALID == 유효하지 않은] : %d", WSAGetLastError());
	}

	printf("소켓 생성됨.\n");

	// sockaddr_in 구조체 정의
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	/*
		bind() : 소켓을 특정 주소와 포트에 고정시킨다.
			소켓은 첫번째 파라미터 s에, 주소/포트는 두번째 파라미터 sockaddr_in 구조체에.
			bind? 바인딩? 묶다? 고정시키다?
			bind -> listen -> accept
			By doing this we ensure that all incoming data which is directed towards this port number is received by this application.
			따라서 bind를 함으로써 이 어플리케이션이 포트를 점유하게 된다는듯.
	*/
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("bind 실패했다고함. : %d", WSAGetLastError());
	}

	printf("bind 완료.\n");

	/*
		listen() : 소켓을 listen 상태로 만듬.
			얘는 예외처리조차 없다.
			Ref1에서 설명해주지 않은 두번째 파라미터는 backlog. 연결 대기 큐의 최대 길이.
	*/
	listen(s, 3);

	printf("연결 대기중...\n");

	/*
		accept() : 연결을 받아 새 소켓을 리턴.
	*/
	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept 실패. : %d", WSAGetLastError());
	}

	printf("Connection accepted\n");

	/*
		client에게 메시지 전달
	*/
	message = "안녕하세요.\0";
	/*
		send() :
			new_socket에 message를 strlen(message)길이 만큼 보내겠다.
			마지막 파라미터는 플래그.
	*/
	send(new_socket, message, strlen(message), 0);

	getchar();

	closesocket(s);
	WSACleanup();
	return 0;
}