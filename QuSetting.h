#pragma once
#include <string>
#include "tinyxml2.h"
using namespace tinyxml2;
class QuSetting
{
public:
	QuSetting(int id,int cd, int rt);
	~QuSetting();
//private:
	int id;
	int countdown;
	int resetto;	
};

