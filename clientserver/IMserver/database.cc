#include "database.h"
#include "article.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

Database::Database(){}

void Database::set_persistent(bool f){
	in_file = f;	
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


void Database::write_database(string file_name){
	if(!in_file){
		return;
		cout << "Not using filesystem." << endl;
	}
	ofstream ofile(file_name);
	ofile << newsgroups;
}

std::istream& operator>>(istream& inf, Database& db){
	string nwname, title, author, content, ng;
	int id = 0;
	while(getline(inf, ng, '#')){
		cout << "Ng at the moment: " << endl << ng << endl << endl;
		size_t i = ng.find("$");
		nwname = ng.substr(0, i);
		ng.erase(0, i+1);
		db.create_newsgroup(nwname);
		while(ng.find("$") != string::npos){
			i = ng.find("$");
			title = ng.substr(0, i);
			ng.erase(0, i+1);
			
			i = ng.find("$");
			author = ng.substr(0, i);
			ng.erase(0, i+1);
			
			i = ng.find("$");
			content = ng.substr(0, i);
			ng.erase(0, i+1);
			cout << "ID: " << id;
			//cout << "Title: " << title << "    Author: " << author << "    Content: " << content << " :" << endl << endl << endl;
			db.create_article(id, title, author, content);
		}	
		id++;
		//cout << "What was erased: " << nwname << endl;
		//cout << "What is left:  " << newsgroup << endl;
	}
	return inf;
}

void Database::read_database(string file_name){
	if(!in_file){
		cout << "Not using filesystem." << endl;
		return;	
	}	
	ifstream ifile(file_name);
		ifile >> *this;
}


std::ostream& operator<<(ostream& of, const vector<Newsgroup>& vect){
	for(auto nws : vect)
	{
		of << nws.name << "$";
		for(auto art : nws.articles)
		{
			of << art.get_title() << "$" << art.get_author() << "$" << art.get_content() << "$";	
		}
		of << "#";
	}	
	return of;
}


