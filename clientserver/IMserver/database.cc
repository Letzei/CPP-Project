#include "database.h"
#include "article.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

Database::Database()
{
	file = "";
}

Database::Database(const string& filename){
	//read from file.. maybe make a method get_file() to check from outside if we want to write to/from a file
	file = filename;
}

int Database::get_size(){
	int size = count_if(newsgroups.begin(), newsgroups.end(), [&] 
		(Newsgroup ng) { return ng.get_id() > -1; });
	return size;
}

vector<Newsgroup> Database::list_all_newsgroups() const {
	vector<Newsgroup> ngs;
	for(Newsgroup ng: newsgroups){
		if(ng.get_id() > -1){
			ngs.push_back(ng);
		}
	}
	return ngs;
}


bool Database::create_newsgroup(const std::string& newsgroup){
	for(auto ng : newsgroups){
		if(ng.get_name() == newsgroup && ng.get_id() > -1){
			return false;
		}
	}
	int id = newsgroups.size();
	Newsgroup ng(id, newsgroup);
	newsgroups.push_back(ng);
	return true;
}


bool Database::newsgroup_exists(const string& name){
	for(auto e : newsgroups){
		if(e.get_name() == name){
			return true;
		}
	}
	return false;
}

bool Database::newsgroup_exists(const int id){
	for(auto e : newsgroups){
		if(e.get_id() == id){
			return true;
		}
	}
	return false;
}

bool Database::delete_newsgroup(int id){
	if(newsgroup_exists(id)){
		newsgroups[id].set_id(-1);
		return true;
	}
	return false;
}


vector<Article> Database::list_articles(int id){
	vector<Article> empty;
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [&] 
		(Newsgroup ng) { return ng.get_id() == id; });
	if(it != newsgroups.end()){
		return newsgroups[id].get_articles();
	} else {
		return empty;
	}
}


bool Database::create_article(int ng_id, const string& title, 
					const string& author, const string& text){
	auto it = find_if(newsgroups.begin(), newsgroups.end(), [&] 
		(Newsgroup ng) { return ng.get_id() == ng_id; });

	if(it != newsgroups.end()){
		int id = newsgroups[ng_id].get_articles().size();
		Article art(id, title, author, text);
		newsgroups[ng_id].add_article(art);
		return true;
	} else {
		return false;
	}
}


bool Database::delete_article(const int ng_id, const int art_id){
	if(newsgroup_exists(ng_id)){
		if(article_exists(art_id)){
			newsgroups[ng_id].delete_article(art_id);
			return true;
		}
	}
	return false;
}


bool Database::article_exists(int id){
	for(Newsgroup ng : newsgroups){
		for(Article art : ng.get_articles()){
			if(art.get_id() == id && art.get_id() > -1){
				return true;
			}
		}
	}
	return false;
}

Article Database::get_article(const int ng_id, const int art_id){
	if(newsgroup_exists(ng_id)){
		if(article_exists(art_id)){
			return newsgroups[ng_id].get_article(art_id);
		}
	}
	Article art(-1,"","","");
	return art;
}