#include <ctype.h>
#include <iostream>
#include <fstream>
#include "cstreams.h"

void help() {
	std::cout << "Use \ncompress24 [-options] source_file_name destination_file_name\n";
	std::cout << "  -mtf     move to front\n";
	std::cout << "  -rle     run length encoding\n";
	std::cout << "  -demtf   decode move to front\n";
	std::cout << "  -derle   decode RLE\n";
	std::cout << "  -lzw     Lempel-Ziv-Welch encoding\n";
}

char* str2lower(char* arg) {
	char* str = arg;
	while (*str) {
		*str = tolower(*str);
		++str;
	}
	return arg;
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		help();
		return 1;
	}
	cstream* cs = new icstream(argv[argc-2]);
	if (!cs->is_open()) {
		std::cout << "Can not open file test.txt\n";
		delete cs;
		return 1;
	}
	for (int i = 1; i < argc - 2; ++i) {
		str2lower(argv[i]);
		if (strcmp(argv[i], "-mtf")==0)
			cs = new MTF(cs);
		else if (strcmp(argv[i], "-demtf") == 0)
			cs = new deMTF(cs);
		else if (strcmp(argv[i], "-rle") == 0)
			cs = new RLE(cs);
		else if (strcmp(argv[i], "-derle") == 0)
			cs = new deRLE(cs);
		else if (strcmp(argv[i], "-lzw") == 0) {
			int bits = 0;
			if (i + 1 < argc - 2) {
				bits = atoi(argv[i + 1]);
				if (bits > 0) {
					++i;
				}
			}
			bits = (bits < 9) ? 9 : bits;
			cs = new LZW(cs, bits);
			cs = new obitstream(cs, bits);
		}
		else if (strcmp(argv[i], "-delzw") == 0) {
			int bits = 0;
			if (i + 1 < argc - 2) {
				bits = atoi(argv[i + 1]);
				if (bits > 0) {
					++i;
				}
			}
			bits = (bits < 9) ? 9 : bits;
			cs = new deLZW(cs, bits);
		}

	}

	std::ofstream outFile(argv[argc-1], std::ios_base::binary);
	if (!outFile) {
		std::cout << "Can not write to file\n";
		delete cs;
		return 2;
	}

	int ch;
	while ((ch = cs->get()) != EOF) {
		outFile.put(ch);
	}
	
	outFile.close();
	
	delete cs;
	return 0;
}
