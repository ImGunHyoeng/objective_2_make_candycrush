#include "NumberCard.h"



void NumberCard::reTouch(const char* shape)
{
	setShape(shape);
}

void NumberCard::change(int type)
{
	static const char* shape1 = "     #    #    #    #    #         ";
	static const char* shape2 = "     ###    #  ###  #    ###       ";
	static const char* shape3 = "     ###    #  ###    #  ###       ";
	static const char* shape4 = "     # #  # #  ###    #    #       ";
	static const char* shape5 = "     ###  #    ###    #  ###       ";
	
	setType(type+1);
	switch (type)
	{
	case 0:
		reTouch(shape1);
		break;
	case 1:
		reTouch(shape2);
		break;
	case 2:
		reTouch(shape3);
		break;
	case 3:
		reTouch(shape4);
		break;
	case 4:
		reTouch(shape5);
		break;
	}
	setVisible();
}


