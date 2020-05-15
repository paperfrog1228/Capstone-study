// Ref1. https://modoocode.com/269
/*
������ ��� ����
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include <vector> // C++ ���� �ȸ������� �̷��� �ִ��� ��������
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

	printf("\n���� �ʱ�ȭ...\n");

	// �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("����. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("�ʱ�ȭ��.\n");

	// ���� ����
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("[INVALID == ��ȿ���� ����] : %d", WSAGetLastError());
		return 1;
	}

	printf("���� ������.\n");

	// sockaddr_in ����ü ����
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("bind �����ߴٰ���. : %d", WSAGetLastError());
		return 1;
	}

	printf("bind �Ϸ�.\n");

	// listen
	listen(s, 3);

	printf("���� �����...\n");

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
		printf("accept ����. : %d", WSAGetLastError());
		return 1;
	}

	closesocket(s);
	WSACleanup();
	return 0;
}