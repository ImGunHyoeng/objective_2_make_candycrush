#include "NumberCard.h"

void NumberCard::update(InputManager& input)
{
	static const char* shape1 = "     #    #    #    #    #         ";
	static const char* shape2 = "     ###    #  ###  #    ###       ";
	static const char* shape3 = "     ###    #  ###    #  ###       ";
	static const char* shape4 = "     # #  # #  ###    #    #       ";
	static const char* shape5 = "     ###  #    ###    #  ###       ";

	switch (type)
	{
	case 1:
		if (0 == strcmp(getShape(), shape1)) return;
		
		break;
	case 2:
		if (0 == strcmp(getShape(), shape2))return;
		break;
	case 3:
		if (0 == strcmp(getShape(), shape3))return;
		break;
	case 4:
		if (0 == strcmp(getShape(), shape4))return;
		break;
	case 5:
		if (0 == strcmp(getShape(), shape5))return;
		break;
	}

}

void NumberCard::reTouch(char* shape)
{

}

void NumberCard::change(NumberCard& target)
{
	NumberCard temp = target;
}
