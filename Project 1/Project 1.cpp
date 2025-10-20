// Project 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include "Command.h"
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <sstream>
#include <algorithm>

using namespace std;

int bitWidth = 1;
int pageWidth = 1;
int cacheWidth = 1;
int frameWidth = 1;
int backStoreWidth = 1;

string cacheMode = "t";
string ramMode = "t";
string ramPolicy = "lf";

bool inSim = false;

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

    cacheWidth = number;
}

void Frame(std::vector<std::string> args) {
    if (args.size() < 2) {
        cout << "Must have a size" << endl;
        return;
    }
    string num = args[1];
    int number = atoi(num.c_str());
    int width = pow(2, number);
    cout << "Chnaged # of Frames to: " << width << endl;

    frameWidth = number;
}

void BackingStore(std::vector<std::string> args) {
    if (args.size() < 2) {
        cout << "Must have a size" << endl;
        return;
    }
    string num = args[1];
    int number = atoi(num.c_str());
    int width = pow(2, number);
    cout << "Chnaged # of Backing Stores to: " << width << endl;

    backStoreWidth = number;
}

void Mode(std::vector<std::string> args) {
    if (args.size() < 2) {
        cout << "Must have arguments" << endl;
        return;
    }
    string mode = ToLower(args[1]);
    
    if (mode == "c") {
        
        if (ToLower(args[2]) == "b")
            cout << "Changed Cache Mode to: Write Back" << endl;

        if (ToLower(args[2]) == "t")
            cout << "Changed Cache Mode to: Write Through" << endl;

    }

    else if (mode == "r") {

        string rMode = ToLower(args[2]);

        if (rMode == "b")
            cout << "Changed RAM Mode to: Write Back" << endl;

        if (rMode == "t")
            cout << "Changed RAM Mode to: Write Through" << endl;

        ramMode = rMode;

        if (args.size() < 4) return;

        string rPolicy = ToLower(args[3]);

        if (rPolicy == "lr")
            cout << "Changed Replacement Policy to: Least Recently Used" << endl;

        if (rPolicy == "mr")
            cout << "Changed Replacement Policy to: Most Recently Used" << endl;

        if (rPolicy == "mf")
            cout << "Changed Replacement Policy to: Most Frequently Used" << endl;

        if (rPolicy == "lf")
            cout << "Changed Replacement Policy to: Least Frequently Used" << endl;

        ramPolicy = rPolicy;
    }
}

void StartSimulation(std::vector<std::string> args) {
    cout << "Start Simulation!" << endl;
}
#pragma endregion

#pragma region AfterSim
void CreateProcess(std::vector<std::string> args) {
    cout << "CreatedProcess" << endl;
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
    cout << "Verbose mode" << endl;
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
    beforeSimCommands.emplace("f", Command(Frame));
    beforeSimCommands.emplace("s", Command(BackingStore));
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

    std::cout << "Hello World!\n";

    string input;
  
    vector<std::string> args;

    while (true) {
        
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