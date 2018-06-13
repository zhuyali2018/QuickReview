
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
		countdown--;
		if (countdown <= 0) {			
			QA->SetAttribute("countdown", resetto);
		}
		else {
			QA->SetAttribute("countdown", countdown);
		}
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
	
	XMLDocument* doc = new XMLDocument();          //in tinyxml2 namespace
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <questionDataFile>" << endl;
		//return -1;
		doc->LoadFile("QASheet.txt");
	}
	else {
		doc->LoadFile(argv[1]);
	}

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
	int questionNo = 1;
	for (Question q : questions) {
		system("CLS");   //clear the screen of command line window
		printf("(%d) Question %d : %s\n", q.id,questionNo++, q.question.c_str());
		cin.get();
		printf("Answer : %s\n", q.answer.c_str());
		cin.get();
	}
	cin.get();
	string newPath = "C:\\cygwin64\\home\\yali.zhu\\YaliSources\\QuickReviewer\\Debug\\NewlySavedXMLFile.xml";
	doc->SaveFile(newPath.c_str());
	errorID = doc->ErrorID();
	if (errorID) {
		printf("XML file '%s' Save failed: ErrorID=%d\n", argv[1], errorID);		
		exit(0);
	}
	printf("XML file '%s' Saved successfully: ErrorID=%d\n", newPath.c_str(), errorID);
	return 0;
}
