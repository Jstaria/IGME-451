#pragma once
#include <vector>

class Block
{
public:
	bool dirty;
	bool inUse;
	std::vector<uint8_t> data;

	int pid;
	int lastUsed;
	int useCount;

	Block(int dataSize) : 
		dirty(false), inUse(false), lastUsed(-1), useCount(-1), pid(-1) {
		data.resize(dataSize);
	}
};

