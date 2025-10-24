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

int Memory::FindLR()
{
	int count = 99999999999;
	int fid = 0;

	for (int i = 0; i < memory.size();i++) {
		if (memory[i].lastUsed < count) {
			count = memory[i].lastUsed;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindMR()
{
	int count = 0;
	int fid = 0;

	for (int i = 0; i < memory.size(); i++) {
		if (memory[i].lastUsed >= count) {
			count = memory[i].lastUsed;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindLF()
{
	int count = 999999999;
	int fid = 0;

	for (int i = 0; i < memory.size(); i++) {
		if (memory[i].useCount < count) {
			count = memory[i].useCount;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindMF()
{
	int count = 0;
	int fid = 0;

	for (int i = 0; i < memory.size(); i++) {
		if (memory[i].useCount >= count) {
			count = memory[i].useCount;
			fid = i;
		}
	}

	return fid;
}

void Memory::FlushCache(int pid)
{
	for (auto& block : cache) {
		if (block.dirty && block.inUse) {
			int frameIndex = block.tag;
		}

		block.inUse = false;
	}
}

int Memory::LoadFromStore(int pid, int pageNum)
{
	return 0;
}

int Memory::WriteToStore(int pid, int pageNum, int frame)
{
	return 0;
}
