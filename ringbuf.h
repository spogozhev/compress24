#pragma once
#include "lzw.h"
template <typename T, int N>
class ringbuf {
	T buf[N];
	int first;
	int count;
public:
	ringbuf() : count(0), first(0) {}
	int size() const { return count; }
	void push_back(const T& X) {
		if (count >= N)
			throw "out of space";
		int index = (first + count)%N;
		buf[index] = X;
		++count;
	}
	T pop_front() {
		if (count == 0)
			throw "no elements";
		T ans = buf[first];
		++first;
		first %= N;
		--count;
		return ans;
	}
	T pop_back() {
		if (count == 0)
			throw "no elements";
		T ans = buf[(first+count-1)%N];
		--count;
		return ans;
	}
	void empty() { count = 0; }
	T operator[](int index) {
		if (index >= count || index < 0)
			throw "out of limits";
		return buf[(first + index) % N];
	}
	T last() const {
		if (count == 0)
			throw "no elements";
		return buf[(first + count - 1) % N];
	}
	bool is_rep()const {
		if (count < 2)
			return false;
		int last = (first + count - 1) % N;
		int prev = last - 1;
		prev = (prev < 0) ? prev + N : prev;
		return buf[prev] == buf[last];
	}
};

class expandableRingbuf {
	int N;
	unsigned char* buf;
	int first;
	int count;
public:
	ringbuf() : N(10), first(0),count(0) {
		buf = new unsigned char[N];
	}
	ringbuf(const stroka& s):N(s.size()+10),first(0),count(s.size()) {
		buf = new unsigned char[N];
		for (int i = 0; i < count; ++i) {
			buf[i] = s[i];
		}
	}
	ringbuf& operator=(const stroka& s) {
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
	}
	int size() const { return count; }
	void push_back(const unsigned char X) {
		if (count > N) {
			throw "lost elements";
		}
		if (count == N) {
			N = count + 10;
			unsigned char* tmp = new unsigned char[N];
			for (int i = 0; i < count-first; ++i) {
				tmp[i] = buf[i + first];
			}
			for (int i = 0; i < first; ++i) {
				tmp[count - first + i] = buf[i];
			}
			first = 0;
			delete[] buf;
			buf = tmp;
		}
		int index = (first + count) % N;
		buf[index] = X;
		++count;
	}
	unsigned char pop_front() {
		if (count == 0)
			throw "no elements";
		unsigned char ans = buf[first];
		++first;
		first %= N;
		--count;
		return ans;
	}
	unsigned char pop_back() {
		if (count == 0)
			throw "no elements";
		unsigned char ans = buf[(first + count - 1) % N];
		--count;
		return ans;
	}
	void empty() { count = 0; }
	unsigned char operator[](int index) {
		if (index >= count || index < 0)
			throw "out of limits";
		return buf[(first + index) % N];
	}
	unsigned char last() const {
		if (count == 0)
			throw "no elements";
		return buf[(first + count - 1) % N];
	}
};