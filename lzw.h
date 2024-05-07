#pragma once
#include "cstream.h"
#include <cstring>
#include <map>
<<<<<<< HEAD
#include "stroka.h"
=======

class stroka {
	unsigned char *str;
	int count;
	int maxsize;
	void swap(stroka& other) {
		std::swap(count, other.count);
		std::swap(maxsize, other.maxsize);
		std::swap(str, other.str);
	}
public:
	stroka() : str(new unsigned char[4]), count(0), maxsize(4) {}
	stroka(unsigned char ch) : str(new unsigned char[4]), count(1), maxsize(4) { str[0] = ch; }
	~stroka() { delete[] str; }
	stroka(const stroka& other) {
		count = other.count;
		maxsize = other.maxsize;
		str = new unsigned char[maxsize];
		memcpy(str, other.str, maxsize);
	}
	stroka& operator=(const stroka& other) {
		if (this != &other) {
			stroka tmp(other);
			swap(tmp);
		}
		return *this;
	}
	stroka(stroka&& other) {
		count = 0;
		maxsize = 0;
		str = nullptr;
		swap(other);
	}
	stroka& operator=(stroka&& other) {
		if (this != &other) {
			swap(other);
		}
		return *this;
	}
	stroka& operator+=(unsigned char ch) {
		if (count == maxsize) {
			maxsize += 4;
			unsigned char* tmp = new unsigned char[maxsize];
			memcpy(tmp, str, count);
			delete[] str;
			str = tmp;
		}
		str[count++] = ch;
		return *this;
	}
	stroka operator+(unsigned char ch) {
		stroka tmp(*this);
		tmp += ch;
		return tmp;
	}
	bool operator==(const stroka& other) const {
		if (count == other.count) {
			for (int i = 0; i < count; ++i) {
				if (str[i] != other.str[i])
					return false;
			}
			return true;
		}
		return false;
	}
	bool operator<(const stroka& other) const {
		if (count == other.count) {
			int i;
			for (i = 0; i < count; ++i) {
				if (str[i] < other.str[i])
					return true;
				if (str[i] > other.str[i])
					return false;
			}
			return false;
		}
		return (count < other.count);
	}

	friend std::ostream& operator<<(std::ostream& out, const stroka& s) {
		for (int i = 0; i < s.count; ++i) {
			if (s.str[i] <= 32)
				out << (int)(s.str[i])<<" ";
			else 
				out << s.str[i];
		}
		return out;
	}
};
/*
class table {
	int size;
	int maxsize;
	stroka* data;
	table(const table&) {};
public:
	table(int bitscount) {
		maxsize = (1 << bitscount);
		data = new stroka[maxsize];
		if (data) {
			size = 256;
			for (int i = 0; i < size; ++i) {
				data[i] = stroka(i);
			}
		}
		else throw "not enought memory";
	}
	~table() { delete[] data; }
	void insert(const stroka& x) {
		if (size < maxsize) {
			data[size++] = x;
		}
	}
	int find(const stroka& x) {
		for (int i = 0; i < size; ++i) {
			if (data[i] == x)
				return i;
		}
		return -1;
	}
	void print() {
		for (int i = 0; i < size && i<500; ++i) {
			std::cout << i << "\t" << data[i] << std::endl;
		}
	}

};
*/

>>>>>>> main

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
