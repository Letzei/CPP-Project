#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "newsgroup.h"
#include "article.h"

class Database {
public:
	
	Database();
	
	//Specifies if the database will be in memory or written to a file
	void set_persistent(bool);

	Database(const std::string&);

	//~Database() = default;

	int get_size();

	/*
	* Lists all newsgroups. Returns array of newsgroups. If no newsgroups
	* exists an empty array is returned.
	*/
	std::vector<Newsgroup> list_all_newsgroups() const;

	/*
	* Create a newsgroup. Returns false if NewsGroup already exists.
	*/
	bool create_newsgroup(const std::string& newsgroup);

	/*
	* Returns true if a newsgroup or article with name 'name' or id 'id' exists
	*/
	bool newsgroup_exists(const std::string& name);
	bool newsgroup_exists(const int id);
	bool article_exists(const int id);

	/*
	* Deletes newsgroup. Returns true if newsgroup was deleted and
	* false if the newsgroup doesn't exist.
	*/
	bool delete_newsgroup(int id);

	/*
	* Lists all articles in specified newsgroup. Returns array of articles.
	* If no newsgroups exists an empty array is returned.
	*/
	std::vector<Article> list_articles(int id);

	/*
	* Adds new article in specified NewsGroup. Returns false
	* if newsgroup doesn't exists.
	*/
	bool create_article(int, const std::string&,
	const std::string&, const std::string&);

	/*
	* Adds new article in specified NewsGroup. Returns false
	* if newsgroup doesn't exists.
	*/
	bool delete_article(const int ng_id, const int art_id);
	Article get_article(const int ng_id, const int art_id);

	//Either saves the current information stored in the database to a file or does nothing.
	void write_database();
	void read_database();
private:
	std::vector<Newsgroup> newsgroups;
	bool in_mem;
};
#endif

