#pragma once
#include "cstream.h"
#include <cstring>
#include <map>
#include "stroka.h"

class table {
	int maxsize;
	std::map<stroka, int> data;
public:
	table(int bitscount) {
		maxsize = (1 << bitscount);
		for (int i = 0; i < 256; ++i) {
			data.insert({ stroka(i), i });
		}
	}
	void insert(const stroka& x) {
		int count = data.size();
		if (count < maxsize) {
			data.insert({ x, count });
		}
	}
	int find(const stroka& x) {
		auto pos = data.find(x);
		if (pos != data.end()) {
			return pos->second;
		}
		return -1;
	}
	int size()const { return data.size(); }
	void print() {
		for (int i = 0; i < data.size() && i < 500; ++i) {
			for (auto pos = data.begin(); pos != data.end(); ++pos) {
				if (pos->second == i) {
					std::cout << pos->second << "\t" << pos->first << std::endl;
					break;
				}
			}
		}
	}
};

class bitsbuf {
	unsigned int buf;
	int count;
public:
	bitsbuf() : buf(0), count(0) {}
	int get_byte() {
		if (count < 8) return -1;
		unsigned int result = buf >> (count - 8);
		buf -= result << (count - 8);
		count -= 8;
		return static_cast<int>(result);
	}
	int get() {
		return static_cast<int>(buf);
	}
	void push(int x, int bitscount) {
		buf <<= bitscount;
		buf |= x;
		count += bitscount;
	}
	int size() const { return count; }
};

class LZW : public cstream {
	stroka str;
	table table;
	bool is_eof;
public:
	LZW(cstream* s, int bitscount = 12) : cstream(s), table(bitscount), is_eof(false) {}
	bool is_open() { return prev->is_open(); }
	int get() {
		if (is_eof) {
			return EOF;
		}
		int res, ch;
		do {
			ch = prev->get();
			if (ch == EOF) {
				is_eof = true;
				return table.find(str);
			}
			stroka str_ch = str + ch;
			res = table.find(str_ch);
			if (res == -1) {
				table.insert(str_ch);
			}
			else {
				str += ch;
			}
		} while (res != -1);

		res = table.find(str);
		str = stroka(ch);
		return res;
	}
};
