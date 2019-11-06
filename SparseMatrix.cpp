// SparseMatrix.cpp : Defines the entry point for the console application.
//

#include "SparseMatrix.hpp"
#include <iostream>
#include <fstream>
#include <chrono>

#include <map>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace SparseMatrix;

template <typename DurType = long double>
struct Timer {
    DurType &duration;
    chrono::system_clock::time_point start;

    // Add the duration of this timer's life to this reference.
    inline explicit Timer(DurType &dur) :
            duration(dur),
            start(start = chrono::system_clock::now()) {}

    inline ~Timer() {
        chrono::duration<DurType> diff = chrono::system_clock::now() - start;
        duration += diff.count();
    }
};

template <typename T>
struct TestSet {

    struct Timings {
        long double creation, sum, product, access;
    };

    explicit TestSet(const char *name) {
        ifstream in(name);
        string line;
        getline(in, line);
        sscanf(line.c_str(), "%d,%d,%d", &p, &q, &r);
        vec_a = file2SparseMatrices(in);
        vec_b = file2SparseMatrices(in);
        vec_c = file2SparseMatrices(in);
        vec_s = file2SparseMatrices(in);
        vec_p = file2SparseMatrices(in);
    }

    template<template<typename...> typename MajorC,
            template<typename...> typename MinorC>
    Timings doTest() const {
        Timings t{};

        {
            Timer<> timer(t.creation);
            SparseMatrix::Mat<T, MajorC, MinorC> a(p, q, 0.0f, vec_a);
            SparseMatrix::Mat<T, MajorC, MinorC> b(q, r, 0.0f, vec_b);
            SparseMatrix::Mat<T, MajorC, MinorC> c(p, q, 0.0f, vec_c);
            SparseMatrix::Mat<T, MajorC, MinorC> s(p, q, 0.0f, vec_s);
            SparseMatrix::Mat<T, MajorC, MinorC> m(p, r, 0.0f, vec_p);
        }
        return t;
    }

private:
    static std::vector<IndexedValue<T>> file2SparseMatrices(ifstream &in) {
        std::vector<IndexedValue<T>> values;
        for (string line; getline(in, line);) {
            IndexedValue<T> iv;
            long double value;
            if (sscanf(line.c_str(), "(%d, %d) %Lf",
                       &iv.x, &iv.y, &value) != 3)
                break;
            iv.value = static_cast<T>(value);
            values.push_back(iv);
        }
        return values;
    }

    unsigned p{}, q{}, r{};
    std::vector<IndexedValue<T>> vec_a;
    std::vector<IndexedValue<T>> vec_b;
    std::vector<IndexedValue<T>> vec_c;
    std::vector<IndexedValue<T>> vec_s;
    std::vector<IndexedValue<T>> vec_p;
};

template<typename T>
void testAndReport(const char *name)
{
    TestSet<T> ts(name);

    auto timings = ts. template doTest<map, map>();
    cout << timings.creation << "  Map-Map\n";

    timings = ts. template doTest<map, unordered_map>();
    cout << timings.creation << "  Map-UOMap\n";

    timings = ts. template doTest<unordered_map, unordered_map>();
    cout << timings.creation << "  UOMap-UOMap\n";
}

int main() {

    const char *name = "/home/ishwark/projects/SparseMatrix/1.txt";

    testAndReport<float>(name);

    testAndReport<double>(name);

    return 0;
}

