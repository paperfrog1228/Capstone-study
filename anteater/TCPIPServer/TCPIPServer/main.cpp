#include "MyHeader.h"

int main()
{
	IRunnable* server = new TCPIPThreadServer();
	server->run();
	return 0;
}