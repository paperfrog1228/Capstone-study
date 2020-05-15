// Ref1. https://www.binarytides.com/winsock-socket-programming-tutorial/
// Ref2. https://docs.microsoft.com/ko-kr/windows/win32/api/winsock2/
/*
반복문 사용 Live 서버
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include "MyHeader.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888

int TCPIPLiveServer::run()
{
	printf("Hello World!\n");

	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	const char* message;

	printf("\n윈솤 초기화...\n");

	// 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("실패. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("초기화됨.\n");

	// 소켓 생성
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("[INVALID == 유효하지 않은] : %d", WSAGetLastError());
		return 1;
	}

	printf("소켓 생성됨.\n");

	// sockaddr_in 구조체 정의
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("bind 실패했다고함. : %d", WSAGetLastError());
		return 1;
	}

	printf("bind 완료.\n");

	// listen
	listen(s, 3);

	printf("연결 대기중...\n");

	c = sizeof(struct sockaddr_in);
	// accept
	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		printf("Connection accepted\n");

		message = "안녕하세요.\0";
		send(new_socket, message, strlen(message), 0);
	}

	if (new_socket == INVALID_SOCKET)
	{
		printf("accept 실패. : %d", WSAGetLastError());
		return 1;
	}

	closesocket(s);
	WSACleanup();
	return 0;
}