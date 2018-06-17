
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
		bool firstTimeHide = false;   //first time to hide this question, when resetto >0 and countdown is 0
		QA->QueryIntAttribute("ID", &id);
		QA->QueryStringAttribute("type", &qtype);
		QA->QueryStringAttribute("category", &category);
		QA->QueryIntAttribute("countdown", &countdown);
		QA->QueryIntAttribute("resetto", &resetto);
		if (countdown > 0) {
			countdown--;
		}
		else {
			if (resetto > 0)
			   firstTimeHide = true;
		}
		const string question = QA->FirstChildElement("Question")->GetText();
		const string answer = QA->FirstChildElement("Answer")->GetText();

		Question q(id, qtype, category, countdown, resetto, question, answer);
		q.Qc = QA;   //save pointer to QA in order to be able to set resetto
        //now q is properly constructed, ready to be inserted in tovector 
		if (countdown == 0) {
			if (!firstTimeHide)
			    Qs.push_back(q);
			if (resetto > 0) {
				QA->SetAttribute("countdown", resetto);
			}
			else {
				QA->SetAttribute("countdown", countdown);     //if just decrease to 0 and reset was just reset to 0
			}
		}
		else {  //if countdown is not 0 yest, not added for showing
			QA->SetAttribute("countdown", countdown);
		}

		XMLElement * QB = (XMLElement *)QA->NextSibling();
		if (!QB) break;   //if no more questions, done and exit
		QA = QB;
	}
	return Qs;
}
void promptAtQuestion() {
	printf("\n\n\n\n --------------------------------------------\n");
	printf("           Number: jump to question numbered \n");
	printf("            Enter: Next \n");
	printf("        r<number>: skip this question for the next <number> round (default to 3 if number omitted)\n");
	printf("           q or x: Exit \n");
	printf("       Your Input: ");
}
int main(int argc, const char ** argv) 
{
	string newPath = "NewSavedXMLFile.xml";
	cout << "Quick Reviewer version 2.1" << endl;
	
	XMLDocument* doc = new XMLDocument();          //in tinyxml2 namespace
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <questionDataFile>" << endl;
		//return -1;
		doc->LoadFile(newPath.c_str());
		if (doc->ErrorID()) {
			if (doc->ErrorID() == XML_ERROR_FILE_NOT_FOUND) 
			    doc->LoadFile("QASheet.txt");
		    if (!doc->ErrorID()) {
				printf("XML file QASheet.txt loaded and parsed successfully\n\n");
		    }
		}
		else {
			printf("XML file '%s' loaded and parsed successfully\n\n", newPath.c_str());
		}
	}
	else {
		doc->LoadFile(argv[1]);
		if(!doc->ErrorID())
			printf("XML file '%s' loaded and parsed succesfully\n\n", argv[1]);
	}

	int errorID = doc->ErrorID();
	if (errorID) {
		printf("XML file loaded and parsed failed: \n"
			   "                 ErrorID: %d\n"
			   "               Error Msg: %s\n"
			   "              Error Line: %d", errorID,doc->ErrorName()+4, doc->ErrorLineNum());
		//cin.get();
		exit(0);
	}
	
	cin.get();    //press Enter to start
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
		if ((k[0] == 'x') || (k[0] == 'q'))
			break;
		else if (k[0] == 'r') {
			if(k[1]==0)
			  q.Qc->SetAttribute("resetto", 3);
			else {
				char *p = &k[1];
				int n = atoi(p);
				if(n>0 && n<999)
				  q.Qc->SetAttribute("resetto", n);
			}
		}
	}
	
	doc->SaveFile(newPath.c_str());
	errorID = doc->ErrorID();
	if (errorID) {
		printf("\nXML file '%s' Save failed: ErrorID=%d\n", argv[1], errorID);		
		exit(0);
	}
	printf("\nXML file '%s' Saved successfully: ErrorID=%d\n", newPath.c_str(), errorID);
	return 0;
}
