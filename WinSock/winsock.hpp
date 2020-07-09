#pragma once


#include <winsock2.h>
#include <iostream>

#ifndef SFML_NETWORK
#pragma comment(lib,"ws2_32.lib")
#endif // !SFML_NETWORK


#define WINSOCK_VER MAKEWORD(2, 2)
inline bool Setup()
{
	WSADATA wsaData;
	return (WSAStartup(WINSOCK_VER, &wsaData) == 0);
}

#define Cleanup() (WSACleanup() != SOCKET_ERROR)



SOCKET ConnectToServer(const char* szServerName, WORD portNum)
{
	struct hostent* hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)& addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*) & server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}
#define CloseSocket(sock) (closesocket(sock) != SOCKET_ERROR)

#define Send(socket, pdata, iflags) (send(socket, pdata, strlen(pdata), iflags) != SOCKET_ERROR)

#define BUFF_LEN 2056
bool Receive(SOCKET sock, std::string & data)
{
	data.clear();
	char buffer[BUFF_LEN];
	int nDataLength = 0;
	// recieve html
	while ((nDataLength = recv(sock, buffer, BUFF_LEN, 0)) > 0)
	{
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
		{

			data += buffer[i];
			i++;
		}
	}
	return data.length();
}














#define Pause(TXT) std::cout << TXT << std::endl; std::cin.ignore(); //cin.get();

int test_win_sock(const std::string& host)
{
	SOCKET sock;
	std::string recv_data;

	//HTTP GET
	//char *header = "GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
	//send(sockfd, header, strlen(header), 0);
	std::string get_http = "GET / HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";

	std::cout << "---Info:" << std::endl << "Web Request:" << std::endl << get_http << std::endl;

	if (Setup())
	{
		sock = ConnectToServer(host.c_str(), 80);

		if (sock == NULL)
		{
			perror("ERROR: Could not connect\n");
			return -2;
		}
	}
	else
	{
		perror("ERROR: Failed to init\n");
		return -1;
	}


	// send GET / HTTP
	if (Send(sock, get_http.c_str(), 0))
	{
		size_t nRecvLen = Receive(sock, recv_data);
		if (nRecvLen > 0)
		{
			// Display HTML source 
			printf("=================================[ReceivedData(%d)]=================================\n", nRecvLen);
			std::cout << recv_data << std::endl << std::endl;
		}
		else
		{
			perror("ERROR: No data received...\n");
		}
	}
	else
	{
		perror("ERROR: Failed to send data...\n");
	}

	if (!CloseSocket(sock))
		perror("ERROR: Failed to close socket\n");
	if (!Cleanup())
		perror("ERROR: Failed to cleanup\n");



	// pause
	Pause("\n\nPress ANY key to close.");


	return 0;
}