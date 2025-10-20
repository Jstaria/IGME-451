#pragma once
#include <vector>
#include <string>

__interface ICommand
{
public:
	void CallCommand(std::vector<std::string> args);
};

