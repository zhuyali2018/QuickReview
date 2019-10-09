
#include "tinyxml2.h"
#include <cstring>
#include <iostream>
#include <istream>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <array>

#include "Question.h"
#include "QuSetting.h"

using namespace tinyxml2;
using namespace std;

void searchAndDisplayByQ2Words(char * word, vector<Question> questions);
void searchAndDisplayByQuestions(char * word, vector<Question> questions);
void searchAndDisplayByAnswers(char * word, vector<Question> questions);
void searchAndDisplayByQAs(char * word, vector<Question> questions);
unsigned pagesize = 5;
static inline void trim(std::string &s);
// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}
map<int, QuSetting> GetAllQuestionConfigs(XMLElement * parent) {
	if (!parent) {
		throw NoDataException();
	}
	map<int, QuSetting> Qc;
	XMLElement* QA = parent->FirstChildElement("QA");;
	while (true) {
		int id;
		int countdown;
		int resetto;
		QA->QueryIntAttribute("ID", &id);
		QA->QueryIntAttribute("countdown", &countdown);
		QA->QueryIntAttribute("resetto", &resetto);
		QuSetting q(id, countdown, resetto);
		Qc.insert(make_pair(id, q));
		XMLElement * QB = (XMLElement *)QA->NextSibling();
		if (!QB) break;   //if no more questions, done and exit	
		QA = QB;
	}
	return Qc;
}
vector<Question> GetAllQuestions(XMLElement * parent, XMLElement * parent1) {
	if (!parent) {
		throw NoDataException();
	}
	map<int,QuSetting> Qc=GetAllQuestionConfigs(parent1);
	printf("config file nodes: %d\n",(int)Qc.size());
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

		map<int, QuSetting>::iterator itr = Qc.find(id);
		if (itr != Qc.end()) {
			QuSetting Qcfg = itr->second;
			countdown = Qcfg.countdown;
			resetto = Qcfg.resetto;
			QA->SetAttribute("resetto", resetto);    //if found, always reset reseto
		}
		else {
			printf("********Config file not found Qid:%d\n",id);
		}

		if (countdown > 0) {
			countdown--;
		}
		else {
			if (resetto > 0)
			   firstTimeHide = true;
		}
		string question;
		XMLElement * questionElement = QA->FirstChildElement("Question");
		if (questionElement) {
			if (questionElement->GetText() != NULL) {
				question = questionElement->GetText();
			}
		}
		XMLElement * answerElement = QA->FirstChildElement("Answer");
		string answer;
		if (answerElement) {
			if (answerElement->GetText() != NULL) {
				answer = answerElement->GetText();
			}
		}
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
		string question;
		XMLElement * questionElement = QA->FirstChildElement("Question");
		if (questionElement) {
			if (questionElement->GetText() != NULL) {
				question = questionElement->GetText();
			}
		}
		XMLElement * answerElement = QA->FirstChildElement("Answer");
		string answer;
		if (answerElement) {
			if (answerElement->GetText() != NULL) {
				answer = answerElement->GetText();
			}
		}
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
	printf("        r<number>: skip this question for the next <number> rounds (default to 3 if number omitted)\n");
	printf("                R: reload data \n");
	printf("          s<word>: search questions \n");
	printf("          a<word>: search Answers \n");
	printf("          b<word>: search both questions and answers \n");
	printf("                l: list questions \n");
	printf("          c<word words>: search questions with 2 separate words \n");
	printf("                example: creplace in vi \n");
	printf("        p<number>: Specify the nubmer of questions per page \n");
	printf("           q or x: Exit \n");
	printf("       Your Input: ");
}
void browse(vector<Question> questions);

int main(int argc, const char ** argv) 
{
	bool applyConfigfile = false;
	string newPath = "NewSavedXMLFile.xml";
	cout << "\n Quick Reviewer version 3.8" << endl << endl;
	bool reload = true;
	XMLDocument* doc = nullptr;
	while (reload) {
		reload = false;
	   doc = new XMLDocument();          //in tinyxml2 namespace
	   XMLDocument* docCfg = nullptr;    //For Doc Question Settings only

		if (argc == 1) {
			cout << "\n Usage: " << argv[0] << " <questionDataFile> [oldQuestionDataFile]" << endl;
			cout << "\n Note: oldQuestionDataFile for question rotation settings only" << endl << endl;
			//return -1;
			doc->LoadFile(newPath.c_str());
			if (doc->ErrorID()) {
				if (doc->ErrorID() == XML_ERROR_FILE_NOT_FOUND)
					doc->LoadFile("QASheet.txt");
				if (!doc->ErrorID()) {
					printf("XML file QASheet.txt loaded and parsed successfully\n\n");
				}
				else {
					printf("XML file '%s' and QASheet.txt loading or parsing failed\n\n", newPath.c_str());
				}
			}
			else {
				printf("XML file '%s' loaded and parsed successfully\n\n", newPath.c_str());
			}
		}
		else {
			doc->LoadFile(argv[1]);
			if (!doc->ErrorID()) {
				printf("XML file '%s' loaded and parsed succesfully\n\n", argv[1]);
				//loading config file
				if (argc == 3) {   //only when the first file loaded successfully
					docCfg = new XMLDocument();       //For Doc Question Settings only
					docCfg->LoadFile(argv[2]);
					int errorID = docCfg->ErrorID();
					if (errorID) {
						printf("XML file %s loading or parsing failed: \n"
							"              Error   ID: %d\n"
							"              Error  Msg: %s\n"
							"              Error Line: %d\n", argv[3], errorID, doc->ErrorName() + 4, doc->ErrorLineNum());
					}
					else {
						printf("Question config file '%s' loaded and parsed succesfully\n\n", argv[2]);
						applyConfigfile = true;   //set the configfile flag
					}
				}
			}
		}

		int errorID = doc->ErrorID();
		if (errorID) {
			printf("XML file loaded and parsed failed: \n"
				"              Error   ID: %d\n"
				"              Error  MSg: %s\n"
				"              Error Line: %d\n", errorID, doc->ErrorName() + 4, doc->ErrorLineNum());
			//cin.get();
			exit(0);
		}

		cin.get();    //press Enter to start
		//-----------------------------------------------------------------------------
		const char * name = "QuestionsAndAnswers";
		XMLElement* root = doc->FirstChildElement(name);
		XMLElement* root1 = nullptr;  //for configfile
		vector<Question> questions;

		if (applyConfigfile) {
			root1 = docCfg->FirstChildElement(name);
			questions = GetAllQuestions(root, root1);
		}
		else {
			questions = GetAllQuestions(root);
		}
		//cin.get();
		//loop through each questions
		char k[32] = { 0 };
		int questionNo = 1;
		unsigned qno = 0;
		int total = questions.size();
		//for (Question q : questions) {
		while (true) {
			if ((qno > 0) && (qno < questions.size()))
				questionNo = qno;
			Question &q = questions[questionNo];
			qno = 0;  //reset goto question No
			system("clear");   //clear the screen of command line window
			printf("[Qid:%d]  Question %d/%d : \n%s\n", q.id, questionNo++, total, q.question.c_str());
			cin.getline(k, 30);       //wait before show answer

			printf("Answer : \n%s\n", q.answer.c_str());
			promptAtQuestion();    //show options to exit or job to another questions

			cin.getline(k, 30);
			qno = atoi(k);
			if ((k[0] == 'x') || (k[0] == 'q'))   //out of loop to exit
				break;
			else if (k[0] == 'r') {               //skip specified number of rounds, rarely used feature
				if (k[1] == 0)
					q.Qc->SetAttribute("resetto", 3);
				else {
					char *p = &k[1];
					int n = atoi(p);
					if (n > 0 && n < 999)           //range is up to 999
						q.Qc->SetAttribute("resetto", n);
				}
			}
			else if (k[0] == 'R') {   //reload 
				delete doc;
				reload = true;
				break;
			}
			else if (k[0] == 'l') {   //list all questions				
				browse(questions);				
			}
			else if (k[0] == 'p') {   //list all questions by page, size is set here
				pagesize = atoi(k + 1);
				if (pagesize < 5)
					pagesize = 5;
			}
			else if (k[0] == 's') {   //search questions	
				searchAndDisplayByQuestions(k + 1, questions);				
			}
			else if (k[0] == 'a') {   //search Answers
				searchAndDisplayByAnswers(k + 1, questions);
			}
			else if (k[0] == 'b') {   //search questions and answers	
				searchAndDisplayByQAs(k + 1, questions);
			}
			else if (k[0] == 'c') {   //search questions with 2 key words	
				searchAndDisplayByQ2Words(k + 1, questions);
			}
		}
		if (!reload)
			break;
	}
	
	doc->SaveFile(newPath.c_str());
	int errorID = doc->ErrorID();
	if (errorID) {
		printf("\nXML file '%s' Save failed: ErrorID=%d\n", argv[1], errorID);		
		exit(0);
	}
	printf("\nXML file '%s' Saved successfully: ErrorID=%d\n", newPath.c_str(), errorID);
	return 0;
}

void display(vector<Question>::iterator it) {
	string question = it->question;
	trim(question);
	cout << right << setw(4) << it->id << " " << question.c_str() ;
	cout << endl;
}
void searchAndDisplayByQuestions(char * word, vector<Question> questions) {
	//search using phone number and list the matched records
	for (vector<Question>::iterator it = questions.begin(); it != questions.end(); ++it) {		
		std::size_t found = it->question.find(word);
		if (found != std::string::npos) {
			display(it);
		}
	}
	cin.get();
}
void searchAndDisplayByQ2Words(char * word, vector<Question> questions) {
	//search for 2 words 
   char * p = word;  //moving pointer to scan the words line
   char * rd=NULL;   //for second word
   while (*p!=0){
      if ( *p == ' '){
          *p=0;
          rd=p+1;
          break;
      }
      p++;
   }
   if (rd != NULL && *rd !=0)
      cout << endl << right << setw(4) << "       2 words used :" << word << " 2rd: " << rd << endl;
   else{
      cout << endl << "   Error: did not find second word" << endl;
      cin.get();
      return;
   }
   cout << endl;

	for (vector<Question>::iterator it = questions.begin(); it != questions.end(); ++it) {		
		std::size_t found1 = it->question.find(word);
		std::size_t found2 = it->question.find(rd);
      if ((found1 != std::string::npos)&&(found2 != std::string::npos)) {
         display(it);
      }
	}
	cin.get();
}
void searchAndDisplayByAnswers(char * word, vector<Question> questions) {
	//search using phone number and list the matched records
	for (vector<Question>::iterator it = questions.begin(); it != questions.end(); ++it) {		
		std::size_t found = it->answer.find(word);
		if (found != std::string::npos) {
			display(it);
		}
	}
	cin.get();
}
void searchAndDisplayByQAs(char * word, vector<Question> questions) {
	//search using phone number and list the matched records
	for (vector<Question>::iterator it = questions.begin(); it != questions.end(); ++it) {
		std::size_t found1 = it->question.find(word);
		std::size_t found2 = it->answer.find(word);
		
		if ((found1 != std::string::npos)|| (found2 != std::string::npos)) {
			display(it);
		}
	}
	cin.get();
}
void browse(vector<Question> questions) {
	//search using phone number and list the matched records
	char k[32] = { 0 };
	auto i= 0;
	for (vector<Question>::iterator it = questions.begin(); it != questions.end(); ++it) {
		display(it);
		if (i >= pagesize) {
			while (true) {
				cin.getline(k, 30);
				if (k[0] == 'q')
					return;
				else if (k[0] == 'g') {
					int id = atoi(k + 1);
					it = it + id;
					break;
				}
				else if (k[0] == 'p') {   //list all questions				
					pagesize = atoi(k + 1);
					if (pagesize < 5)
						pagesize = 5;
					break;
				}
				else if (k[0] == 'i') {   //go to id			
					unsigned questionNo = atoi(k + 1);
					Question &q = questions[questionNo];
					system("CLS");   //clear the screen of command line window
					printf("[Qid:%d]  Question %d : \n%s\n", q.id, questionNo++, q.question.c_str());
					printf("Answer : \n%s\n", q.answer.c_str());
					it -= pagesize;     //after examin the record, the list postion should keep the same
				}
				else {
					break;
				}
			}
			i = 0;
		}
		++i;
	}
	cin.get();
}
