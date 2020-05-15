// Ref1. https://www.binarytides.com/winsock-socket-programming-tutorial/
// Ref2. https://docs.microsoft.com/ko-kr/windows/win32/api/winsock2/
/*
�ݺ��� ��� Live ����
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
	// accept
	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		printf("Connection accepted\n");

		message = "�ȳ��ϼ���.\0";
		send(new_socket, message, strlen(message), 0);
	}

	if (new_socket == INVALID_SOCKET)
	{
		printf("accept ����. : %d", WSAGetLastError());
		return 1;
	}

	closesocket(s);
	WSACleanup();
	return 0;
}