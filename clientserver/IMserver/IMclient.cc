/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "messagehandler.h"
#include "protocol.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

/*
bool is_numerical(const string& s){
	return !s.empty() && find_if(s.begin(), s.end(), [](char c) {return !isdigit(c); }) == s.end();
}
*/
void display_manual(){
	cout << "==============================================================================================" << endl;
	cout << "The different operations you can perform are listed below:" << endl;
	cout << "Type the number of the desired command, then press ENTER, followed by the parameters seperated with a space" << endl;
	cout << "1. List newsgroups." << endl;
	cout << "2. Create newgroup. <title>" << endl;
	cout << "3. Delete newsgroup. <ID>" << endl;
	cout << "4. List articles. <ID>" << endl;
	cout << "5. Create article. <ID> <title> <author> \\n <text>" << endl;
	cout << "6. Delete article. <IDgroup> <IDarticle>" << endl;
	cout << "7. Get article. <IDgroup> <IDarticle>" << endl;
	cout << "To display this manual again type \"help\", \"h\", \"man\" or \"manual\"" << endl;
	cout << "==============================================================================================" << endl;

}

void list_newsgroups(MessageHandler mh){
	mh.write_command(Protocol::COM_LIST_NG);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_LIST_NG){
		throw exception();
	}
	if(mh.read_command() != Protocol::PAR_NUM){
		throw exception();
	}

	int nr_ngs = mh.read_number();
	cout << "Number of Newsgoups: " << nr_ngs << endl;

	for(int i = 0; i < nr_ngs; ++i){
		if(mh.read_command() != Protocol::PAR_NUM){
			throw exception();
		}
		int id = mh.read_number();
		cout << "Newsgroup ID: " << id << "		";
		if(mh.read_command() != Protocol::PAR_STRING){
			throw exception();
		}
		int nbr_chars = mh.read_number();
		string name = mh.read_string(nbr_chars);
		cout << "Newsgroup Name: " << name << endl;
	}
	if(mh.read_command() != Protocol::ANS_END){
		throw exception();
	}
	cout << "List ngs klar" << endl;
}

void create_newsgroup(MessageHandler mh){
	mh.write_command(Protocol::COM_CREATE_NG);
	cout << "Type the name of the newsgroup you want to create" << endl;
	cout << ">>>";
	string name;
	cin >> name;
	mh.write_string(name);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_CREATE_NG){
		throw exception();
	}
	if(mh.read_command() == Protocol::ANS_ACK){
		cout << "Newsgroup was succesfully created" << endl;
	} else {
		if(mh.read_command() != Protocol::ANS_NAK) { throw exception(); }
		if(mh.read_command() != Protocol::ERR_NG_ALREADY_EXISTS) { throw exception(); }
		cout << "Newsgroup already exists" << endl;
	}
	if(mh.read_command() != Protocol::ANS_END){
		throw exception();
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: IMclient host-name port-number" << endl;
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

	cout << "You are now connected to the server." << endl;
	display_manual();
	int com;
	string input;
	for(;;) {
		try {
			cout << ">>>";
			cin >> input;
			com = atoi(input.c_str());
			switch(com) {
				case 1: /* List Newsgroups */
				list_newsgroups(mh);
				break;

				case 2: /* Create a Newsgroup */
				create_newsgroup(mh);
				break;

				case 3: /* Delete Newsgroup */
				break;

				case 4: /* List Articles */
				break;

				case 5: /* Create Article */
				break;

				case 6: /* Delete Article */
				break;

				case 7: /* Get Article */ 
				break;

				default: /* not a number */
				break;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

