#pragma once
#include <vector>

class CacheBlock
{
public:
	bool dirty;
	bool inUse;
	std::vector<uint8_t> data;

	int tag;
	int lastUsed;
	int useCount;

	CacheBlock(int dataSize) : 
		dirty(false), inUse(false), lastUsed(0), useCount(0), tag(-1) {
		data.resize(dataSize);
	}
};

