#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>
#include<thread>


#pragma comment(lib,"ws2_32.lib")
using namespace std;


//intilize
bool initilize()
{
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}
void SendMsg(SOCKET s)
{
	cout << "enter your chatname: " << endl;
	string name;
	getline(cin, name);
	string message;
	while (1)
	{
		getline(cin, message);
		string msg = name + " : " + message;
		int bytesent = send(s, msg.c_str(), msg.length(), 0);
		if (bytesent == SOCKET_ERROR)
		{
			cout << "error sending message" << endl;
			break;
		}
		if (message == "quite")
		{
			cout << "stopping the application" << endl;
			break;
		}
	}
	closesocket(s);
	WSACleanup();

}
void ReceiveMsg(SOCKET s)
{
	char buffer[4096];
	int recvlength;
	string msg = "";
	while (1)
	{
		recvlength = recv(s, buffer, sizeof(buffer), 0);
		if (recvlength <= 0)
		{
			cout << "disconnected from the server" << endl;
			break;
		}
		else
		{
			msg = string(buffer, recvlength);
			cout << msg << endl;
		}

	}
	closesocket(s);
	WSACleanup();

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
	thread senderthread(SendMsg, s);
	thread receiverthread(ReceiveMsg, s);

	senderthread.join();
	receiverthread.join();



	return 0;
}