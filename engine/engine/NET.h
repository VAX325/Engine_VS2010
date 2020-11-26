class Address
{
public:

    Address();
    Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
    Address(unsigned int address, unsigned short port);
    unsigned int GetAddress() const;
    unsigned char GetA() const;
    unsigned char GetB() const;
    unsigned char GetC() const;
    unsigned char GetD() const;
    unsigned short GetPort() const;
    bool operator == (const Address& other) const;
    bool operator != (const Address& other) const;

private:

    unsigned char aA;
    unsigned char bA;
    unsigned char cA;
    unsigned char dA;

    unsigned int addressA;
    unsigned short portA;
};

class Socket
{
public:

    Socket();
    ~Socket();
    bool Open(unsigned short port);
    void Close();
    bool IsOpen() const;
    bool Send(const Address& destination, const void* data, int size);
    int Receive(Address& sender, void* data, int size);

private:
    bool isOpen = false;
    int handle;
};