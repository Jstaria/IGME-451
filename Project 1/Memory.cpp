#include "Memory.h"

int Memory::GetReplacementFrame()
{
	switch (memoryReplacePolicy) {
	case LR: return FindLR();
	case MR: return FindMR();
	case LF: return FindLF();
	case MF: return FindMF();
	}
}

#pragma region Replacement
int Memory::FindLR()
{
	int count = 99999999999;
	int fid = 0;

	for (int i = 0; i < memoryBlocks.size();i++) {
		if (memoryBlocks[i].lastUsed < count) {
			count = memoryBlocks[i].lastUsed;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindMR()
{
	int count = 0;
	int fid = 0;

	for (int i = 0; i < memoryBlocks.size(); i++) {
		if (memoryBlocks[i].lastUsed >= count) {
			count = memoryBlocks[i].lastUsed;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindLF()
{
	int count = 999999999;
	int fid = 0;

	for (int i = 0; i < memoryBlocks.size(); i++) {
		if (memoryBlocks[i].useCount < count) {
			count = memoryBlocks[i].useCount;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindMF()
{
	int count = 0;
	int fid = 0;

	for (int i = 0; i < memoryBlocks.size(); i++) {
		if (memoryBlocks[i].useCount >= count) {
			count = memoryBlocks[i].useCount;
			fid = i;
		}
	}

	return fid;
}
#pragma endregion

uint8_t Memory::ReadByte(PageTableEntry entry, int offset, int blockSize)
{
	//int base = pageNum * blockSize;
	//return storeBlocks[processStart[pid]].data[base + offset];
	return 0xFF;
}


void Memory::FlushCache(int pid)
{
	for (auto& block : cacheBlocks) {
		if (block.dirty && block.inUse) {
			int frameIndex = block.pid;
		}

		block.inUse = false;
	}
}

int Memory::LoadFromStore(int pid, int pageNum)
{
	return 0;
}

void Memory::WriteToStore(Process *process, int bitWidth)
{
	std::vector<PageTableEntry> &table = process->pageTable;

	int count = 0;
	int storeBlock = 0;

	while (count < table.size()) {
		if (storeBlocks[storeBlock].inUse) continue;

		table[count].storeBlock = storeBlock;
		storeBlocks[storeBlock].data.resize(bitWidth, 0xFF);
	}
}
