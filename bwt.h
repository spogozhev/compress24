#pragma once
#include <vector>
#include <deque>
#include <algorithm>
#include "cstream.h"

class BWT : public cstream {
    std::deque<int> word;
    int numOfStr;
    int sizeOfBlock;
    void transform(std::deque<int>& s) {
        std::vector<std::deque<int>> transformationMatrix;
        size_t len = s.size();
        transformationMatrix.resize(len);
        transformationMatrix[0] = s;
        for (size_t i = 1; i < len; ++i) {
            shiftDeque(s);
            transformationMatrix[i] = s;
        }
        shiftDeque(s);
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
        s.push_front(numOfStr);
    }
    void shiftDeque(std::deque<int>& given) {
        int lastSymb = given.back();
        given.pop_back();
        given.push_front(lastSymb);
    }
public:
    BWT(cstream* s, int k = 256) : cstream(s), sizeOfBlock(k) {}
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
        } while (++count < sizeOfBlock);
        transform(word);
        return word[++index];
    }
};

class deBWT : public cstream {
    int numOfCol;
    int sizeOfBlock;
    std::vector<std::deque<int>> table;
    void transform(std::deque<int>& word) {
        size_t sizeOfWord = word.size();
        if (numOfCol == sizeOfWord) {
            return;
        }
        table.resize(sizeOfWord);
        for (size_t i = 0; i < sizeOfWord; ++i) {
            table[i].clear();
        }
        for (size_t i = 0; i < sizeOfWord; ++i) {
            for (size_t j = 0; j < sizeOfWord; ++j) {
                table[j].push_front(word[j]);
            }
            std::sort(table.begin(), table.end());
        }
        word = table[numOfCol];
    }
public:
    deBWT(cstream* s, int k = 256) : cstream(s), sizeOfBlock(k) {}
    bool is_open() {
        return prev->is_open();
    }
    int get() {
        static int index1 = 0;
        static int count = 0;
        static std::deque<int> word;
        if (index1 >= count || index1 == 0) {
            word.clear();
            index1 = 0;
            count = 0;
            if (numOfCol != EOF) numOfCol = prev->get();
            if (numOfCol != EOF) {
                do {
                    int ch = prev->get();
                    if (ch == EOF) {
                        if (!word.empty()) {
                            transform(word);
                            numOfCol = EOF;
                            return word[index1++];
                        }
                        numOfCol = EOF;
                        return EOF;
                    }
                    word.push_back(static_cast<char>(ch));
                } while (++count < sizeOfBlock);
            }
            else {
                return EOF;
            }
            transform(word);
            return word[index1++];
        }
        else {
            return word[index1++];
        }
    }
};
