#define MAX_THREADS 1024
#define SPLIT_THRESHOLD 25000000

#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "Command.h"
#include "Helper.h"

using namespace std;

// ----------------------------------------------------------------
// GLOBAL STATE
// ----------------------------------------------------------------

map<string, Command> currentSetOfCommands;
map<string, atomic<int>> threadGroupCount;

static atomic<int> globalIds{ 0 };
static atomic<int> totalActive{ 0 };
static atomic<int> currentThreadCount{ 0 };

std::mutex groupMapMutex;

// ----------------------------------------------------------------
// THREADED WORK
// ----------------------------------------------------------------

void OneThread(vector<string> args, int id, int workload) {
    string group = args[1];

    threadGroupCount[group]++;
    totalActive++;
    currentThreadCount++;

    int iterations = workload;

    for (int i = 0; i < iterations; ++i)
    {
        int j = rand();
        int k = rand();
        volatile int l = j + k;
    }

    cout << "Thread " << id << " finished in group " << group << endl;

    threadGroupCount[group]--;
    totalActive--;
    currentThreadCount--;
}

void ThreadWork(vector<string> args, int id, int workload)
{
    string group = args[1];

    threadGroupCount[group]++;
    totalActive++;
    currentThreadCount++;

    if (workload > SPLIT_THRESHOLD && currentThreadCount.load() + 2 <= MAX_THREADS)
    {
        int half = workload / 2;

        // spawn two children
        int newId1 = globalIds++;
        int newId2 = globalIds++;

        std::thread(ThreadWork, args, newId1, half).detach();
        std::thread(ThreadWork, args, newId2, half).detach();
    }
    else
    {
        int iterations = workload;

        for (int i = 0; i < iterations; ++i)
        {
            int j = rand();
            int k = rand();
            volatile int l = j + k;
        }

        cout << "Thread " << id << " finished in group " << group << endl;
    }

    threadGroupCount[group]--;
    totalActive--;
    currentThreadCount--;
}

// ----------------------------------------------------------------
// SPAWN FUNCTION
// ----------------------------------------------------------------

void SpawnThread(const vector<string>& args, int workload)
{
    string group = args[1];

    {
        lock_guard<mutex> lock(groupMapMutex);
        if (threadGroupCount.find(group) == threadGroupCount.end())
            threadGroupCount.emplace(group, 0);
    }

    if (currentThreadCount.load() >= MAX_THREADS)
    {
        cerr << "MAX_THREADS reached. Ignoring new thread request.\n";
        return;
    }

    int id = globalIds++;

    std::thread(ThreadWork, args, id, workload).detach();
}

void SpawnOneThread(const vector<string>& args, int workload)
{
    string group = args[1];

    {
        lock_guard<mutex> lock(groupMapMutex);
        if (threadGroupCount.find(group) == threadGroupCount.end())
            threadGroupCount.emplace(group, 0);
    }

    if (currentThreadCount.load() >= MAX_THREADS)
    {
        cerr << "MAX_THREADS reached. Ignoring new thread request.\n";
        return;
    }

    int id = globalIds++;

    std::thread(OneThread, args, id, workload).detach();
}
// ----------------------------------------------------------------
// CREATE THREAD COMMAND
// ----------------------------------------------------------------

void CreateOneThread(vector<string> args) {
    if (args.size() < 2)
    {
        cout << "Usage: c <groupName> [workload]\n";
        return;
    }

    int workload = 1000000000;
    if (args.size() >= 3)
    {
        try { workload = stoi(args[2]); }
        catch (...) { workload = 100000000; }
    }

    SpawnOneThread(args, workload);

    cout << "| Threads Active |" << endl << endl;

    while (true)
    {
        {
            lock_guard<mutex> lock(groupMapMutex);
            for (auto& pair : threadGroupCount)
            {
                cout << pair.first << ": " << pair.second.load() << endl;
            }
        }

        cout << "\nTotal active threads: " << totalActive.load() << endl;
        cout << "Current thread count: " << currentThreadCount.load() << endl;

        if (totalActive.load() == 0)
            break;

        this_thread::sleep_for(chrono::milliseconds(400));
    }

    cout << "All threads in this run finished.\n";
}

void CreateThread(vector<string> args)
{
    if (args.size() < 2)
    {
        cout << "Usage: c <groupName> [workload]\n";
        return;
    }

    int workload = 1000000000;
    if (args.size() >= 3)
    {
        try { workload = stoi(args[2]); }
        catch (...) { workload = 100000000; }
    }

    SpawnThread(args, workload);

    cout << "| Threads Active |" << endl << endl;

    while (true)
    {
        {
            lock_guard<mutex> lock(groupMapMutex);
            for (auto& pair : threadGroupCount)
            {
                cout << pair.first << ": " << pair.second.load() << endl;
            }
        }

        cout << "\nTotal active threads: " << totalActive.load() << endl;
        cout << "Current thread count: " << currentThreadCount.load() << endl;

        if (totalActive.load() == 0)
            break;

        this_thread::sleep_for(chrono::milliseconds(400));
    }

    cout << "All threads in this run finished.\n";
}

// ----------------------------------------------------------------
// OTHER COMMANDS
// ----------------------------------------------------------------

void PrintHello(vector<string> args)
{
    cout << "Hello!" << endl;
}

void SetupCommands()
{
    currentSetOfCommands.emplace("p", PrintHello);
    currentSetOfCommands.emplace("c", CreateThread);
    currentSetOfCommands.emplace("co", CreateOneThread);
}

// ----------------------------------------------------------------
// MAIN
// ----------------------------------------------------------------

int main()
{
    srand((unsigned)time(nullptr));

    SetupCommands();

    cout <<
        "----------------------------------------------------------------------\n"
        "|                         Thread Simulation                          |\n"
        "----------------------------------------------------------------------\n\n"
        "c <Name> <WorkLoad (0-MaxInt)> \n// Creates a thread that will request to split\n"
        "co <Name> <WorkLoad (0-MaxInt)> \n// Creates a single thread that tries to do all the work by itself (To show the difference)\n\n";


    string input;
    vector<string> args;

    while (true)
    {
        cout << "Enter Command: ";
        getline(cin, input);

        args = SplitArgs(input);
        if (args.empty()) continue;

        if (tolower(args[0][0]) == 'q')
            break;

        auto it = currentSetOfCommands.find(args[0]);
        if (it != currentSetOfCommands.end())
            it->second.CallCommand(args);
        else
            cout << "Unknown command.\n";
    }

    return 0;
}
