#pragma once
#include <fstream>

class cstream {
protected:
	cstream* prev;
public:
	cstream(cstream* s = nullptr) : prev(s) {}
	virtual int get() = 0; // pure virtual
	virtual bool is_open() = 0; // pure virtual
	virtual ~cstream() {
//		std::cout << "Kill like cstream\n";
		delete prev;
	}
};

/*
class plus1cstream : public cstream {
public:
	plus1cstream(cstream* s) : cstream(s) {}
	int get() {
		int tmp = prev->get();
		if (tmp != EOF) {
			++tmp;
		}
		return tmp;
	}
	bool is_open() { return prev->is_open(); }
	~plus1cstream() {
//		std::cout << "Kill like plus1cstream\n";
	}
};

class toUppercstream : public cstream {
public:
	toUppercstream(cstream* s) : cstream(s) {}
	bool is_open() { return prev->is_open(); }
	int get() {
		int tmp = prev->get();
		if (tmp >= 'a' && tmp <= 'z') {
			tmp = (tmp - 'a') + 'A';
		}
		return tmp;
	}
};
*/