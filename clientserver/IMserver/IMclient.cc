/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

/*
 * Send an integer to the server as four bytes.
 */
void writeNumber(const Connection& conn, int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

/*
 * Read a string from the server.
 */
string readString(const Connection& conn) {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
}

bool is_numerical(const string& s){
	return !s.empty() && find_if(s.begin(), s.end(), [](char c) {return !isdigit(c); }) == s.end();
}

void display_manual(){
	cout << "You are now connected to the server. \n The different operations you can perform are listed below:" << endl;
	cout << "1. List newsgroups." << endl;
	cout << "2. Create newgroup. <title>" << endl;
	cout << "3. Delete newsgroup. <ID>" << endl;
	cout << "4. List articles. <ID>" << endl;
	cout << "5. Create article. <ID> <title> <author> \\n <text>" << endl;
	cout << "6. Delete article. <IDgroup> <IDarticle>" << endl;
	cout << "7. Get article. <IDgroup> <IDarticle>" << endl;
	cout << "To display this manual again type \"help\", \"h\", \"man\" or \"manual\"" << endl;
}


int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	auto conn = make_shared<Connection>(argv[1], port);
	if (!conn->isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	MessageHandler mh;
	mh.set_connection(conn);
	display_manual();
	int com;
	string input;
	for(;;) {
		try {
			cout << ">>>";
			cin >> input;
			if(is_numerical(input)){com = stoi(input);}
			switch(com) {
				case 1:
			
				break;

				case 2:
			
				break;

				case 3:
			
				break;
				
				case 4:
			
				break;
				
				case 5:
			
				break;
				
				case 6:
			
				break;
				
				case 7:
			
				break;
				
				default:
			
				break;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

