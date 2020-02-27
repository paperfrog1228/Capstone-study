// Ref1. https://modoocode.com/269
/*
������ ��� ����
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include <thread> // ������ ǥ�� ���̺귯��. C++ 11�� �߰���.
#include <vector> // C++ ���� �ȸ������� �̷��� �ִ��� ��������
#include "MyHeader.h"

using std::thread;
using std::vector;

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888

void connection_handler(SOCKET* client, int client_id);

int TCPIPThreadServer::run()
{
	printf("Hello World!\n");

	WSADATA wsa;
	SOCKET s, new_socket, *hand_socket;
	struct sockaddr_in server, client;
	int c;
	int client_num = 0;
	vector<thread> connections;
	const char * message;

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

	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		printf("a new client requested a connection.\n");
		message = "assigning a handler...\n";
		send(new_socket, message, strlen(message), 0);

		hand_socket = (SOCKET *)malloc(sizeof(SOCKET));
		*hand_socket = new_socket;

		connections.push_back(thread(connection_handler, hand_socket, client_num++));
		connections.back().detach();
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

void connection_handler(SOCKET* client, int client_id) {
	thread::id thread_id = std::this_thread::get_id();
	const char * message;
	char response[2000];
	int recv_size;

	printf("Connection accepted. thread# %x, client_id#%d\n",thread_id, client_id);

	message = "�ȳ��ϼ���.\0";

	send(*client, message, strlen(message), 0);

	while (recv_size = recv(*client, response, 2000, 0) != SOCKET_ERROR)
	{
		/*
			���ȭ �� Cllient <=> Server �� �������� ���� ...?
		*/
		response[recv_size] = '\0';
		printf("client#%d: %s\n", client_id, response);

		send(*client, response, recv_size + 1, 0); // echo
	}

	closesocket(*client);
	free(client);
	printf("client#%d disconnected.\n", client_id);
}