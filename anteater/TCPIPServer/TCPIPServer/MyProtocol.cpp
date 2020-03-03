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
소켓을 스레드 풀에 등록합니다.
*/
void MyProtocol::connectionPush(SOCKET * client, int id)
{
	connections.push_back(thread(connectionHandler, client, id));
	connections.back().detach();
}

/*
클라이언트로부터 받은 메세지를 분석해 보낼 메세지를 생성합니다.
recv_msg : 클라이언트로부터 받은 메세지
리턴값 : 클라이언트에게 보낼 메세지
*/
string MyProtocol::decodeMsg(string recv_msg)
{
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
string 표준 라이브러리는 만들어놨으면서 왜 split은 없지?
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