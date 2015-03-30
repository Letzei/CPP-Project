#include "article.h"
#include <string>

using namespace std;

Article::Article(int id, string title, string author, string content) :
	id(id), title(title), author(author), content(content) {}

//Article::~Article() {}

int Article::get_id() const {
	return id;
}

void Article::set_id(const int new_id){
	id = new_id;
}

string Article::get_title() const {
	return title;
}

string Article::get_author() const {
	return author;
}

string Article::get_content() const {
	return content;
}