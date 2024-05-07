#pragma once
#include "cstream.h"
#include <vector>
#include <string>

class delz77_buf {

	int size;
	int* buf;
	int end;
public:
	delz77_buf(size_t size): size(size) {
		end =  0;
		buf = new int[size];
		for (int i = 0; i < size; ++i) {
			buf[i] = -1;
		}
	}

	void next(int& in) {
		++in;
		if (in >= size || buf[in] == -1) {
			in = 0;
		}
	}

	void prev(int& in ) {
		--in;
		if (in < 0) {
			in = size - 1;
		}

		if (buf[in] != -1) {
			while (buf[in] == -1) {
				--in;
			}
		}
	}

	void push_back(int in) {
		int tmp = end;
		if (buf[0] == -1 ) {
			buf[0] = in;
		}
		else if ( end + 1 < size ) {
			buf[++end] = in;
		}
		else{
			next(end);
			buf[end] = in;
		}
	}
	void get(int offset,int len, int ret[]) {
		int cur = end;
		for (int i = 0; i < offset - 1; ++i) {
			prev(cur);
		}

		int pos = 0;
		for (int i = 0; i < len; ++i,next(cur)) {
			ret[i] = buf[cur];
		}
	}
	~delz77_buf() {
		delete[] buf;
	}
};

class delz77 : public cstream {

	delz77_buf buf;
	int* ans;
	int counter;

	int ans_len;

public:
	delz77(cstream* s, size_t buf_size = 128) : cstream(s),buf(buf_size) {
		ans = new int[buf_size* 3];
		counter = 0 ;
		ans_len = -1;
	}
	bool is_open() { return prev->is_open(); }
	int get() {

		if (counter < ans_len) {
			++counter;
			return ans[counter - 1];
		}



		int offset = prev->get();
		int len = prev->get();

		if (offset == EOF || len == EOF) {
			return EOF;
		}

		if (offset == 0 && len == 0) {
			int symb = prev->get();
			buf.push_back(symb);
			return symb;
		}

		buf.get(offset, len, ans);
		//counter = len;

		for (int i = 0; i < len; ++i) {
			buf.push_back(ans[i]);
		}

		counter = 1;
		ans_len = len;

		int symb = prev->get();
		if (symb != EOF) {
			buf.push_back(symb);
		    ans[ans_len] = symb;
			++ans_len;
		}


		return ans[0];

	}
	~delz77() {
		delete[] ans;
	}

};

class lz77_bufer
{
	// len длина 

	//размер 128
	// size память

	// push_back(ch)
	// int int  find(string )
	// int next(int) 

	int* arr;
	int size;
	int first;
	int length;
public:
	lz77_bufer(size_t size) : size(size), length(0), first(0)
	{
		arr = new int[size] {0};
	}

	bool is_empty() {
		return length == 0;
	}

	int next(int pos)
	{
		if (pos + 1 >= length) {
			return 0;
		}
		return pos + 1;
	}

	int prev(int pos)
	{
		if (pos - 1 < 0)
		{
			return length - 1;
		}
		return pos - 1;
	}

	void push_back(int ch)
	{
		//если буфер пустой
		if (length == 0) {
			arr[0] = ch;
			length = 1;
		}
		else if (length + 1 <= size) {
			arr[length] = ch;
			++length;
		}
		else { // если память законичлась
			arr[first] = ch;
			first = next(first);
		}
	}

	void find(const std::vector<int>& s, int& res)
	{
		int pos = prev(first);
		do {
			int tpos = pos;
			int slen = s.size();
			int i = 0;
			for (; i < slen; ++i, tpos = next(tpos)) {
				if (arr[tpos] != s[i]) { // буфер отличается от s
					break;
				}
			}

			if (i == slen) {
				int count = 0;
				for (int j = prev(first); j != pos; ++count, j = prev(j));
				res = count+1;
				return;
			}

			pos = prev(pos);
		} while (pos != prev(first));
		res = -1;
	}
};

class lz77 : public cstream
{
	lz77_bufer buf;
	// (0,0,a)
	int offset;
	int length;
	int next;
public:
	lz77(cstream* s,size_t size = 128) : cstream(s), offset(-1), length(-1), next(-1), buf(size) {}
	
	bool is_open() { return prev->is_open(); }
	int get()
	{
		//если есть что отдавать,то отдаем
		if (offset != -1) {
			int tmp = offset;
			offset = -1;
			return tmp;
		}
		if (length != -1) {
			int tmp = length;
			length = -1;
			return tmp;
		}
		if (next != -1) {
			int tmp = next;
			next = -1;
			return tmp;
		}


		int ch = prev->get();
		if (ch == EOF) {
			return EOF;
		}

		if (buf.is_empty()) {
			offset = -1;
			length = 0;
			next = ch;
			buf.push_back(next);
			return 0; // offset
		}
		std::vector<int>s(1,ch);
		//s.push_back;
		int res = 0;
		offset = 0;
		length = 0;
		next = 0;
		
		for (int kol = 0; kol < 128;++kol) {
		
			buf.find(s, res);
			if (res == -1) {
				int tmp = offset;
				next = ch;
				for (int i = 0; i < s.size(); ++i) {
					buf.push_back(s[i]);
				}
				offset = -1;
				return tmp;
			}
			else
			{
				offset = res;
				length = s.size();

				if (length == 127) {
					int tmp = offset;
					next = prev->get();
					for (int i = 0; i < s.size(); ++i) {
						buf.push_back(s[i]);
					}
					offset = -1;
					return tmp;
				}

				ch = prev->get();
				if (ch == EOF) {
					int tmp = offset;
					next = ch;
					offset = -1;
					return tmp;
				}

				s.push_back(ch);
			}

		}

	}
};
