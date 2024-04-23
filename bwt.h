#pragma once
#include <vector>
#include <string>
#include "cstream.h"

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



class BWT : public cstream {
    std::string word;
    int numOfStr;
    std::string transform(std::string s) {
        std::vector<std::string> transformationMatrix;
        size_t len = s.size();
        transformationMatrix.resize(len);
        transformationMatrix[0] = s;
        for (size_t i = 0; i < len; ++i) {
            shiftString(s);
            transformationMatrix[i] = s;
        }

        sort(transformationMatrix);

        for (size_t i = 0; i < len; ++i) {
            if (s == transformationMatrix[i]) {
                numOfStr = i;
                break;
            }
        }
        std::string res;
        res.reserve(len);
        for (size_t i = 0; i < len; ++i) {
            res += transformationMatrix[i].back();
        }
        return res;
    }
    void shiftString(std::string& given) {
        char lastSymb = given.back();
        given.pop_back();
        given = lastSymb + given;
    }
public:
    BWT(cstream* s) : cstream(s) {}
    int get() {
        static int index = -1;
        int count = 0;
        if (index != word.size() - 1) {
            return word[++index];
        }
        word.clear();
        do {
            int ch = prev->get();
            if (ch == EOF) {
                break;
            }
            word.push_back(static_cast<char>(ch));
        } while (++count < 12);
        word = transform(word);

        return word[++index];
    }

};
