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
#include <queue>

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
vector<Process*> processQueue;
int processQueueIndex;
vector<int> pidList;

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

	memory.bitWidth = bitWidth;
	memory.blockSize = blockSize;
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

	for (auto& p : pidList) {
		if (p == pid) {
			cout << "Process ID already in use!" << endl;
			return;
		}
	}

	pidList.push_back(pid);

	// In class notes:
	// Each part of memory is made up of the same type of block
	// When created, you allocate a page, no frame allocated right now since we arent loading it, and load into backing store
	// C 1 7 => 
	// bitwidth is 2
	// Data is 1, the store blocks in use are then filled with this data
	// 2 page, 2 frame, 2 backing store blocks
	// Put backing store check for leftover size

	// Each process can have a table that maps cache to frame
	// R 2 => Nothing is loaded yet (frame is -1) => Page 0 is in backing store 0 => RAM retrieves next frame and sets data => 
	// Load into cache using same data and set it to inUse => Update page table with cache and frame blocks => Update block counters
	// R 6 => Not loaded => Pull backing store location into ram => RAM gets next frame and sets data => Load into cache => Set rest ^

	// N => All of cache gets reset
	// R 1 && R 5 => Repeat => Update counters => Set cache => Set all none in use data bits to FF

	// Create => put it into backing store => try to load => put it in frame => put it in cache

	int pageCount = (bytes + blockSize - 1) / blockSize;
	int logicalSize = pageCount * blockSize;

	// Set current process if its the first new one (ie use a queue and the top is current)

	processQueue.push_back(new Process(pid, logicalSize, pageCount));
	memory.WriteToStore(processQueue.back(), 0xFF);

	if (!currentProcess) currentProcess = processQueue.front();

	cout << "Created Process" << endl;
}

void DumpInfo(std::vector<std::string> args) {
	cout << "Dumped Info" << endl;

	memory.PrintCurrentProcessInfo(currentProcess);
}

void DeleteProcess(std::vector<std::string> args) {
	DumpInfo(args);

	cout << "DeletedProcess" << endl;

	int pidToRemove = currentProcess->pid;

	pidList.erase(std::remove_if(pidList.begin(), pidList.end(),
		[pidToRemove](int p) {
			return p == pidToRemove;
		}),
		pidList.end());

	memory.DeleteProcess(currentProcess);
	processQueue.erase(
		std::remove_if(processQueue.begin(), processQueue.end(),
			[pidToRemove](Process* p) {
				return p->pid == pidToRemove;
			}),
		processQueue.end()
	);
	processQueueIndex--;
}

void AdvanceToNextProcess(std::vector<std::string> args) {
	if (processQueue.size() == 0) {
		cout << "Nothing in queue!" << endl;
		return;
	}
	
	cout << "Advanced to next Process" << endl;
	
	memory.FlushCache(currentProcess);
	processQueueIndex = (processQueueIndex + 1) % processQueue.size();
	currentProcess = processQueue[processQueueIndex];
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
		{
			currentSetOfCommands = inSimCommands;
			memory.Initialize(numFrames, numStoreBlocks, numCacheBlocks);
		}


		if (tolower(input[0]) == 'q')
			break;
	}

	return 0;
}