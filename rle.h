#pragma once
#include "cstream.h"
#include "ringbuf.h"

class RLE : public cstream {
	//unsigned char buf[132];
	//int buflen;
	ringbuf<unsigned char, 132> buf;
	int counter;
public:
	RLE(cstream* s) : cstream(s), counter(0) {}
	bool is_open() { return prev->is_open(); }
	int get() {
		/*
			counter - количество символов, которые ещё надо отдать
		*/
		if (counter > 0) {
			--counter;
			return buf.pop_front();
		}
		int ch = prev->get();
		if (ch == EOF) {
			if ((buf.size() == 2) && buf.is_rep()) {
				counter = 1;
				buf.pop_front();
				return 0;
			}
			else if (buf.size() > 0) {
				counter = buf.size();
				return (buf.size() - 1) + 128;
			}
			return ch;
		}
		buf.push_back(ch);
		if (buf.size() == 1) {
			ch = prev->get();
			if (ch == EOF) {
				counter = buf.size();
				return (buf.size() - 1) + 128;
			}
			buf.push_back(ch);
		}
		while (buf.size() < 130 && buf.is_rep()) {
			// копим повторения
			ch = prev->get();
			if (ch == EOF) break;
			buf.push_back(ch);
		}
		if (buf.size() > 2) {
			int result = buf.size();
			auto tmp2 = buf.pop_back();
			auto tmp1 = buf.pop_back();
			buf.empty();

			if (ch == EOF) {
				buf.push_back(tmp2);
			}
			else {
				buf.push_back(tmp1);
				buf.push_back(tmp2);
				result -= 1;
			}
			counter = 1;
			return result - 2;
		}
		else if (buf.size() == 2 && buf.is_rep()) {
			buf.pop_front();
			counter = 1;
			return 0;
		}
		while (buf.size() < 130 && !buf.is_rep()) {
			// копим уникальные элементы
			ch = prev->get();
			if (ch == EOF) break;
			buf.push_back(ch);
		}
		if (ch == EOF) {
			counter = (buf.size() > 128) ? 128 : buf.size();
		}
		else {
			counter = buf.size() - 2;
		}
		return 128 + (counter - 1);
	}
};
 

class deRLE : public cstream {
	int buf;
	int counter_rep;
	int counter_unique;
public:
	deRLE(cstream* s) : cstream(s), counter_rep(0), counter_unique(0) {}
	bool is_open() { return prev->is_open(); }
	int get() {
		if (prev == nullptr)
			return -1;
		if (counter_rep > 0) {
			--counter_rep;
			return buf;
		}
		if (counter_unique > 0) {
			--counter_unique;
			return prev->get();
		}
		int ch = prev->get();
		if (ch == EOF) {
			return ch;
		}
		else if (ch < 128) {
			counter_rep = ch + 2 - 1;
			buf = prev->get();
			return buf;
		}
		else {
			counter_unique = (ch - 128);
			return prev->get();
		}
	}

};