// Ref1. https://www.binarytides.com/winsock-socket-programming-tutorial/
// Ref2. https://docs.microsoft.com/ko-kr/windows/win32/api/winsock2/
/*
���Ӽ�����.. ��� ���ư����� �� �𸣰�����... �ϴ� Socket ��� ����.
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

	printf("\n���� �ʱ�ȭ...\n");
	/*
		WSAStartup() : winsock ���̺귯�� �ʱ�ȭ �Լ�.
			ù��° �Ķ���ʹ� �ε��� ����
			�ι�° �Ķ���ʹ� ������ ��� WSADATA ����ü
	*/
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("����. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("�ʱ�ȭ��.\n");

	/*
		socket() : ���� ����.
		AF_INET (address family) : IPv4 ����ϰڴ�.
		SOCK_STREAM (type) : ����������(��Ʈ��) �������� ����ϰڴ�.
			0 (protocol) :
			If a value of 0 is specified, the caller does not wish to specify a protocol and the service provider will choose the protocol to use.
			0�� å������, �� �ܿ� IPPROTO_TCP, IPPROTO_UDP ���� ����.
	*/
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("[INVALID == ��ȿ���� ����] : %d", WSAGetLastError());
	}

	printf("���� ������.\n");

	// sockaddr_in ����ü ����
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	/*
		bind() : ������ Ư�� �ּҿ� ��Ʈ�� ������Ų��.
			������ ù��° �Ķ���� s��, �ּ�/��Ʈ�� �ι�° �Ķ���� sockaddr_in ����ü��.
			bind? ���ε�? ����? ������Ű��?
			bind -> listen -> accept
			By doing this we ensure that all incoming data which is directed towards this port number is received by this application.
			���� bind�� �����ν� �� ���ø����̼��� ��Ʈ�� �����ϰ� �ȴٴµ�.
	*/
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("bind �����ߴٰ���. : %d", WSAGetLastError());
	}

	printf("bind �Ϸ�.\n");

	/*
		listen() : ������ listen ���·� ����.
			��� ����ó������ ����.
			Ref1���� ���������� ���� �ι�° �Ķ���ʹ� backlog. ���� ��� ť�� �ִ� ����.
	*/
	listen(s, 3);

	printf("���� �����...\n");

	/*
		accept() : ������ �޾� �� ������ ����.
	*/
	c = sizeof(struct sockaddr_in);
	new_socket = accept(s, (struct sockaddr *)&client, &c);
	if (new_socket == INVALID_SOCKET)
	{
		printf("accept ����. : %d", WSAGetLastError());
	}

	printf("Connection accepted\n");

	/*
		client���� �޽��� ����
	*/
	message = "�ȳ��ϼ���.\0";
	/*
		send() :
			new_socket�� message�� strlen(message)���� ��ŭ �����ڴ�.
			������ �Ķ���ʹ� �÷���.
	*/
	send(new_socket, message, strlen(message), 0);

	getchar();

	closesocket(s);
	WSACleanup();
	return 0;
}