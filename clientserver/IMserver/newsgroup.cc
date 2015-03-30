#include <iostream>
#include <string>
#include <vector>
#include "newsgroup.h"
#include "article.h"

using namespace std;

Newsgroup::Newsgroup(int i, string n) : name(n), id(i) {}

Article Newsgroup::get_article(unsigned int i){
	Article empty(-1,"","","");
	if(i < articles.size()){
		return articles[i];
	}
	return empty;
}

string Newsgroup::get_name(){
	return name;
}

int Newsgroup::get_id(){
	return id;
}

void Newsgroup::set_id(const int new_id){
	id = new_id;
}

vector<Article> Newsgroup::get_articles(){
	vector<Article> res;
	for(Article a : articles){
		if(a.get_id() > -1){
			res.push_back(a);
		}
	}
	return res;
}

void Newsgroup::add_article(Article& art){
	articles.push_back(art);
}

void Newsgroup::delete_article(const int id){
	size_t i = 0;
	while (i < articles.size() && articles[i].get_id() != id) {
		++i;
	}
	if (i < articles.size()){
		articles[i].set_id(-1);
	}
}
