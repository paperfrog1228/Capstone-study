// Ref1. https://modoocode.com/269
/*
스레드 사용 서버
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include <vector> // C++ 많이 안만져봐서 이런거 있는지 몰랐어유
#include <string>
#include "MyHeader.h"
#include "MyProtocol.h"

using std::vector;
using std::string;

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888

int TCPIPThreadServer::run()
{
	printf("Hello World!\n");

	WSADATA wsa;
	SOCKET s, newSocket, *handSocket;
	struct sockaddr_in server, client;
	int c;
	int clientNum = 0;
	string message;

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

	while ((newSocket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		printf("a new client requested a connection.\n");
		message = "MSG|assigning a handler ... ";
		send(newSocket, message.c_str(), message.size(), 0);

		handSocket = (SOCKET *)malloc(sizeof(SOCKET));
		*handSocket = newSocket;

		MyProtocol::connectionPush(handSocket, clientNum++);
	}

	if (newSocket == INVALID_SOCKET)
	{
		printf("accept 실패. : %d", WSAGetLastError());
		return 1;
	}

	closesocket(s);
	WSACleanup();
	return 0;
}