#include "MyProtocol.h"

#include <sstream>

// ��������
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
������ ������ Ǯ�� ����մϴ�.
*/
void MyProtocol::connectionPush(SOCKET * client, int id)
{
	connections.push_back(thread(connectionHandler, client, id));
	connections.back().detach();
}

/*
Ŭ���̾�Ʈ�κ��� ���� �޼����� �м��� ���� �޼����� �����մϴ�.
recvMsg : Ŭ���̾�Ʈ�κ��� ���� �޼���
���ϰ� : Ŭ���̾�Ʈ���� ���� �޼���
*/
string MyProtocol::decodeMsg(string recvMsg)
{
	vector<string> msg = split(recvMsg, '|');
	string sendMsg;

	if (msg.size() == 0)
		sendMsg = "MSG|Ʋ��!";
	else if (msg[0].compare("ATK") == 0)
		sendMsg = "MSG|����� " + attack() + "��(��) �����߽��ϴ�.";
	else if (msg[0].compare("RFR") == 0)
		sendMsg = "";
	else if (msg[0].compare("LOGIN") == 0)
		sendMsg = msg[1];
	else
		sendMsg = "MSG|Ʋ��!";

	return sendMsg;
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
		damage = 10 + (int)rand() % 11; // 10 ~ 20 ������ ����
		sendMsg = "ATK|" + std::to_string(damage);
		send(*userList[target].socket, sendMsg.c_str(), sendMsg.size(), 0);

		printf("client#%d: attacked client#%d, dealt %d.\n", userInfo.id, userList[target].id, damage);

		return userList[target].name + "(#" + std::to_string(userList[target].id) + ")";
	}
	return "";
}

/*
userList���� id�� ���� user�� ã�� �����մϴ�.
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