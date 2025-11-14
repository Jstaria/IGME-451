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
map<int, thread> threads;
int ids;

// First create thread pool and give it something to print out to make sure it works
// Then add heavier random for loop to test working threads
// Thread pool should have a set amt of threads that can be accessible, with queue for overloading

// Test in release
void PrintThreadHello(vector<string> args, int id) {

	int rand = std::rand() * 10;

	// Gen number, if number big enough, split up the threads into multiple and dispatch

	for (int i = 0; i < rand; i++) {
		cout << "Hello from thread: " << id << "!" << endl;
		int rn = std::rand();
	}

	threads.erase(id);
}

void CreateThread(vector<string> args) {

	int number = args.size() == 2 ? stoi(args[1]) : 1;

	for (int i = 0; i < number; i++) {
			threads.emplace(ids, thread(PrintThreadHello, std::move(args), std::move(ids)));
			threads[ids].detach();

			ids++;
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