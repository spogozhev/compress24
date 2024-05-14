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

class growingTable {
	std::map<stroka, unsigned> data;
	int maxsize;
public:
	growingTable(int bits): maxsize(1<<bits) {
		for (int i = 0; i < 256; ++i) {
			data.insert({ stroka(static_cast<unsigned char>(i)), i });
		}
	}
	bool insert(const stroka& x) {
		int count = static_cast<int>(data.size());
		data.insert({ x, count });
		if (count == maxsize-1) {
			maxsize <<= 1;
			return true;
		}
		return false;
	}
	int find(const stroka& x) {
		auto pos = data.find(x);
		if (pos != data.end()) {
			return pos->second;
		}
		return -1;
	}
	int size()const { return static_cast<int>(data.size()); }
};

class bitsbuf {
	unsigned int buf;
	int count;
public:
	bitsbuf() : buf(0), count(0) {}
	int get_byte() {
		if (count < 8)
		{
			buf <<= (8 - count);
			count = 8;
		}

		unsigned int result = buf >> (count - 8);
		buf -= result << (count - 8);
		count -= 8;
		
		return static_cast<int>(result);
	}
	int get ()
	{
		return static_cast<int>(buf);
	}
	void push(int x, int bitscount) {
		buf <<= bitscount;
		buf |= x;
		count += bitscount;
	}
	int size() const { return count; }
};

class LZW2 : public cstream {
	stroka str;
	growingTable table;
	bool is_eof;
	bitsbuf buffer;
	int curbits;
	int shift;
public:
	LZW2(cstream* s, int bitscount = 12) : cstream(s), curbits(bitscount), table(bitscount), buffer(bitsbuf()), is_eof(false),shift(-1) {}
	bool is_open() { return prev->is_open(); }
	int get() {

		while (buffer.size() < 8 && !is_eof) {

			int res, ch;

			do {
				ch = prev->get();
				if (ch == EOF) {
					is_eof = true;
					break;
				}
				stroka str_ch = str + ch;
				res = table.find(str_ch);
				if (res == -1) {
					if (table.insert(str_ch)) {
						shift = 2;
					}
				}
				else {
					str += ch;
				}
			} while (res != -1);

			if (shift >= 0) {
				if (shift == 0) {
					++curbits;
				}
				--shift;
			}
			buffer.push(table.find(str), curbits);
			str = stroka(ch);
		}

		if (is_eof && buffer.size() == 0)
		{
			return EOF;
		}

		return buffer.get_byte();
	}
};
