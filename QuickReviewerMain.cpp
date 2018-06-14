
#include "tinyxml2.h"
#include <cstring>
#include <iostream>
#include <istream>
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
void promptAtQuestion() {
	printf("\n\n\n\n --------------------------------------------\n");
	printf("           Number: jump to question numbered \n");
	printf("            Enter: Next \n");
	printf("                x: Exit \n");
	printf("       Your Input: ");
}
int main(int argc, const char ** argv) 
{
	cout << "Quick Reviewer version 1.4" << endl;
	
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
	
	//loop through each questions
	char k[32] = { 0 };
	int questionNo = 1;
	int qno = 0;
	int total = questions.size();
	for (Question q : questions) {
		if (qno > questionNo) {
			questionNo++;
			continue;
		}
		qno = 0;  //reset goto question No
		system("CLS");   //clear the screen of command line window
		printf("[Qid:%d]  Question %d/%d : \n%s\n", q.id,questionNo++,total, q.question.c_str());	
		cin.getline(k, 30);       //wait before show answer

		printf("Answer : \n%s\n", q.answer.c_str());
		promptAtQuestion();    //show options to exit or job to another questions
		
		cin.getline(k, 30);
		qno = atoi(k);		
		if (k[0] == 'x')
			break;
	}
	
	string newPath = "C:\\cygwin64\\home\\yali.zhu\\YaliSources\\QuickReviewer\\Debug\\NewlySavedXMLFile.xml";
	doc->SaveFile(newPath.c_str());
	errorID = doc->ErrorID();
	if (errorID) {
		printf("\nXML file '%s' Save failed: ErrorID=%d\n", argv[1], errorID);		
		exit(0);
	}
	printf("\nXML file '%s' Saved successfully: ErrorID=%d\n", newPath.c_str(), errorID);
	return 0;
}
