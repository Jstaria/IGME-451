#pragma once
#include "ICommand.h"
class Command: public ICommand {
public:
	Command(void (*func)(std::string));
	void CallCommand(std::string call) override;
private:
	void (*func)(std::string call);
};