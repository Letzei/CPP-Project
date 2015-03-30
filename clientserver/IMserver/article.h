#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article {
public:
	Article(int, std::string, std::string, std::string);
	//~Article();
	int get_id() const;
	void set_id(const int);
	std::string get_title() const;
	std::string get_author() const;
	std::string get_content() const;
private:
	int id;
	std::string title;
	std::string author;
	std::string content;
};
#endif