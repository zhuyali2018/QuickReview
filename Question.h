#pragma once
#include <string>
#include "tinyxml2.h"
using namespace tinyxml2;
class Question
{
public:
	Question(int id, std::string type, std::string category, int cd, int rt, std::string question, std::string answer);
	~Question();
//private:
	int id;
	std::string qtype;
	std::string qcategory;
	int countdown;
	int resetto;
	std::string question;
	std::string answer;
	XMLElement * Qc;    //pointer to Question xml element
};
class NoDataException : public std::exception {
public:
	const char * what() const throw() {
		return "Question List: No Question Exception";
	}
};

