#pragma once
#include "ICommand.h"
#include <vector>
class Command: public ICommand {
public:
	Command();
	Command(void (*func)(std::vector<std::string>));
	void CallCommand(std::vector<std::string> args) override;
private:
	static void Empty(std::vector<std::string> args);
	void (*func)(std::vector<std::string>);
};