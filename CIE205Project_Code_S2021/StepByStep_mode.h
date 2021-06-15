#pragma once
#include "Battle_mode.h"
class StepByStep_mode :
    public Battle_mode
{

public:
	StepByStep_mode(Battle* b) :Battle_mode(b) {};
	void Run() override;

};

