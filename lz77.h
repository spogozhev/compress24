#pragma once
#include "cstream.h"

class delz77_buf {

	int size = 128;
	int* buf;
	int end;
public:
	delz77_buf() {
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
		if (buf[end] == -1) {
			buf[end] = in;
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
			ret[pos] = buf[i];
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
	delz77(cstream* s) : cstream(s) {
		ans = new int[128];
		counter = 0 ;
		ans_len = -1;
	}
	bool is_open() { return prev->is_open(); }
	int get() {

		if (counter <= ans_len) {
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
		counter = len;
		for (int i = 0; i < len; ++i) {
			buf.push_back(ans[i]);
		}
		counter = 1;
		ans_len = len;

		int symb = prev->get();
		if (symb != EOF) {
			buf.push_back(symb);
		}

		return ans[0];

	}
	~delz77() {
		delete[] ans;
	}

};