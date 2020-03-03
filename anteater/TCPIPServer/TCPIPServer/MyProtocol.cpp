#include "MyProtocol.h"

#include <sstream>

using std::vector;
using std::string;
using std::thread;

vector<thread> MyProtocol::connections = vector<thread>();

MyProtocol::MyProtocol()
{
}

/*
������ ������ Ǯ�� ����մϴ�.
*/
void MyProtocol::connectionPush(SOCKET * client, int id)
{
	connections.push_back(thread(connectionHandler, client, id));
	connections.back().detach();
}

/*
Ŭ���̾�Ʈ�κ��� ���� �޼����� �м��� ���� �޼����� �����մϴ�.
recv_msg : Ŭ���̾�Ʈ�κ��� ���� �޼���
���ϰ� : Ŭ���̾�Ʈ���� ���� �޼���
*/
string MyProtocol::decodeMsg(string recv_msg)
{
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
vector<string> MyProtocol::split(const string& s, char delimiter) {
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


void MyProtocol::connectionHandler(SOCKET * client, int id)
{
	thread::id threadId = std::this_thread::get_id();
	string sendMsg, recvMsg = "";
	char response[512];
	int recv_size;

	printf("Connection accepted. thread# %x, ID#%d\n", threadId, id);

	sendMsg = "Hello I'm your connection handler.\0";
	send(*client, sendMsg.c_str(), sendMsg.size(), 0);

	while ((recv_size = recv(*client, response, 512, 0)) != SOCKET_ERROR)
	{
		recvMsg.assign(response);
		printf("client#%d: %s (%d byte(s))\n", id, recvMsg.c_str(), recv_size);

		sendMsg = MyProtocol::decodeMsg(recvMsg);

		send(*client, sendMsg.c_str(), sendMsg.size(), 0);
	}

	closesocket(*client);
	free(client);
	printf("client#%d disconnected.\n", id);
}