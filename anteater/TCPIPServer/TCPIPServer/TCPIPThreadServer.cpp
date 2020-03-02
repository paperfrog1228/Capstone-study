// Ref1. https://modoocode.com/269
/*
������ ��� ����
*/

#include <io.h>
#include <stdio.h>
#include <WinSock2.h>
#include <thread> // ������ ǥ�� ���̺귯��. C++ 11�� �߰���.
#include <vector> // C++ ���� �ȸ������� �̷��� �ִ��� ��������
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
		send(new_socket, message.c_str(), message.size(), 0);

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
Ŭ���̾�Ʈ�κ��� ���� �޼����� �м��� ���� �޼����� �����մϴ�.
	recv_msg : Ŭ���̾�Ʈ�κ��� ���� �޼���
	���ϰ� : Ŭ���̾�Ʈ���� ���� �޼���
*/
string MyProtocol(string recv_msg) {
	vector<string> msg = split(recv_msg, '|');

	if (msg.size() != 2)
		return "wrong!";

	return "yes!";
}
/*
�����ϴ°Ŵϱ� �޼��� ������ ����..
seperator : |
structure : msg_type | msg
*/

/*
���ڿ��� Ư�� ���ڸ� �������� �и��մϴ�. Ref : https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
	s : ���ڿ�
	delimiter : Ư�� ����
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
string ǥ�� ���̺귯���� ���������鼭 �� split�� ����?
*/