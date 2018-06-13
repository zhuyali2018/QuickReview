
#include "tinyxml2.h"
#include <cstring>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include "Question.h"

using namespace tinyxml2;
using namespace std;

vector<Question> GetAllQuestions(XMLElement * parent) {
	if (!parent) {
		throw NoDataException();
	}
	vector<Question> Qs;
	XMLElement* QA = parent->FirstChildElement("QA");;
	while (true) {
		int id;
		const char *qtype;
		const char *category;
		int countdown;
		int resetto;
		
		QA->QueryIntAttribute("ID", &id);
		QA->QueryStringAttribute("type", &qtype);
		QA->QueryStringAttribute("category", &category);
		QA->QueryIntAttribute("countdown", &countdown);
		QA->QueryIntAttribute("resetto", &resetto);

		const string question = QA->FirstChildElement("Question")->GetText();
		const string answer = QA->FirstChildElement("Answer")->GetText();
		
		Question q(id, qtype, category, countdown, resetto,question,answer);
		Qs.push_back(q);
		XMLElement * QB = (XMLElement *)QA->NextSibling();
		if (!QB) break;
		QA = QB;
	}
	return Qs;
}
int main(int argc, const char ** argv) 
{
	cout << "Quick Reviewer version 1.0" << endl;
	
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <questionDataFile>" << endl;
		return -1;
	}

	XMLDocument* doc = new XMLDocument();          //in tinyxml2 namespace
	doc->LoadFile(argv[1]);

	int errorID = doc->ErrorID();
	if (errorID) {
		printf("XML file '%s' loaded and parsed failed: ErrorID=%d\n", argv[1], errorID);
		cin.get();
		exit(0);
	}
	printf("XML file '%s' loaded and parsed successfully\n\n", argv[1]);
	
	//-----------------------------------------------------------------------------
	const char * name = "QuestionsAndAnswers";
	XMLElement* root = doc->FirstChildElement(name);
	vector<Question> questions = GetAllQuestions(root);
	for (Question q : questions) {
		printf("Question ID:%d\n", q.id);
		printf("Question type:%s\n", q.qtype.c_str());
		printf("Question cate:%s\n", q.qcategory.c_str());
		printf("Question countdown:%d\n", q.countdown);
		printf("Question resetto:%d\n\n", q.resetto);
		printf("Question : %s\n", q.question.c_str());
		printf("Answer : %s\n", q.answer.c_str());
	}
	cin.get();

	return 0;
}
