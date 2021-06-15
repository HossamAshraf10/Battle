#pragma once
#include "Battle_mode.h"
class Silent_mode :
    public Battle_mode
{
public:
	Silent_mode(Battle* b) :Battle_mode(b) {};
	void Run() override;

};

