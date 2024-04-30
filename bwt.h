#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "cstream.h"
/*
void quickSort(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
	if (begin >= end) {
		return;
	}
	std::string pivot = *begin;
	auto left = begin;
	auto right = end;
	while (left < right) {
		while (*right >= pivot && left < right) {
			--right;
		}
		while (*left <= pivot && left < right) {
			++left;
		}
		if (left < right) {
			std::swap(*left, *right);
		}
	}
	*begin = *left;
	*left = pivot;
	quickSort(begin, left - 1);
	quickSort(left + 1, end);
}

void sort(std::vector<std::string>& arr) {
	quickSort(begin(arr), end(arr));
}
*/

class BWT : public cstream {
	std::string word;
	int numOfStr;
	void transform(std::string& s) {
		std::vector<std::string> transformationMatrix;
		size_t len = s.size();
		transformationMatrix.resize(len);
		transformationMatrix[0] = s;
		for (size_t i = 1; i < len; ++i) {
			shiftString(s);
			transformationMatrix[i] = s;
		}
		shiftString(s);
		std::sort(transformationMatrix.begin(), transformationMatrix.end());

		for (size_t i = 0; i < len; ++i) {
			if (s == transformationMatrix[i]) {
				numOfStr = static_cast<int>(i);
				break;
			}
		}
		for (size_t i = 0; i < len; ++i) {
			s[i] = transformationMatrix[i].back();
		}
		s = static_cast<char>(numOfStr) + s;
	}
	void shiftString(std::string& given) {
		char lastSymb = given.back();
		given.pop_back();
		given = lastSymb + given;
	}
public:
	BWT(cstream* s) : cstream(s) {}
	bool is_open() {
		return prev->is_open();
	}
	int get() {
		static int index = -1;
		int count = 0;
		if (index != word.size() - 1) {
			return word[++index];
		}
		word.clear();
		index = -1;
		do {
			int ch = prev->get();
			if (ch == EOF) {
				if (word.empty()) {
					return EOF;
				}
				break;
			}
			word.push_back(static_cast<char>(ch));
		} while (++count < 12);
		transform(word);
		return word[++index];
	}
};

class deBWT : public cstream {
	int numOfCol;
	std::vector<std::string> table;
	std::string transform(std::string word) {
		int sizeOfWord = word.size();
		if (numOfCol == sizeOfWord) {
			return word;
		}
		table.resize(sizeOfWord);
		for (int i = 0; i < sizeOfWord; ++i) {
			table[i] = "";
		}
		std::vector<std::string> helpful(sizeOfWord, "");
		for (int i = 0; i < sizeOfWord; ++i) {
			for (int j = 0; j < sizeOfWord; ++j) {
				table[j] = word[j] + table[j];
			}
			std::sort(table.begin(), table.end());
		}
		return (table[numOfCol]);
	}
public:
	deBWT(cstream* s) : cstream(s) {}
	bool is_open() {
		return prev->is_open();
	}
	int get() {
		static int index1 = 0;
		static int count = 0;
		static std::string word = "";
		if (index1>=count || index1==0){
			word = "";
			index1 = 0;
			count = 0;
			if (numOfCol!=EOF) numOfCol = prev->get();
			if (numOfCol != EOF) {
				do {
					int ch = prev->get();
					if (ch == EOF) {
						if (word != "") {
							word = transform(word);
							numOfCol = EOF;
							return word[index1++];
						}
						numOfCol = EOF;
						return EOF;
					}
					word.push_back(static_cast<char>(ch));
				} while (++count < 12);
			}
			else {
				return EOF;
			}
			word = transform(word);
			return word[index1++];
		}
		else {
			return word[index1++];
		}
	}
};
