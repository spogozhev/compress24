#pragma once
#include "cstream.h"

class MTF : public cstream {
	int data[256];
public:
	MTF(cstream* s) : cstream(s) {
		for (int i = 0; i < 256; ++i) {
			data[i] = i;
		}
	}
	int get() {
		int ch = prev->get();
		if (ch == EOF)
			return ch;

		int res = 0;
		while (data[res] != ch) {
			++res;
			if (res == 256) {
				return -1;
			}
		}
		for (int i = res; i > 0; --i) {
			data[i] = data[i - 1];
		}
		data[0] = ch;
		return res;
	}
	bool is_open() { return prev->is_open(); }
};

class deMTF : public cstream {
	int data[256];
public:
	deMTF(cstream* s) : cstream(s) {
		for (int i = 0; i < 256; ++i) {
			data[i] = i;
		}
	}
	int get() {
		int ch = prev->get();
		if (ch == EOF)
			return ch;

		int res = data[ch];
		for (int i = ch; i > 0; --i) {
			data[i] = data[i - 1];
		}
		data[0] = res;
		return res;
	}
	bool is_open() { return prev->is_open(); }
};
