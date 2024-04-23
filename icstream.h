#pragma once
#include "cstream.h"
#include <fstream>

class icstream : public cstream {
	std::ifstream inFile;
public:
	icstream(const char* filename) {
		inFile.open(filename, std::ios_base::binary);
	}
	~icstream() {
		inFile.close();
	}
	int get() {
		return inFile.get();
	}
	bool is_open() { return inFile.is_open(); }
};
