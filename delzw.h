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
	int bitscnt;
	std::map<int, stroka> data;
public:
	rtable(int bitscount) : bitscnt(bitscount) {
		maxsize = (1 << bitscount);
		for (int i = 0; i < 256; ++i) {
			data.insert({ i, stroka(i) });
		}
	}
	bool insert(const stroka& x) {
		int count = data.size();
		if (count < maxsize) {
			data.insert({ count,x });
		}
		else
		{
			resize(++bitscnt);
			return false;
		}

		return true;
	}
	stroka find(int x) {
		auto pos = data.find(x);
		if (pos != data.end()) {
			return pos->second;
		}
		return stroka();
	}
	int size()const { return static_cast<int>(data.size()); }
	void resize (int newbitscount)
	{
		maxsize = (1 << newbitscount);
	}
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
	int bits;
	unsigned int buf;
	int count;

	expandableRingbuf exp_buf;
	stroka pr;
	rtable decodeTable;
	bool is_eof;
public:
	deLZW(cstream* s, int bitscount = 12) : cstream(s), decodeTable(bitscount), is_eof(false), 
		pr(stroka()), exp_buf(expandableRingbuf()), bits(bitscount), count(0), buf(0) {}
	bool is_open() { return prev->is_open(); }
	int get() {

		if (is_eof) {
			return EOF;
		}
		int res;
		if (exp_buf.size() > 0) {
			res = static_cast<int>(exp_buf.pop_front());
		}
		else {
			int code;

			while (count < bits && !is_eof)
			{
				int tmp = prev->get ();
				if (tmp == EOF)
				{
					is_eof = true;
				} 		
				else
				{
					buf = (buf << 8) | static_cast<unsigned int>(tmp);
					count += 8;
				}
			}

			if (is_eof && count < bits)
			{
				code = EOF;
			}
			else
			{
				int shift = count - bits;
				unsigned int result = buf >> shift;
				buf -= result << shift;
				count -= bits;
				code = static_cast<int>(result);
			}

			if (code == EOF || code > decodeTable.size()) {
				is_eof = true;
				return EOF;
			}

			if (code == decodeTable.size() && pr.size() > 0) {
				pr += pr[0];
				if (!decodeTable.insert (pr))
				{
					++bits;
				}
				exp_buf = pr;
			}
			else {
				stroka entry = decodeTable.find(code);
				exp_buf = entry;
				if (pr.size() > 0) {
					pr += entry[0];
					if (!decodeTable.insert (pr))
					{
						++bits;
					}
				}
				pr = entry;
			}
			res = static_cast<int>(exp_buf.pop_front());
		}
		return res;
	}
};
