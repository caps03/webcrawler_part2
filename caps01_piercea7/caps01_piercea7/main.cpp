/* 
	main.cpp
	Authors: Steve Cap Tony Pierce
	Date: 8/30/2017
	Purpose: This main file calls winsock_test() which sends a http get message to a server and recieves a webpage back
*/
#include "crawl.h"
int main(int argc, const char* argv[])
{
	string url = "www.google.com";
	char *url1 = "http://www.google.com";
	int numThrends = 0;
	string filename = "";
	if (argc > 1)
	{
		numThrends = atoi(argv[1]);
		filename = argv[2];
		printf("number of thrends is %d, filename is %s\n\n", numThrends, filename.c_str());
	}
	MySocket s;
	ifstream file_;
	file_.open(filename.c_str());
	string nextURL;
	if (file_.fail()) {
		printf("No such a file. Failed to open it\n");  
		return 0;
	}
	while (!file_.eof()) {
		file_ >> nextURL;
		s.Q.push(nextURL);
	}
	file_.close();
	string temp = s.Q.front();
	char *ttemp;
	ttemp = &temp[0];
	while (!s.Q.empty()) {
		string temp = s.Q.front();
		char *ttemp;
		ttemp = &temp[0];
		printf("URL: %s\n", ttemp);
		s.winsock_test(ttemp);
		s.Q.pop();
	}
	printf("\nPress enter to exit...\n");
	getchar();
	return 0;
}