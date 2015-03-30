/* IMserver.cc */
#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "article.h"
#include "database.h"
#include "messagehandler.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

bool flag = false;
Database db;
MessageHandler mh;



void list_newsgroups()
{
	cout << "in list_newsgroups().. reading command.." << endl;
	if(mh.read_command() != Protocol::COM_END) {
		cout << "Received wrong command in list_newsgroups(). Expected COM_END" << endl;
		throw exception();
	}
	cout << "Writing ANS_LIST_NG to mh" << endl;
	mh.write_command(Protocol::ANS_LIST_NG);

	vector<Newsgroup> newsgroups = db.list_all_newsgroups();

	cout << "EFTER LIST ALL NEWSGROUPS()" << endl;

	mh.write_command(Protocol::PAR_NUM);
	mh.write_number(db.get_size());
	for (int i = 0; i < db.get_size(); ++i) {
		mh.write_command(Protocol::PAR_NUM);
		mh.write_number(newsgroups[i].get_id());
		mh.write_string(newsgroups[i].get_name());
	}
	cout << "Klar.. gör nu ANS_END" << endl;
	mh.write_command(Protocol::ANS_END);
}

void create_newsgroup(){
	cout << "Inside create_newsgroup()" << endl;

	if(mh.read_command() != Protocol::PAR_STRING) {
		throw exception();
	}
	int number_of_chars = mh.read_number();
	cout << "NUMBER OF CHARS = " << number_of_chars << endl;
	string newsgroup = mh.read_string(number_of_chars);

	if(mh.read_command() != Protocol::COM_END) {
		cout << "Received wrong command in create_newsgroup().. Expected COM_END" << endl;
		throw exception();
	}
	cout << "Writing ANS_CREATE_NG to mh" << endl;
	mh.write_command(Protocol::ANS_CREATE_NG);

	if(!db.newsgroup_exists(newsgroup)) {
		if(!db.create_newsgroup(newsgroup)){
			throw exception();
		}
		mh.write_command(Protocol::ANS_ACK);
	} else {
		mh.write_command(Protocol::ANS_NAK);
		mh.write_command(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	cout << "Klar.. gör nu ANS_END" << endl;
	mh.write_command(Protocol::ANS_END);
}

void delete_newsgroup(){
	if(mh.read_command() != Protocol::PAR_NUM) {
		cout << "Received wrong command in delete_newsgroup().. Expected PAR_NUM" << endl;
		throw exception();
	}

	int id = mh.read_number();

	if(mh.read_command() != Protocol::COM_END) {
		cout << "Received wrong command in delete_newsgroup().. Expected COM_END" << endl;
		throw exception();
	}
	cout << "Writing ANS_DELETE_NG to mh" << endl;
	mh.write_command(Protocol::ANS_DELETE_NG);

	if(db.newsgroup_exists(id)) {
		if(!db.delete_newsgroup(id)){
			throw exception();
		}
		mh.write_command(Protocol::ANS_ACK);
	} else {
		mh.write_command(Protocol::ANS_NAK);
		mh.write_command(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	cout << "Klar.. gör nu ANS_END" << endl;
	mh.write_command(Protocol::ANS_END);
}

void list_articles(){
	if(mh.read_command() != Protocol::PAR_NUM) {
		cout << "Received wrong command in create_article().. Expected PAR_NUM" << endl;
		throw exception();
	}

	int id = mh.read_number();

	if(mh.read_command() != Protocol::COM_END) {
		cout << "Received wrong command in create_article().. Expected PAR_NUM" << endl;
		throw exception();
	}

	mh.write_command(Protocol::ANS_LIST_ART);

	if(db.newsgroup_exists(id)){
		mh.write_command(Protocol::ANS_ACK);
		vector<Article> arts = db.list_articles(id);
		mh.write_command(Protocol::PAR_NUM);
		mh.write_number(arts.size());
		for(auto a : arts) {
			mh.write_command(Protocol::PAR_NUM);
			mh.write_number(a.get_id());
			mh.write_string(a.get_title());
		}
		cout << "ARTICLES LISTED" << endl;
	} else {
		mh.write_command(Protocol::ANS_NAK);
		mh.write_command(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh.write_command(Protocol::ANS_END);
}

void create_article(){
	if(mh.read_command() != Protocol::PAR_NUM) {
		cout << "Received wrong command in create_article().. Expected PAR_NUM" << endl;
		throw exception();
	}

	int id = mh.read_number();

	string title;
	string author;
	string text;

	if(mh.read_command() != Protocol::PAR_STRING) {
		cout << "Received wrong command in create_article().. Expected PAR_STRING" << endl;
		throw exception();
	}

	int number_of_chars = mh.read_number();
	title = mh.read_string(number_of_chars);

	if(mh.read_command() != Protocol::PAR_STRING) {
		cout << "Received wrong command in create_article().. Expected PAR_STRING" << endl;
		throw exception();
	}

	number_of_chars = mh.read_number();
	author = mh.read_string(number_of_chars);

	if(mh.read_command() != Protocol::PAR_STRING) {
		cout << "Received wrong command in create_article().. Expected PAR_STRING" << endl;
		throw exception();
	}

	number_of_chars = mh.read_number();
	text = mh.read_string(number_of_chars);

	cout << "ID = " << id << endl;
	cout << "TITLE = " << title << endl;
	cout << "AUTHOR = " << author << endl;
	cout << "TEXT = " << text << endl;

	if(mh.read_command() != Protocol::COM_END) {
		cout << "Received wrong command in create_article().. Expected COM_END" << endl;
		throw exception();
	}

	mh.write_command(Protocol::ANS_CREATE_ART);
	if(db.newsgroup_exists(id)){
		if(!db.create_article(id, title, author, text)){
			throw exception();
		}
		cout << "ARTICLE CREATED" << endl;
		mh.write_command(Protocol::ANS_ACK);
	} else {
		mh.write_command(Protocol::ANS_NAK);
		mh.write_command(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh.write_command(Protocol::ANS_END);
}

void delete_article(){
	if(mh.read_command() != Protocol::PAR_NUM) {
		cout << "Received wrong command in delete_article().. Expected PAR_NUM" << endl;
		throw exception();
	}
	int ng_id = mh.read_number();

	if(mh.read_command() != Protocol::PAR_NUM) {
		cout << "Received wrong command in delete_article().. Expected PAR_NUM" << endl;
		throw exception();
	}
	int art_id = mh.read_number();

	if(mh.read_command() != Protocol::COM_END) {
		cout << "Received wrong command in delete_article().. Expected COM_END)" << endl;
		throw exception();
	}

	mh.write_command(Protocol::ANS_DELETE_ART);
	if(db.newsgroup_exists(ng_id)){
		if(db.article_exists(art_id)){
			db.delete_article(ng_id, art_id);
			mh.write_command(Protocol::ANS_ACK);
		} else {
				mh.write_command(Protocol::ANS_NAK);
				mh.write_command(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
	} else {	
				mh.write_command(Protocol::ANS_NAK);
				mh.write_command(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh.write_command(Protocol::ANS_END);
}

void get_article(){
	if(mh.read_command() != Protocol::PAR_NUM) {
		cout << "Received wrong command in delete_article().. Expected PAR_NUM" << endl;
		throw exception();
	}
	int ng_id = mh.read_number();

	if(mh.read_command() != Protocol::PAR_NUM) {
		cout << "Received wrong command in delete_article().. Expected PAR_NUM" << endl;
		throw exception();
	}
	int art_id = mh.read_number();

	if(mh.read_command() != Protocol::COM_END) {
		cout << "Received wrong command in delete_article().. Expected COM_END)" << endl;
		throw exception();
	}

	mh.write_command(Protocol::ANS_GET_ART);
	if(db.newsgroup_exists(ng_id)){
		if(db.article_exists(art_id)){
			mh.write_command(Protocol::ANS_ACK);
			Article art = db.get_article(ng_id, art_id);
			string title = art.get_title();
			string author = art.get_author();
			string text = art.get_content();

			mh.write_string(title);
			mh.write_string(author);
			mh.write_string(text);
		} else {
			mh.write_command(Protocol::ANS_NAK);
			mh.write_command(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
	} else {
		mh.write_command(Protocol::ANS_NAK);
		mh.write_command(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	mh.write_command(Protocol::ANS_END);
}

int main(int argc, char* argv[]){
	if (argc < 2) {
		cerr << "Usage: %s" << argv[0] << "<port-number> [database]" << endl;
		exit(1);
	}

	if(argc < 3) {
		flag = false;
	}

	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	} else {
		cout << "Server Initialized." << endl;
	}
	while (true) {
		cout << "======inne i WHILE.. gör nu waitForActivity()..=====" << endl;
		auto conn = server.waitForActivity();

		cout << "efter waitForActivity()" << endl;
		mh.set_connection(conn);

		if (conn != nullptr) {
			cout << "conn != null" << endl;
			try {
				cout << "i TRY.. gör nu MessageHandler.read_command" << endl;
				unsigned char nbr = mh.read_command();
				string result;
				cout << "command = " << nbr << endl;

				switch(nbr){
					cout << "i switch case" << endl;
					case Protocol::COM_LIST_NG: // list newsgroups
					cout << "***case COM_LIST_NG***" << endl;
					list_newsgroups();
					break;

					case Protocol::COM_CREATE_NG: // create newsgroup
					cout << "***case COM_CREATE_NG***" << endl;
					create_newsgroup();
					break;

					case Protocol::COM_DELETE_NG: // delete newsgroup
					cout << "***case COM_DELETE_NG***" << endl;
					delete_newsgroup();
					break;

					case Protocol::COM_LIST_ART: // list articles
					cout << "***case COM_LIST_ART***" << endl;
					list_articles();
					break;

					case Protocol::COM_CREATE_ART: // create article
					cout << "***case COM_CREATE_ART***" << endl;
					create_article();
					break;

					case Protocol::COM_DELETE_ART: // delete article
						cout << "***case COM_DELETE_ART***" << endl;
						delete_article();
						break;

					case Protocol::COM_GET_ART: // get article
						cout << "***case COM_GET_ART***" << endl;
						get_article();
						break;

					default:
						cout << "***case DEFAULT***" << endl;
						break;
					}
				} catch (ConnectionClosedException&) {
					server.deregisterConnection(conn);
					cout << "Client closed connection" << endl;
				}
			} else {
				cout << "conn = NULL!!" << endl;
				conn = make_shared<Connection>();
				server.registerConnection(conn);
				cout << "New client connects" << endl;
			}
		}
	}
