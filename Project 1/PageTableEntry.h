#pragma once

struct PageTableEntry {
	int frameBlock = -1;
	int cacheBlock = -1;
	int storeBlock = -1;
};