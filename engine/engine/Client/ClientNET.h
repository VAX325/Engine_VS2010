#include "../NET.h"

class ClientNET
{
public:
	ClientNET(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
	ClientNET(unsigned int address, unsigned short port);
	~ClientNET();

	char* Send(const char data[]);
	int Recive();

private:
	Socket socket;
	Address serverAddress;
};