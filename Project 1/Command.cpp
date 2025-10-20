#include "Command.h"

using namespace std;

Command::Command() { func = Empty; }
Command::Command(void (*func)(vector<std::string>)) : func(func)
{
}

void Command::Empty(std::vector<std::string> args) {}

void Command::CallCommand(vector<std::string> args)
{
	if (args.size() == 0) return;
	if (func != nullptr)
		func(args);
}
