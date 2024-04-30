#pragma once
#include "stroka.h"
#include <map>
#include "cstream.h"

class expandableRingbuf {
	int N;
	unsigned char* buf;
	int first;
	int count;
public:
	expandableRingbuf() : N(10), first(0), count(0) {
		buf = new unsigned char[N];
	}
	expandableRingbuf(const stroka& s) :N(s.size() + 10), first(0), count(s.size()) {
		buf = new unsigned char[N];
		for (int i = 0; i < count; ++i) {
			buf[i] = s[i];
		}
	}
	expandableRingbuf& operator=(const stroka& s) {
		first = 0;
		count = s.size();
		if (count > N) {
			delete[]buf;
			N = count + 10;
			buf = new unsigned char[N];
		}
		for (int i = 0; i < count; ++i) {
			buf[i] = s[i];
		}
		return *this;
	}
	int size() const { return count; }
	unsigned char pop_front() {
		if (count == 0)
			throw "no elements";
		unsigned char ans = buf[first];
		++first;
		first %= N;
		--count;
		return ans;
	}
};

class rtable {
	int maxsize;
	std::map<int, stroka> data;
public:
	table(int bitscount) {
		maxsize = (1 << bitscount);
		for (int i = 0; i < 256; ++i) {
			data.insert({ i, stroka(i) });
		}
	}
	void insert(const stroka& x) {
		int count = data.size();
		if (count < maxsize) {
			data.insert({ count,x });
		}
	}
	stroka find(int x) {
		auto pos = data.find(x);
		if (pos != data.end()) {
			return pos->second;
		}
		return stroka();
	}
	int size()const { return static_cast<int>(data.size()); }
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
class deLZW : public cstream {
	expandableRingbuf buf;
	stroka pr;
	rtable decodeTable;
	bool is_eof;
public:
	deLZW(cstream* s, int bitscount = 12) :cstream(s), decodeTable(bitscount), is_eof(false), pr(stroka()), buf(expandableRingbuf()) {}
	bool is_open() { return prev->is_open(); }
	int get() {
		if (is_eof) {
			return EOF;
		}
		int res;
		if (buf.size() > 0) {
			res = static_cast<int>(buf.pop_front());
		}
		else {
			int code = prev->get();
			if (code == EOF) {
				is_eof = true;
				return EOF;
			}
			if (code >= decodeTable.size()) {
				return EOF;
			}
			stroka entry = decodeTable.find(code);
			buf = entry;
			if (pr.size() > 0) {
				pr += entry[0];
				decodeTable.insert(pr);
			}
			pr = entry;
			res = static_cast<int>(buf.pop_front());
		}
		return res;
	}
};