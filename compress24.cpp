#include <ctype.h>
#include <iostream>
#include <fstream>
#include "cstreams.h"
#include <vector>
#include <cstring>

void help() {
	std::cout << "Use \ncompress24 [-options] source_file_name destination_file_name\n";
	std::cout << "  -mtf     move to front\n";
	std::cout << "  -rle     run length encoding\n";
    std::cout << "  -lzw     Lempel-Ziv-Welch encoding\n";
	std::cout << "  -bwt     Burrows – Wheeler transform\n";
	std::cout << "  -demtf   decode move to front\n";
	std::cout << "  -derle   decode RLE\n";
	std::cout << "  -debwt	 decode BWT\n";
	std::cout << "  -d       decompress\n";
}

char* str2lower(char* arg) {
	char* str = arg;
	while (*str) {
		*str = tolower(*str);
		++str;
	}
	return arg;
}

enum class Blocks {RLE = 1, deRLE, MTF, deMTF, BWT, deBWT, oBit, iBit, LZW, deLZW, LZ77, deLZ77};

int main(int argc, char* argv[]) {
	if (argc < 3) {
		help();
		return 1;
	}
	cstream* cs = nullptr;

    std::ofstream outFile(argv[argc-1], std::ios_base::binary);
    if (!outFile) {
        std::cout << "Can not write to file " << argv[argc-1] << "\n";
        return 2;
    }

    cs = new icstream(argv[argc-2]);
    if (!cs->is_open()) {
        std::cout << "Can not open file "<< argv[argc-2] << "\n";
        delete cs;
        outFile.close();
        return 3;
    }

	if (strcmp(argv[1], "-d")==0){
		cs->get(); // Skip C2
		cs->get();
		cs->get();
		int ch;
		cstream * inFile = cs;
		while((ch=inFile->get())!=0){
            if (ch == static_cast<int>(Blocks::deMTF)){
                cs = new deMTF(cs);
            } else if (ch == static_cast<int>(Blocks::deRLE)){
				cs = new deRLE(cs);
			} else if (ch == static_cast<int>(Blocks::deBWT)){
                cs = new deBWT(cs);
            } else if (ch == static_cast<int>(Blocks::iBit)){
                int bits = inFile->get();
				cs = new ibitstream(cs, bits);
            } else if (ch == static_cast<int>(Blocks::deLZW)){
                cs = new deLZW(cs);
            }
		}
	} else {

		std::vector<unsigned char> code_str;
		for (int i = 1; i < argc - 2; ++i) {
			str2lower(argv[i]);
			if (strcmp(argv[i], "-mtf")==0){
				cs = new MTF(cs);
				code_str.push_back(static_cast<unsigned char>(Blocks::deMTF));
			} else if (strcmp(argv[i], "-rle") == 0) {
				cs = new RLE(cs);
                code_str.push_back(static_cast<unsigned char>(Blocks::deRLE));
			} else if (strcmp(argv[i], "-lzw") == 0) {
				int bits = 0;
				if (i + 1 < argc - 2) {
					bits = atoi(argv[i + 1]);
					if (bits > 0) {
						++i;
					}
				}
				bits = (bits < 9) ? 9 : bits;
				bits = (bits >255) ? 255 : bits;
				cs = new LZW(cs, bits);
				cs = new obitstream(cs, bits);
                code_str.push_back(static_cast<unsigned char>(Blocks::deLZW));
                code_str.push_back(static_cast<unsigned char>(bits));
                code_str.push_back(static_cast<unsigned char>(Blocks::iBit));

			} else if (strcmp(argv[i], "-bwt") == 0) {
				cs = new BWT(cs);
                code_str.push_back(static_cast<unsigned char>(Blocks::deBWT));
			} else if (strcmp(argv[i], "-lz77") == 0) {
				// cs = new LZ77(cs);
                code_str.push_back(static_cast<unsigned char>(Blocks::deLZ77));
			} else {
				std::cout << "Wrong argument '" << argv[i] << "'. Skipped." << std::endl;
			}
		}
		outFile.write("C24", 3);
		for(int i=code_str.size()-1; i>=0; --i){
			outFile.put(code_str[i]);
		}
		outFile.put(0);
	}

	int ch;
	while ((ch = cs->get()) != EOF) {
		outFile.put(ch);
	}

	outFile.close();

	delete cs;
	return 0;
}
