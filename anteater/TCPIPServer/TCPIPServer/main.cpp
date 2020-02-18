#include "MyHeader.h"

int main()
{
	IRunnable* server = new TCPIPLiveServer();
	server->run();
	return 0;
}