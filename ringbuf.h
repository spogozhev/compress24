#pragma once

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

