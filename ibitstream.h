#pragma once
#include "cstream.h"

class ibitstream : public cstream {
	int bits;
	unsigned int buf;
	int count;
	bool is_eof;
public:
	ibitstream(cstream* s, int bitscount = 12) : cstream(s), bits(bitscount),
		count(0), buf(0), is_eof(false) {}
	bool is_open() { return prev->is_open(); }
	int get() {
		while (count < bits && !is_eof) {
			int tmp = prev->get();
			if (tmp == EOF) {
				is_eof = true;
			}
			else {
				buf = (buf << 8) | static_cast<unsigned int>(tmp);
				count += 8;
			}
		}
		if (is_eof && count < bits) {
			return EOF;
		}

		int shift = count - bits;
		unsigned int result = buf >> shift;
		buf -= result << shift;
		count -= bits;
		return static_cast<int>(result);
	}
};

