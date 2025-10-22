#pragma once

struct PageTableEntry {
	int frameNum = -1;
	bool valid = false;
};