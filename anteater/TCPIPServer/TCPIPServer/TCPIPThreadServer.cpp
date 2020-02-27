// Ref1. https://modoocode.com/269
/*
스레드 사용 서버
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include <thread> // 스레드 표준 라이브러리. C++ 11에 추가됨.
#include <vector> // C++ 많이 안만져봐서 이런거 있는지 몰랐어유
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
		printf("accept 실패. : %d", WSAGetLastError());
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

	message = "안녕하세요.\0";

	send(*client, message, strlen(message), 0);

	while (recv_size = recv(*client, response, 2000, 0) != SOCKET_ERROR)
	{
		/*
			모듈화 후 Cllient <=> Server 간 프로토콜 구현 ...?
		*/
		response[recv_size] = '\0';
		printf("client#%d: %s\n", client_id, response);

		send(*client, response, recv_size + 1, 0); // echo
	}

	closesocket(*client);
	free(client);
	printf("client#%d disconnected.\n", client_id);
}