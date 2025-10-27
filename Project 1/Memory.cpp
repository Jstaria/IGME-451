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
	int count = INT_MAX;
	int fid = 0;

	for (int i = 0; i < frameBlocks.size(); i++) {
		if (frameBlocks[i].lastUsed < count) {
			count = frameBlocks[i].lastUsed;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindMR()
{
	int count = 0;
	int fid = 0;

	for (int i = 0; i < frameBlocks.size(); i++) {
		if (frameBlocks[i].lastUsed > count) {
			count = frameBlocks[i].lastUsed;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindLF()
{
	int count = INT_MAX;
	int fid = 0;

	for (int i = 0; i < frameBlocks.size(); i++) {
		if (frameBlocks[i].useCount < count) {
			count = frameBlocks[i].useCount;
			fid = i;
		}
	}

	return fid;
}

int Memory::FindMF()
{
	int count = 0;
	int fid = 0;

	for (int i = 0; i < frameBlocks.size(); i++) {
		if (frameBlocks[i].useCount > count) {
			count = frameBlocks[i].useCount;
			fid = i;
		}
	}

	return fid;
}
void Memory::Initialize(int frameSize, int storeSize, int cacheSize)
{
	frameBlocks.resize(frameSize, Block(blockSize));
	cacheBlocks.resize(cacheSize, Block(blockSize));
	storeBlocks.resize(storeSize, Block(blockSize));
}
#pragma endregion

uint8_t Memory::ReadByte(Process* process, uint8_t logicalAddr)
{
	// Get current process page table
	std::vector<PageTableEntry>& table = process->pageTable;

	// Get page number and data offset
	int pageNum = logicalAddr / blockSize;
	int offset = logicalAddr % blockSize;

	int block = table[pageNum].cacheBlock;

	CheckLoad(process, logicalAddr);

	return cacheBlocks[block].data[offset];
}

void Memory::WriteByte(Process* process, uint8_t logicalAddr, uint8_t data) {
	// Get current process page table
	std::vector<PageTableEntry>& table = process->pageTable;

	// Get page number and data offset
	int pageNum = logicalAddr / blockSize;
	int offset = logicalAddr % blockSize;

	int block = table[pageNum].cacheBlock;

	CheckLoad(process, logicalAddr);

	cacheBlocks[block].data[offset] = data;
	cacheBlocks[block].dirty = true;
}

void Memory::FlushCache(Process* process)
{
	// Get current process page table
	std::vector<PageTableEntry>& table = process->pageTable;

	for (auto& entry : table) {

		if (entry.cacheBlock == -1) continue;
		// Get cache block
		Block& block = cacheBlocks[entry.cacheBlock];

		// Write block to store if its dirty and in use
		if (block.dirty && block.inUse) {
			WriteToStore(block, entry);
		}

		block.inUse = false;
	}
}

void Memory::LoadFromMemory(Process* process, uint8_t logicalAddr) {
	process->memoryStats.misses++;

	// Get page number and data offset
	int pageNum = logicalAddr / blockSize;
	int offset = logicalAddr % blockSize;

	// Get current process page table
	std::vector<PageTableEntry>& table = process->pageTable;

	// Get frame block from page table
	Block block = frameBlocks[table[pageNum].frameBlock];

	// Get next cache block
	for (int i = 0; i < cacheBlocks.size(); i++) {
		if (!cacheBlocks[i].inUse) {
			table[pageNum].cacheBlock = i;
			cacheBlocks[i].data = block.data;
			cacheBlocks[i].useCount = 0;
			cacheBlocks[i].lastUsed = 0;
			cacheBlocks[i].inUse = true;
		}
	}
}

void Memory::LoadFromStore(Process* process, uint8_t logicalAddr)
{
	process->cacheStats.misses++;
	int frame = GetReplacementFrame();

	// Get page number and data offset
	int pageNum = logicalAddr / blockSize;
	int offset = logicalAddr % blockSize;

	// Get current process page table
	std::vector<PageTableEntry>& table = process->pageTable;

	// Get store block from page table
	Block block = storeBlocks[table[pageNum].storeBlock];

	// Set replacement frame to the store block
	frameBlocks[frame] = block;
	// Update page table
	table[pageNum].frameBlock = frame;
}

void Memory::CheckLoad(Process* process, uint8_t logicalAddr) {

	// Get current process page table
	std::vector<PageTableEntry>& table = process->pageTable;

	// Get page number and data offset
	int pageNum = logicalAddr / blockSize;
	int offset = logicalAddr % blockSize;

	int block = table[pageNum].cacheBlock;

	if (!frameBlocks[table[pageNum].frameBlock].inUse)
		LoadFromStore(process, logicalAddr);

	if (!cacheBlocks[block].inUse)
		LoadFromMemory(process, logicalAddr);
}

void Memory::WriteToStore(Process* process, uint8_t data)
{
	// Get page table from current process
	std::vector<PageTableEntry>& table = process->pageTable;

	int count = 0;
	int storeBlock = 0;

	// Find blocks in storage and update block values
	while (count < table.size()) {

		if (!storeBlocks[storeBlock].inUse) {
			table[count].storeBlock = storeBlock;
			storeBlocks[storeBlock].useCount++;
			storeBlocks[storeBlock].lastUsed = storeLastUsed++;
			storeBlocks[storeBlock].inUse = true;
			storeBlocks[storeBlock].data.assign(blockSize, data);
			storeBlocks[storeBlock].pid = process->pid;

			count++;
		}

		storeBlock++;
	}
}

void Memory::WriteToStore(Block& block, PageTableEntry entry) {
	storeBlocks[entry.storeBlock] = block;
	frameBlocks[entry.frameBlock] = block;
	block.dirty = false;
}

void Memory::DeleteProcess(Process* process)
{
	std::vector<PageTableEntry>& table = process->pageTable;

	for (auto& entry : table)
	{
		if (entry.cacheBlock >= 0 && entry.cacheBlock < cacheBlocks.size())
		{
			Block& cacheBlock = cacheBlocks[entry.cacheBlock];
			cacheBlock.inUse = false;
			cacheBlock.dirty = false;
			cacheBlock.useCount = 0;
			cacheBlock.lastUsed = 0;
			cacheBlock.data.resize(blockSize, 0xFF);
		}

		if (entry.frameBlock >= 0 && entry.frameBlock < frameBlocks.size())
		{
			Block& memBlock = frameBlocks[entry.frameBlock];
			memBlock.inUse = false;
			memBlock.dirty = false;
			memBlock.useCount = 0;
			memBlock.lastUsed = 0;
			memBlock.data.resize(blockSize, 0xFF);
		}

		if (entry.storeBlock >= 0 && entry.storeBlock < storeBlocks.size())
		{
			Block& storeBlock = storeBlocks[entry.storeBlock];
			storeBlock.inUse = false;
			storeBlock.dirty = false;
			storeBlock.useCount = 0;
			storeBlock.lastUsed = 0;
			storeBlock.data.resize(blockSize, 0xFF);
			storeBlock.pid = -1;
		}
	}

	table.clear();
}

void Memory::PrintCurrentProcessInfo(Process* process)
{
	std::vector<PageTableEntry>& table = process->pageTable;

	std::cout << "====================================================\n";
	std::cout << "Process ID: " << process->pid << " Memory Dump\n";
	std::cout << "====================================================\n";

	// Header
	std::cout << std::left
		<< std::setw(6) << "Page"
		<< std::setw(8) << "Cache#"
		<< std::setw(8) << "Frame#"
		<< std::setw(8) << "Store#"
		<< std::setw(10) << "BlockAddr"
		<< std::setw(6) << "Offset"
		<< "Data (hex)\n";
	std::cout << "----------------------------------------------------\n";

	// Iterate through all pages
	for (size_t pageIndex = 0; pageIndex < table.size(); pageIndex++)
	{
		PageTableEntry& entry = table[pageIndex];
		Block& storeBlock = storeBlocks[entry.storeBlock];

		// Iterate through each byte in the block
		for (size_t offset = 0; offset < storeBlock.data.size(); offset++)
		{
			std::cout << std::setw(6) << pageIndex
				<< std::setw(8) << entry.cacheBlock
				<< std::setw(8) << entry.frameBlock
				<< std::setw(8) << entry.storeBlock
				<< std::setw(10) << pageIndex * blockSize + offset  // starting address of block
				<< std::setw(6) << offset
				<< "0x" << std::hex << std::setw(2) << std::setfill('0')
				<< (int)storeBlock.data[offset]
				<< std::dec << std::setfill(' ') << "\n";
		}
	}

	std::cout << "====================================================\n";
}
