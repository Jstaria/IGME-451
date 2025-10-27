#pragma once
#include <vector>
#include <string>
#include "PageTableEntry.h"
#include "BackingStore.h"

struct Stats {
	int hits = 0;
	int misses = 0;

	float hitRatio() { return (hits + misses) ? (float)(hits / (hits + misses)) : 0; }
	float missRatio() { return (hits + misses) ? (float)(misses / (hits + misses)) : 0; }
};

class Process
{
public:
	int pid;
	int logicalSize;
	int backingStoreStart;

	std::vector<PageTableEntry> pageTable;

	Stats cacheStats, memoryStats, storeStats;

public:
	Process(int pid, int logicalSize, int pageTableSize) :
		pid(pid), logicalSize(logicalSize) {
		
		pageTable.resize(pageTableSize);;
	}
};

