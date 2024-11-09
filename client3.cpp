#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>


#pragma comment(lib,"ws2_32.lib")
using namespace std;


//intilize
bool initilize()
{
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

int main()
{

	if (!initilize())
	{
		cout << "initilize winssock failed" << endl;

		return 1;
	}
	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		cout << "invalid socket is created" << endl;
		return 1;
	}


	string serveraddress = "127.0.0.1";
	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

	if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
	{
		cout << "not able to connect to server" << endl;
		cout << ": " << WSAGetLastError();
		closesocket(s);
		WSACleanup();
		return 1;
	}

	cout << "successfully connected to server" << endl;

	//send and receive
	string message = "hello there!";
	int bytesent;
	bytesent = send(s, message.c_str(), message.length(), 0);

	if (bytesent == SOCKET_ERROR)
	{
		cout << "sent is failed" << endl;

	}
	closesocket(s);
	WSACleanup();





	return 0;
}