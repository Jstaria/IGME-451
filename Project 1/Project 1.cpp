// Project 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bit>
#include <map>
#include "Command.h"

using namespace std;

int bitWidth = 1;
int pageWidth = 1;
int cacheWidth = 1;
int frameWidth = 1;
int backStoreWidth = 1;

bool inSim = false;

map<char, Command> beforeSimCommands;
map<char, Command> inSimCommands;

void SetupCommands() {

}

int main()
{
    SetupCommands();

    std::cout << "Hello World!\n";

    std::string input = "";

    while (true) {
        std::cin >> input;

        

        if (input != "yes" || input != "y")
            break;
    }

    return 0;
}

void BitWidth(string command) {
    
}

void Page(string command) {

}

void Cache(string command) {

}

void Frame(string command) {

}

void BackingStore(string command) {

}

void ChacheMode(string command) {

}

void StartSimulation(string command) {

}

void CreateProcess(string command) {

}

void DeleteProcess(string command) {
    DumpInfo(command);
}

void AdvanceToNextProcess(string command) {

}

void DumpInfo(string command) {

}

void VerboseMode(string command) {

}

void ReadLogicalAddress(string command) {

}

void WriteLogicalAddress(string command) {

}