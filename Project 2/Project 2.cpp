// Project 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define MAX_THREADS 32

#include <iostream>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <thread>

#include "Command.h"
#include "Helper.h"

using namespace std;

map<string, Command> currentSetOfCommands;
thread threads[MAX_THREADS];
vector<thread::id> threadIDsInUse;
// First create thread pool and give it something to print out to make sure it works
// Then add heavier random for loop to test working threads
// Thread pool should have a set amt of threads that can be accessible, with queue for overloading

// Test in release

void CreateThread(vector<string> args) {
	for (int i = 0; i < MAX_THREADS; i++) {
		auto it = std::find(threadIDsInUse.begin(), threadIDsInUse.end(), threads[i].get_id());
		
		if (it == threadIDsInUse.end()) {
			threads[i] = thread{ PrintThreadHello, args, threads[i].get_id()};
			threads[i].detach();
			threadIDsInUse.push_back(threads[i].get_id());

			break;
		}
	}
}

void PrintThreadHello(vector<string> args, thread::id id) {
	cout << "Hello from thread: " << id << "!" << endl;

	auto it = std::find(threadIDsInUse.begin(), threadIDsInUse.end(), id);

	if (it != threadIDsInUse.end()) {
		threadIDsInUse.erase(it);
	}
}

void PrintHello(vector<string> args) {
	cout << "Hello!" << endl;
}

void SetupCommands() {
	currentSetOfCommands.emplace("p", PrintHello);
	currentSetOfCommands.emplace("c", CreateThread);
}

int main()
{
	SetupCommands();

	std::cout << "----------------------------------------------------------------------\n|                         Thread Simulation                          |\n----------------------------------------------------------------------\n";

	string input;

	vector<std::string> args;

	while (true) {
		cout << "Enter Command: ";
		getline(cin, input);

		args = SplitArgs(input);

		if (currentSetOfCommands.find(args[0]) != currentSetOfCommands.end())
			currentSetOfCommands[args[0]].CallCommand(args);

		if (tolower(input[0]) == 'q')
			break;
	}

	return 0;
}