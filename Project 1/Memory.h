#pragma once

#include <vector>
#include "CacheBlock.h"
#include "Frame.h"

enum WriteMode { WRITEBACK, WRITETHROUGH };
enum ReplacePolicy { LR, MR, LF, MF };

class Memory
{
public:
	std::vector<Frame> memory;
	std::vector<CacheBlock> cache;

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
	int WriteToStore(int pid, int pageNum, int frame);
};

