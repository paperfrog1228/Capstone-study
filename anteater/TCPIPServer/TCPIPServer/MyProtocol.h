#pragma once

#include <WinSock2.h>
#include <string>
#include <thread>
#include <vector>

using std::vector;
using std::string;
using std::thread;

class MyProtocol
{
private:
	// fields
	static vector<thread> connections;

	// methods
	static string decodeMsg(string recv_msg);
	static vector<string> split(const string& s, char delimiter);
	static void connectionHandler(SOCKET* client, int id);

public:
	// constructor
	MyProtocol();

	// methods
	static void connectionPush(SOCKET* client, int id);
};

