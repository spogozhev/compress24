#pragma once
#include "cstream.h"

class obitstream : public cstream {
	int bits;
	unsigned int buf;
	int count;
	bool is_eof;
public:
	obitstream(cstream* s, int bitscount = 12) : cstream(s), bits(bitscount),
		count(0), buf(0), is_eof(false) {}
	bool is_open() { return prev->is_open(); }
	int get() {
		while (count < 8 && !is_eof) {
			int tmp = prev->get();
			if (tmp == EOF) {
				is_eof = true;
			}
			else {
				buf = (buf << bits) | static_cast<unsigned int>(tmp);
				count += bits;
			}
		}
		if (is_eof && count == 0) {
			return EOF;
		}
		if (count < 8) {
			buf <<= (8 - count);
			count = 8;
		}
		int shift = count - 8;
		unsigned int result = buf >> shift;
		buf -= result << shift;
		count -= 8;
		return static_cast<int>(result);
	}
};