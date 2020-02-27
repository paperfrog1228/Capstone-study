#pragma once

class IRunnable
{
public:
	virtual int run() = 0;
};

class TCPIPServer : public IRunnable
{
public:
	int run();
};

class TCPIPLiveServer : public IRunnable
{
public:
	int run();
};

class TCPIPThreadServer : public IRunnable
{
public:
	int run();
};