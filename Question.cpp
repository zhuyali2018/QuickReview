#include "Question.h"



Question::Question(int id, std::string type, std::string category, int cd, int rt, std::string question, std::string answer)
	:id(id),qtype(type),qcategory(category),countdown(cd),resetto(rt),question(question),answer(answer)
{
}


Question::~Question()
{
}
