// Ref1. https://modoocode.com/269
/*
스레드 사용 서버
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include <thread> // 스레드 표준 라이브러리. C++ 11에 추가됨.
#include <vector> // C++ 많이 안만져봐서 이런거 있는지 몰랐어유
#include <string>
#include <sstream>
#include "MyHeader.h"

using std::thread;
using std::vector;
using std::string;

#pragma comment(lib, "ws2_32.lib")

#define PORT 8888

void connection_handler(SOCKET* client, int client_id);

string MyProtocol(string recv_msg);

vector<string> split(const string & s, char delimiter);

int TCPIPThreadServer::run()
{
	printf("Hello World!\n");

	WSADATA wsa;
	SOCKET s, new_socket, *hand_socket;
	struct sockaddr_in server, client;
	int c;
	int client_num = 0;
	vector<thread> connections;
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

	while ((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
	{
		printf("a new client requested a connection.\n");
		message = "assigning a handler...\n";
		send(new_socket, message.c_str(), message.size(), 0);

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
	string send_msg, recv_msg = "";
	char response[512];
	int recv_size;

	printf("Connection accepted. thread# %x, client_id#%d\n",thread_id, client_id);

	send_msg = "Hello I'm your connection handler.\0";
	send(*client, send_msg.c_str(), send_msg.size(), 0);

	while ((recv_size = recv(*client, response, 512, 0)) != SOCKET_ERROR)
	{
		recv_msg.assign(response);
		printf("client#%d: %s (%d byte(s))\n", client_id, recv_msg.c_str(), recv_size);

		send_msg = MyProtocol(recv_msg);

		send(*client, send_msg.c_str(), send_msg.size(), 0);
	}

	closesocket(*client);
	free(client);
	printf("client#%d disconnected.\n", client_id);
}

/*
클라이언트로부터 받은 메세지를 분석해 보낼 메세지를 생성합니다.
	recv_msg : 클라이언트로부터 받은 메세지
	리턴값 : 클라이언트에게 보낼 메세지
*/
string MyProtocol(string recv_msg) {
	vector<string> msg = split(recv_msg, '|');

	if (msg.size() != 2)
		return "wrong!";

	return "yes!";
}
/*
연습하는거니까 메세지 구조는 대충..
seperator : |
structure : msg_type | msg
*/

/*
문자열을 특정 문자를 기준으로 분리합니다. Ref : https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
	s : 문자열
	delimiter : 특정 문자
*/
vector<string> split(const string& s, char delimiter) {
	vector<string> tokens;
	string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}
/*
string 표준 라이브러리는 만들어놨으면서 왜 split은 없지?
*/