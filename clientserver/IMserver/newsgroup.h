#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <iostream>
#include <string>
#include <vector>
#include "article.h"
#include <fstream>
//#include "database.h"

class Newsgroup{

private:
	std::vector<Article> articles;
	std::string name;
	int id;
public:
	//friend std::istream& operator>>(std::istream& inf, std::vector<Newsgroup>&);	
	friend std::ostream& operator<<(std::ostream& of, const std::vector<Newsgroup>&);
	Newsgroup(int i, std::string n);
	
	Article get_article(unsigned int i);

	std::string get_name();

	int get_id();

	void set_id(const int);

	std::vector<Article> get_articles();

	void add_article(Article&);

	void delete_article(const int);
};
#endif
