#include<iostream>
#include<Winsock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
using namespace std;

#pragma comment(lib, "ws2_32.lib")


bool Initilization()
{
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}
void InteractWithClient(SOCKET clientsocket)
{
	//send receive
	cout << "client connected" << endl;

	char buffer[4096];
	while (1)
	{
		int byterecvd = recv(clientsocket, buffer, sizeof(buffer), 0);

		if (byterecvd <= 0)
		{
			cout << "client disconnected" << endl;
			break;
		}

		string message(buffer, byterecvd);
		cout << "message from client " << message << endl;


	}

	closesocket(clientsocket);

}
int main()
{
	if (!Initilization())
	{
		cout << "winswock initilization is failed" << endl;
		return 1;
	}
	cout << "server program " << endl;

	SOCKET  listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSocket == INVALID_SOCKET)
	{
		cout << "socket creation failed" << endl;
		return 1;
	}

	//create address structure
	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	//convert the ip address put inside sin family into binary number
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1)
	{
		cout << "setting address structure failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	//bind ip and port with the socket
	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
	{
		cout << "bind failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;

	}

	//listen on the socket and port
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listen failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;

	}

	cout << "server has started listening on port: " << port << endl;
	//accept the connection from the client

	while (1)
	{
		SOCKET clientsocket = accept(listenSocket, nullptr, nullptr);
		if (clientsocket == INVALID_SOCKET)
		{
			cout << "invalid client socket" << endl;

		}
		thread t1(InteractWithClient, clientsocket);

	}




	closesocket(listenSocket);



	WSACleanup();
	return 0;
}
