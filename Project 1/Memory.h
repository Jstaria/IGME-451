#pragma once

#include <vector>
#include "Block.h"
#include <map>
#include "Process.h"
#include <iostream>
#include <iomanip>

enum WriteMode { WRITEBACK, WRITETHROUGH };
enum ReplacePolicy { LR, MR, LF, MF };

class Memory
{
public:
	std::vector<Block> frameBlocks;
	std::vector<Block> cacheBlocks;
	std::vector<Block> storeBlocks;

	int bitWidth;
	int blockSize;
	int storeLastUsed = 0;
	int frameLastUsed = 0;

	WriteMode cacheWriteMode = WRITETHROUGH;
	WriteMode memoryWriteMode = WRITETHROUGH;
	ReplacePolicy memoryReplacePolicy = MR;

	int GetReplacementFrame();
	int FindLR();
	int FindMR();
	int FindLF();
	int FindMF();

	void Initialize(int frameSize, int storeSize, int cacheSize);

	void FlushCache(Process* process);
	void LoadFromStore(Process* process, uint8_t logicalAddr);
	void LoadFromMemory(Process* process, uint8_t logicalAddr);
	void CheckLoad(Process* process, uint8_t logicalAddr);
	void WriteToStore(Process *process, uint8_t data);
	void WriteToStore(Block &block, PageTableEntry entry);

	uint8_t ReadByte(Process* process, uint8_t logicalAddr);
	void WriteByte(Process* process, uint8_t logicalAddr, uint8_t data);

	void DeleteProcess(Process* process);
	void PrintCurrentProcessInfo(Process* process);
};

