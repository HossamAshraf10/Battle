#pragma once
#include "Battle_mode.h"

class Interactive_mode:public Battle_mode
{
	//static Interactive_mode* OBJ;
public:

	Interactive_mode(Battle* b);
	//static Interactive_mode* getInstance(Battle* b);
	void Run() override;

};
//Interactive_mode* OBJ;

