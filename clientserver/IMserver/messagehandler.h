#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <string>
#include <iostream>
#include "connection.h"
#include <vector>
#include <memory>

class MessageHandler 
{
public:
	MessageHandler();
	unsigned char read_command();
	void set_connection(const std::shared_ptr<Connection>& conn);
	void write_command(unsigned char);
	int read_number();
	void write_number(int);
	void write_byte(int);
	int read_byte();
	std::string read_string();
	void write_string(const std::string&);
private:
	std::shared_ptr<Connection> connection;
};
#endif