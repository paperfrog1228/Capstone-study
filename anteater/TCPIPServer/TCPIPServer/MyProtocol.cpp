#include "MyProtocol.h"

#include <sstream>

// 난수생성
#include <cstdlib>
#include <ctime>

using std::vector;
using std::string;
using std::thread;

vector<thread> MyProtocol::connections = vector<thread>();
vector<User> MyProtocol::userList = vector<User>();

MyProtocol::MyProtocol()
{
}

User userInfo;

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
recvMsg : 클라이언트로부터 받은 메세지
리턴값 : 클라이언트에게 보낼 메세지
*/
string MyProtocol::decodeMsg(string recvMsg)
{
	vector<string> msg = split(recvMsg, '|');
	string sendMsg;

	if (msg.size() == 0)
		sendMsg = "MSG|틀림!";
	else if (msg[0].compare("ATK") == 0)
		sendMsg = "MSG|당신은 " + attack() + "을(를) 공격했습니다.";
	else if (msg[0].compare("RFR") == 0)
		sendMsg = "";
	else if (msg[0].compare("LOGIN") == 0)
		sendMsg = msg[1];
	else
		sendMsg = "MSG|틀림!";

	return sendMsg;
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
	string  sendMsg, recvMsg = "";
	char response[512];
	int recvSize;
	User myInfo;

	myInfo.id = id;
	myInfo.socket = client;

	printf("Connection accepted. thread# %x, ID#%d\n", threadId, id);

	if (recvSize = recv(*client, response, 512, 0) != SOCKET_ERROR)
	{
		recvMsg.assign(response);
		myInfo.name = MyProtocol::decodeMsg(recvMsg);
		userList.push_back(myInfo);
		userInfo.id = myInfo.id;

		sendMsg = "Hello " + myInfo.name + ", I'm your connection handler.";
		send(*client, sendMsg.c_str(), sendMsg.size(), 0);
	}

	while ((recvSize = recv(*client, response, 512, 0)) != SOCKET_ERROR)
	{
		recvMsg.assign(response);
		printf("client#%d: %s (%d byte(s))\n", myInfo.id, recvMsg.c_str(), recvSize);

		sendMsg = MyProtocol::decodeMsg(recvMsg);

		if (sendMsg.compare("") != 0)
			send(*client, sendMsg.c_str(), sendMsg.size(), 0);
	}

	printf("client#%d: connection closed.\n", myInfo.id);

	removeUser(&myInfo);
	closesocket(*client);
	free(client);
	printf("client#%d disconnected.\n", id);
}

string MyProtocol::attack()
{
	int target = -1, damage;
	string sendMsg;

	srand((unsigned int)time(NULL));

	if (userList.size() > 0)
	{
		target = (int)rand() % userList.size();
		damage = 10 + (int)rand() % 11; // 10 ~ 20 사이의 난수
		sendMsg = "ATK|" + std::to_string(damage);
		send(*userList[target].socket, sendMsg.c_str(), sendMsg.size(), 0);

		printf("client#%d: attacked client#%d, dealt %d.\n", userInfo.id, userList[target].id, damage);

		return userList[target].name + "(#" + std::to_string(userList[target].id) + ")";
	}
	return "";
}

/*
userList에서 id를 가진 user를 찾아 삭제합니다.
*/
int MyProtocol::removeUser(User* delUser) {
	int index = -1;
	for (int i = 0; i < userList.size(); i++)
	{
		if (userList[i].id == delUser->id)
		{
			index = i;
			break;
		}
	}

	if (index != -1)
	{
		userList.erase(userList.begin() + index);
		printf("removed user #%d from users list.\n", delUser->id);
		return 0;
	}
	return -1;
}