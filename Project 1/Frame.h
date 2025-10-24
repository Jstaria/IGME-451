#pragma once
#include <vector>

class Frame
{
public:
	bool dirty;
	bool inUse;
	std::vector<uint8_t> data;

	int pid;
	int pageNum;
	int lastUsed;
	int useCount;

	Frame(int dataSize) :
	pid(-1), pageNum(-1), lastUsed(0), useCount(0), dirty(false), inUse(false){
		data.resize(dataSize);
	}
};

