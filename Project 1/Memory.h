#pragma once

#include <vector>
#include "Block.h"
#include <map>
#include "Process.h"

enum WriteMode { WRITEBACK, WRITETHROUGH };
enum ReplacePolicy { LR, MR, LF, MF };

class Memory
{
public:
	std::vector<Block> memoryBlocks;
	std::vector<Block> cacheBlocks;
	std::vector<Block> storeBlocks;

	WriteMode cacheWriteMode = WRITETHROUGH;
	WriteMode memoryWriteMode = WRITETHROUGH;
	ReplacePolicy memoryReplacePolicy = MR;

	int GetReplacementFrame();
	int FindLR();
	int FindMR();
	int FindLF();
	int FindMF();

	void FlushCache(int pid);
	int LoadFromStore(int pid, int pageNum);
	void WriteToStore(Process *process, int bitWidth);

	uint8_t ReadByte(PageTableEntry entry, int offset, int bitWidth);
	void WriteByte(PageTableEntry entry, int offset, int bitWidth);
};

