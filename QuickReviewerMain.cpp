
#include "tinyxml2.h"
#include <cstring>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
using namespace tinyxml2;
using namespace std;
int main(int argc, const char ** argv) 
{
	cout << "Quick Reviewer version 1.0" << endl;
	const char * mydoc = "resources/QASheet.txt";

	XMLDocument* doc = new XMLDocument();          //in tinyxml2 namespace
	doc->LoadFile(mydoc);

	int errorID = doc->ErrorID();
	if (errorID) {
		printf("XML file '%s' loaded and parsed failed: ErrorID=%d\n", mydoc, errorID);
		cin.get();
		exit(0);
	}
	printf("XML file '%s' loaded and parsed successfully\n\n", mydoc);

	int qn = 1;   //question number;
	do {
		char * str = nullptr;
		char * cat = nullptr;
		
		char name[10];
		sprintf_s(name, "Q%d", qn++);
		
		XMLElement* attributeQLib = doc->FirstChildElement(name);
		if (!attributeQLib) {
			break;
		}
		attributeQLib->QueryStringAttribute("type", (const char **)&str);
		attributeQLib->QueryStringAttribute("category", (const char **)&cat);
		
		XMLElement *Que = attributeQLib->FirstChildElement("Question");
		const char * question = nullptr;
		if (Que) {
			question = Que->GetText();
		}
		const char * answer = nullptr;
		Que = attributeQLib->FirstChildElement("Answer");
		if (Que) {
			answer = Que->GetText();
		}
		
		cout << "Question No " << qn - 1 << " :" << endl;
		printf("Question type: %s\n", str);
		printf("Question Category: %s\n", cat);
		printf("Question: %s\n", question);
		
		Que = attributeQLib->FirstChildElement("ChoiceA");
		if (Que) {
			const char * choiceA = Que->GetText();
			printf("Choice A: %s\n", choiceA);
		}
		Que = attributeQLib->FirstChildElement("ChoiceB");
		if (Que) {
			const char * choiceB = Que->GetText();
			printf("Choice B: %s\n", choiceB);
		}
		Que = attributeQLib->FirstChildElement("ChoiceC");
		if (Que) {
			const char * choiceC = Que->GetText();
			printf("Choice C: %s\n", choiceC);
		}
		Que = attributeQLib->FirstChildElement("ChoiceD");
		if (Que) {
			const char * choiceD = Que->GetText();
			printf("Choice D: %s\n", choiceD);
		}
		printf("\n\n\nAnswer: %s\n", answer);

		Que = attributeQLib->FirstChildElement("Explain");
		if (Que) {
			const char * explain = Que->GetText();
			printf("Explain: %s\n", explain);
		}
	} while (true);

	cin.get();
	return 0;
}
