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
	std::string read_string(int n);
	void write_string(const std::string&);
private:
	std::shared_ptr<Connection> connection;
};
#endif