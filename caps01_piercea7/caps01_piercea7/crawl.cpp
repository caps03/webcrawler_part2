/*  
	crawl.cpp
	Authors: Steve Cap Tony Pierce
	Date: 8/30/2017
	Purpose: Send HTTP GET message to website server and recieve the webpage back.
			 Can save the site to a fail if wanted currently commented out so hdd is not cluttered with a lot of files
			 To save websites to a file uncomment lines 112-117, 131, and 136
*/
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "crawl.h"
void MySocket::winsock_test(char * url)
{
	char *str1 = url;
	int port = 80;
	char *host = (char*)malloc(1024);
	char *path = (char*)malloc(1024);
	memset(path, 0, 1024);
	memset(host, 0, 1024);
	sscanf(str1, "http://%99[^/]%99[^\n]", host, path);
	if ((*path) == '\0')
	{
		path = "/";
	}
	printf("	Parsing URL... host %s, port 80\n", host);
	int prevSizeHosts = seenHosts.size();
	seenHosts.insert(host);
	if (seenHosts.size() > prevSizeHosts) {
		printf("	Checking host uniqueness... passed\n");
	}
	else {
		printf("	Checking host uniqueness... failed\n");
		return;
	}
	WSADATA wsaData;
	//Initialize WinSock; once per program run
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		printf("WSAStartup error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	// open a TCP socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		printf("socket() generated error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}
	// structure used in DNS lookups
	struct hostent *remote;
	// structure for connecting to server
	struct sockaddr_in server;
	// first assume that the string is an IP address
	DWORD IP = inet_addr(str1);
	if (IP == INADDR_NONE)
	{
		// if not a valid IP, then do a DNS lookup
		if ((remote = gethostbyname(host)) == NULL)
		{
			printf("Invalid string: neither FQDN, nor IP address\n");
			return;
		}
		else // take the first IP address and copy into sin_addr
			memcpy((char *)&(server.sin_addr), remote->h_addr, remote->h_length);
	}
	else
	{
		// if a valid IP, directly drop its binary version into sin_addr
		server.sin_addr.S_un.S_addr = IP;
	}
	// setup the port # and protocol type
	server.sin_family = AF_INET;
	server.sin_port = htons(80);		// host-to-network flips the byte order
	printf("	Doing DNS... found %s\n", inet_ntoa(server.sin_addr));
	int prevSizeIP = seenIPs.size();
	seenIPs.insert(server.sin_addr.S_un.S_addr);
	if (seenIPs.size() > prevSizeIP) {
		printf("	Checking IP uniqueness... passed\n");
	}
	else {
		printf("	Checking IP uniqueness... failed\n");
		return;
	}
	// connect to the server on port 80
	if (connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		printf("Connection error: %d\n", WSAGetLastError());
		return;
	}
	//printf("Successfully connected to %s (%s) on port %d\n", host, inet_ntoa(server.sin_addr), htons(server.sin_port));
	printf("	Connecting...\n");
	// send HTTP requests here
	char *sendGet = (char*)malloc(1024);
	sprintf(sendGet, "GET %s HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", path, host);
	int requestLen = strlen(sendGet);
	if (send(sock, sendGet, requestLen, 0) == SOCKET_ERROR)
	{
		printf("Send error: %d\n", WSAGetLastError()); return;
	}
	int MB = 1024;
	char recvBuf[1024];
	memset(recvBuf, 0, 1024);
	char *filename = (char*)malloc(1024);
	memset(filename, 0, 1024);
	strcat(filename, host);
	//strcat(filename, path);
	strcat(filename, ".html");
	//Can save the site to a fail if wanted currently commented out so hdd is not cluttered with a lot of files
	//To save websites to a file uncomment lines 112-117, 131, and 136
	/*FILE *f = fopen(filename, "w+");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		//exit(1);
	}*/
	int recv_count = 0;
	char *token = "\r\n\r\n";
	int total_bytes = 0;
	int byte_left = 0;
	printf("	Loading...\n");
	recv_count = recv(sock, recvBuf, 1024, 0);
	char *buf = strstr(recvBuf, " ");
	char *response = (char*)malloc(3);
	memset(response, 0, 4);
	memcpy(response, buf + 1, 3);
	printf("	Verifying header... status code %s\n", response);
	char *end_header = strstr(recvBuf, token);
	byte_left = recv_count - ((end_header - recvBuf) + 4);
	//fwrite(end_header + 4, byte_left, 1, f);
	memset(recvBuf, 0, MB);
	total_bytes += recv_count;
	while ((recv_count = recv(sock, recvBuf, MB, 0)) != 0)
	{
		//fwrite(recvBuf, recv_count, 1, f);
		memset(recvBuf, 0, MB);
		total_bytes += recv_count;
	}
	printf("	Saving the file... bytes saved %d\n", total_bytes);
	// close the socket to this server; open again for the next one
	closesocket(sock);
	// call cleanup when done with everything and ready to exit program
	WSACleanup();
}