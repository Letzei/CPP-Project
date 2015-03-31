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

	int nr_ngs = mh.read_number();
	cout << ">>> Number of Newsgoups: " << nr_ngs << endl;

	for(int i = 0; i < nr_ngs; ++i){
		int id = mh.read_number();
		cout << ">>> Newsgroup ID: " << id << "		";

		string name = mh.read_string();
		cout << ">>> Newsgroup Name: " << name << endl;
	}
	if(mh.read_command() != Protocol::ANS_END){
		throw exception();
	}
}

void create_newsgroup(MessageHandler mh){
	mh.write_command(Protocol::COM_CREATE_NG);
	cout << ">>> Type the name of the newsgroup you want to create" << endl;
	cout << ">>> ";
	string name;
	cin >> name;
	mh.write_string(name);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_CREATE_NG){
		throw exception();
	}
	unsigned char ans = mh.read_command();
	if(ans == Protocol::ANS_ACK){
		cout << ">>> Newsgroup was succesfully created" << endl;
	} else {
		if(ans != Protocol::ANS_NAK) { throw exception(); }
		if(mh.read_command() != Protocol::ERR_NG_ALREADY_EXISTS) { throw exception(); }
		cout << ">>> Newsgroup already exists" << endl;
	}
	if(mh.read_command() != Protocol::ANS_END){
		throw exception();
	}
}

void delete_newsgroup(MessageHandler mh){
	mh.write_command(Protocol::COM_DELETE_NG);
	cout << ">>> Type the ID number of the newsgroup you want to delete." << endl;
	cout << ">>> ";
	int id;
	cin >> id;
	mh.write_number(id);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_DELETE_NG) { throw exception(); }
	unsigned char ans = mh.read_command();
	if(ans == Protocol::ANS_ACK) {
		cout << ">>> Newsgroup deleted." << endl;
	} else {
		if(ans != Protocol::ANS_NAK) { throw exception(); }
		if(mh.read_command() != Protocol::ERR_NG_DOES_NOT_EXIST) { throw exception(); }
		cout << ">>> A newsgroup with that ID does not exist." << endl;

	}
	if(mh.read_command() != Protocol::ANS_END){ throw exception(); }
}

void list_articles(MessageHandler mh){
	mh.write_command(Protocol::COM_LIST_ART);
	cout << ">>> Type the ID of the newsgroup containing the articles." << endl;
	int id;
	cout << ">>> ";
	cin >> id;
	mh.write_number(id);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_LIST_ART) { throw exception(); }
	unsigned char ans = mh.read_command();
	if(ans == Protocol::ANS_ACK) {
		int nbr_articles = mh.read_number();
		for(int i = 0; i < nbr_articles; ++i){
			cout << ">>> ID: " << mh.read_number() << "	";
			cout << ">>> Title: " << mh.read_string() << endl;
		}

	} else {
		if(ans != Protocol::ANS_NAK) { throw exception(); }
		if(mh.read_command() != Protocol::ERR_NG_DOES_NOT_EXIST) { throw exception(); }
	}
	if(mh.read_command() != Protocol::ANS_END){ throw exception(); }
}

void create_article(MessageHandler mh) {
	mh.write_command(Protocol::COM_CREATE_ART);
	cout << ">>> Type the ID of the Newsgroup." << endl;
	int id;
	cin >> id;

	cout << ">>> Type the Title of the article." << endl;
	string title;
	cin >> title;

	cout << ">>> Type the Author of the article." << endl;
	string author;
	cin >> author;

	cout << ">>> Type the text of the article, without pressing ENTER." << endl;
	string text;
	cin >> text;
	
	mh.write_number(id);
	mh.write_string(title);
	mh.write_string(author);
	mh.write_string(text);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_CREATE_ART) { throw exception(); }

	unsigned char ans = mh.read_command();
	if(ans == Protocol::ANS_ACK) {
		cout << ">>> Article created." << endl;
	} else if (ans == Protocol::ANS_NAK) {
		if(mh.read_command() != Protocol::ERR_NG_DOES_NOT_EXIST) { throw exception(); }
		cout << ">>> Couldn't create article, newsgroup does not exist." << endl;
	} else { throw exception(); }
	if(mh.read_command() != Protocol::ANS_END){ throw exception(); }
}

void delete_article(MessageHandler mh) {
	mh.write_command(Protocol::COM_DELETE_ART);
	cout << ">>> Type the ID of the Newsgroup you want to delete." << endl;
	int ng_id;
	cin >> ng_id;
	cout << ">>> Type the ID of the Article you want to delete." << endl;
	int art_id;
	cin >> art_id;
	mh.write_number(ng_id);
	mh.write_number(art_id);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_DELETE_ART) { throw exception(); }

	unsigned char ans = mh.read_command();
	if(ans == Protocol::ANS_ACK) {
		cout << ">>> Article deleted." << endl;
	} else if (ans == Protocol::ANS_NAK) {
		unsigned char com = mh.read_command();
		if(com == Protocol::ERR_NG_DOES_NOT_EXIST) {
			cout << ">>> Could not delete article, newsgroup does not exist." << endl;
		} else if(com == Protocol::ERR_ART_DOES_NOT_EXIST){ 
			cout << ">>> Could not delete article, article does not exist." << endl;
		}else { throw exception(); }
	}
	if(mh.read_command() != Protocol::ANS_END){ throw exception(); }
}

void get_article(MessageHandler mh){
	mh.write_command(Protocol::COM_GET_ART);
	cout << ">>> Type the ID of the Newsgroup you want to get." << endl;
	int ng_id;
	cin >> ng_id;
	cout << ">>> Type the ID of the Article you want to get." << endl;
	int art_id;
	cin >> art_id;

	mh.write_number(ng_id);
	mh.write_number(art_id);
	mh.write_command(Protocol::COM_END);

	if(mh.read_command() != Protocol::ANS_GET_ART) { throw exception(); }
	unsigned char ans = mh.read_command();
	if(ans == Protocol::ANS_ACK){
		string name = mh.read_string();

		cout << ">>> Title: " << name << "	";

		name = mh.read_string();

		cout << ">>> Author: " << name << "	 ";

		name = mh.read_string();

		cout << ">>> Text: " << name << endl;
	} else if (ans == Protocol::ANS_NAK){
		unsigned char com = mh.read_command();
		if(com == Protocol::ERR_NG_DOES_NOT_EXIST) {
			cout << ">>> Could not fetch article, newsgroup does not exist." << endl;
		} else if(com == Protocol::ERR_ART_DOES_NOT_EXIST){ 
			cout << ">>> Could not fetch article, article does not exist." << endl;
		} else { throw exception(); }
	}
	if(mh.read_command() != Protocol::ANS_END) { throw exception();}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: IMclient host-name port-number." << endl;
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
			cout << ">>> "; 
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
				delete_newsgroup(mh);
				break;

				case 4: /* List Articles */
				list_articles(mh);
				break;

				case 5: /* Create Article */
				create_article(mh);
				break;

				case 6: /* Delete Article */
				delete_article(mh);
				break;

				case 7: /* Get Article */ 
				get_article(mh);
				break;

				default: /* not a number */
					display_manual();
				break;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

