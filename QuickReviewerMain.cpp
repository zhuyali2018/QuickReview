
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
	
	do {
		char * str = nullptr;
		char * cat = nullptr;
		
		char name[10];
	
		XMLElement* attributeQLib = doc->FirstChildElement(name);
		if (!attributeQLib) {
			break;
		}
		attributeQLib->QueryStringAttribute("type", (const char **)&str);
		attributeQLib->QueryStringAttribute("category", (const char **)&cat);
		
	} while (true);

	cin.get();
	return 0;
}
