#include "Command.h"

using namespace std;

Command::Command(void (*func)(string)) : func(func)
{
}

void Command::CallCommand(string call)
{
	func(call);
}
