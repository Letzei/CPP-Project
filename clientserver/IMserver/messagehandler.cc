#include "messagehandler.h"
#include "connection.h"
#include "protocol.h"
#include <string>

using namespace std;

MessageHandler::MessageHandler() {}

void MessageHandler::set_connection(const shared_ptr<Connection>& conn) {
	connection = conn;
}

int MessageHandler::read_number(){
	if(read_command() != Protocol::PAR_NUM){
		throw exception();
	}
	return read_byte();
}

void MessageHandler::write_number(int com){
	write_command(Protocol::PAR_NUM);
	write_byte(com);
}

int MessageHandler::read_byte(){
	unsigned char byte1 = connection->read();
	unsigned char byte2 = connection->read();
	unsigned char byte3 = connection->read();
	unsigned char byte4 = connection->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void MessageHandler::write_byte(int com){
	unsigned char byte1 = (com >> 24) & 0xff;
	unsigned char byte2 = (com >> 16) & 0xff;
	unsigned char byte3 = (com >> 8) & 0xff;
	unsigned char byte4 = com & 0xff;

	connection->write(byte1);
	connection->write(byte2);
	connection->write(byte3);
	connection->write(byte4);
}

unsigned char MessageHandler::read_command(){
	return connection->read();
}

void MessageHandler::write_command(unsigned char com) {
	connection->write(com & 0xff);
}

string MessageHandler::read_string(){
		if(read_command() != Protocol::PAR_STRING) { throw exception(); }
		int len = read_byte();

		string s;
		char ch;
		for(int i = 0; i < len; ++i){
			ch = connection->read();
			s+= ch;
		}
		return s; 
}

void MessageHandler::write_string(const string& s){
	write_command(Protocol::PAR_STRING);
	write_byte(s.length());
	for(int i = 0; s[i] != '\0'; ++i){
		connection->write(s[i]);
	}
}

