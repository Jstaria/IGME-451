#pragma once
#include <vector>

class CacheBlock
{
public:
	bool dirty;
	bool valid;
	std::vector<uint8_t> data;

	int tag;
	int lastUsed;
	int useCount;

	CacheBlock(int dataSize) : 
		dirty(false), valid(false), lastUsed(0), useCount(0), tag(-1) {
		data.resize(dataSize);
	}
};

