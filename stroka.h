#pragma once

class stroka {
	unsigned char* str;
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
	unsigned char operator[](int index)const {
		unsigned char res = 0;
		if (index < count && index >= 0) {
			res = str[index];
		}
		return res;
	}
	int size()const {
		return count;
	}
	friend std::ostream& operator<<(std::ostream& out, const stroka& s) {
		for (int i = 0; i < s.count; ++i) {
			if (s.str[i] <= 32)
				out << (int)(s.str[i]) << " ";
			else
				out << s.str[i];
		}
		return out;
	}
};
