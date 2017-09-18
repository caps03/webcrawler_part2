/*
	crawl.h
	Authors: Steve Cap Tony Pierce
	Date: 8/30/2017
	Purpose: Header file for crawl.cpp
*/
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUF_SIZE 1024 // array size
#define TIMEOUT 20000 // 20 seconds
#include <winsock2.h>
#include "common.h"
#include <unordered_set>
#include <queue>
class MySocket {
public:
	void winsock_test(char * url);
	unordered_set <DWORD> seenIPs;
	unordered_set <string> seenHosts;
	queue <string> Q;
private:
	char buf[BUF_SIZE]; // used for recv()
	SOCKET sock;
};