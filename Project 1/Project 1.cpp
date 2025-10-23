// Project 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Command.h"
#include "Memory.h"
#include "Process.h"
#include "BackingStore.h"

using namespace std;

int bitWidth, pageWidth, cacheWidth, frameWidth, backStoreWidth;
int blockSize, numPages, numCacheBlocks, numFrames, numStoreBlocks;

bool verboseMode = false;
bool inSim = false;

Process* currentProcess;
vector<Process*> processList;

Memory memory;
BackingStore backingStore;

map<string, Command> beforeSimCommands;
map<string, Command> inSimCommands;

map<string, Command> currentSetOfCommands;

#pragma region MyRegion

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

std::vector<std::string> SplitArgs(const std::string& input)
{
	std::istringstream iss(input);
	std::vector<std::string> args;
	std::string token;
	while (iss >> token)
		args.push_back(ToLower(token));
	return args;
}

#pragma region BeforeSim
void BitWidth(std::vector<std::string> args) {
	if (args.size() < 2) {
		cout << "Must have a size" << endl;
		return;
	}

	string num = args[1];
	int number = atoi(num.c_str());
	int width = pow(2, number);
	cout << "Changed byte size to: " << width << endl;

	blockSize = width;
	bitWidth = number;
}

void Page(std::vector<std::string> args) {
	if (args.size() < 2) {
		cout << "Must have a size" << endl;
		return;
	}
	string num = args[1];
	int number = atoi(num.c_str());
	int width = pow(2, number);
	cout << "Chnaged # of Pages to: " << width << endl;

	numPages = width;
	pageWidth = number;
}

void Cache(std::vector<std::string> args) {
	if (args.size() < 2) {
		cout << "Must have a size" << endl;
		return;
	}
	string num = args[1];
	int number = atoi(num.c_str());
	int width = pow(2, number);
	cout << "Chnaged # of Caches to: " << width << endl;

	numCacheBlocks = width;
	cacheWidth = number;
}

void CreateFrame(std::vector<std::string> args) {
	if (args.size() < 2) {
		cout << "Must have a size" << endl;
		return;
	}
	string num = args[1];
	int number = atoi(num.c_str());
	int width = pow(2, number);
	cout << "Chnaged # of Frames to: " << width << endl;

	numFrames = width;
	frameWidth = number;
}

void CreateBackingStore(std::vector<std::string> args) {
	if (args.size() < 2) {
		cout << "Must have a size" << endl;
		return;
	}
	string num = args[1];
	int number = atoi(num.c_str());
	int width = pow(2, number);
	cout << "Chnaged # of Backing Stores to: " << width << endl;

	numStoreBlocks = width;
	backStoreWidth = number;
}

void Mode(std::vector<std::string> args) {
	if (args.size() < 2) {
		cout << "Must have arguments" << endl;
		return;
	}
	string mode = ToLower(args[1]);
	WriteMode wm = WRITETHROUGH;
	ReplacePolicy rp = MR;

	if (mode == "c") {

		if (ToLower(args[2]) == "b")
		{
			cout << "Changed Cache Mode to: Write Back" << endl;
			wm = WRITEBACK;
		}

		if (ToLower(args[2]) == "t")
		{
			cout << "Changed Cache Mode to: Write Through" << endl;
		}

		memory.cacheWriteMode = wm;
	}

	else if (mode == "r") {

		string rMode = ToLower(args[2]);

		if (rMode == "b")
		{
			cout << "Changed RAM Mode to: Write Back" << endl;
			wm = WRITEBACK;
		}

		if (rMode == "t")
		{
			cout << "Changed RAM Mode to: Write Through" << endl;
		}



		if (args.size() < 4) return;

		string rPolicy = ToLower(args[3]);

		if (rPolicy == "lr")
		{
			cout << "Changed Replacement Policy to: Least Recently Used" << endl;
			rp = LR;
		}

		if (rPolicy == "mr")
		{
			cout << "Changed Replacement Policy to: Most Recently Used" << endl;
			rp = MR;
		}

		if (rPolicy == "mf")
		{
			cout << "Changed Replacement Policy to: Most Frequently Used" << endl;
			rp = MF;
		}

		if (rPolicy == "lf")
		{
			cout << "Changed Replacement Policy to: Least Frequently Used" << endl;
			rp = LF;
		}

		memory.memoryWriteMode = wm;
		memory.memoryReplacePolicy = rp;
	}
}

void StartSimulation(std::vector<std::string> args) {
	cout << "Start Simulation!" << endl;
}
#pragma endregion

#pragma region AfterSim
void CreateProcess(std::vector<std::string> args) {
	if (args.size() < 3) return;
	
	int pid = stoi(args[1]);
	int bytes = stoi(args[2]);

	for (auto& p : processList) {
		if (p->pid == pid) {
			cout << "Process ID already in use!" << endl;
			return;
		}
	}

	int pageCount = (bytes + blockSize - 1) / blockSize;
	int logicalSize = pageCount * blockSize;

	currentProcess = new Process(pid, logicalSize, pageCount, backingStore);
	
	processList.push_back(currentProcess);

	cout << "Created Process" << endl;
}

void DumpInfo(std::vector<std::string> args) {
	cout << "Dumped Info" << endl;
}

void DeleteProcess(std::vector<std::string> args) {
	DumpInfo(args);

	cout << "DeletedProcess" << endl;
}

void AdvanceToNextProcess(std::vector<std::string> args) {
	cout << "Advanced to next Process" << endl;
}

void VerboseMode(std::vector<std::string> args) {

	verboseMode = !verboseMode;

	cout << "Verbose Toggled: " << verboseMode << endl;
}

void ReadLogicalAddress(std::vector<std::string> args) {
	cout << "Read Address" << endl;
}

void WriteLogicalAddress(std::vector<std::string> args) {
	cout << "Wrote Address" << endl;
}
#pragma endregion

void SetupCommands() {
	beforeSimCommands.emplace("b", Command(BitWidth));
	beforeSimCommands.emplace("p", Command(Page));
	beforeSimCommands.emplace("c", Command(Cache));
	beforeSimCommands.emplace("f", Command(CreateFrame));
	beforeSimCommands.emplace("s", Command(CreateBackingStore));
	beforeSimCommands.emplace("m", Command(Mode));
	beforeSimCommands.emplace("z", Command(StartSimulation));

	inSimCommands.emplace("c", Command(CreateProcess));
	inSimCommands.emplace("d", Command(DeleteProcess));
	inSimCommands.emplace("n", Command(AdvanceToNextProcess));
	inSimCommands.emplace("i", Command(DumpInfo));
	inSimCommands.emplace("v", Command(VerboseMode));
	inSimCommands.emplace("r", Command(ReadLogicalAddress));
	inSimCommands.emplace("w", Command(WriteLogicalAddress));

	currentSetOfCommands = beforeSimCommands;
}

#pragma endregion

int main()
{
	SetupCommands();

	std::cout << "----------------------------------------------------------------------\n|                         Memory Simulation                          |\n----------------------------------------------------------------------\n";

	string input;

	vector<std::string> args;

	while (true) {
		cout << "Enter Command: ";
		getline(cin, input);

		args = SplitArgs(input);

		currentSetOfCommands[args[0]].CallCommand(args);

		if (tolower(input[0]) == 'z')
			currentSetOfCommands = inSimCommands;

		if (tolower(input[0]) == 'q')
			break;
	}

	return 0;
}