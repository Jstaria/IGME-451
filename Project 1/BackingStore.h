#pragma once
#include <map>
#include <vector>

class BackingStore
{
public:
	std::vector<uint8_t> storage;
	std::map<int, int> processStart;

    int Allocate(int pid, int bytes)
    {
        int start = storage.size();
        processStart[pid] = start;
        for (int i = 0; i < bytes; ++i)
            storage.push_back(i % 256);
        return start;
    }

    uint8_t ReadByte(int pid, int pageNum, int offset, int blockSize)
    {
        int base = processStart[pid] + pageNum * blockSize;
        return storage[base + offset];
    }

    void WritePage(int pid, int pageNum, int blockSize, const std::vector<uint8_t>& data)
    {
        int base = processStart[pid] + pageNum * blockSize;
        for (int i = 0; i < blockSize; ++i)
            storage[base + i] = data[i];
    }
};

