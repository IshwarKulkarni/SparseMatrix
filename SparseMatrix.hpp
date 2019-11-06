#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <vector>
#include <algorithm>

namespace SparseMatrix {
    template<typename T>
    struct IndexedValue {
        unsigned x{}, y{};
        T value{};
    };

    template<typename T,
            template<typename...> typename MajorC,
            template<typename...> typename MinorC>
    struct Mat {
        template<typename T1, template<typename...> typename C>
        using Array = C<unsigned, T1>;

        const unsigned _width, _height;
        const T &_empty;

        Mat(unsigned width, unsigned height, const T &empty,
            const std::vector<IndexedValue<T>> &values) :
                _width(width),
                _height(height),
                _empty(empty) {
                for (auto &v: values) {
                        _mat[v.y][v.x] = v.value;
                        _matTx[v.x][v.y] = v.value;
                }
        }

        Mat(unsigned width, unsigned height, std::vector<IndexedValue<T>> iv) :
                _width(width),
                _height(height) {
            for (auto &v: iv)
            {
                static const auto lessX = [](const IndexedValue<T>& _1, const IndexedValue<T>& _2)
                {
                    return  _1.x == _2.x ? _1.y < _2.y : _1.x < _2.x;
                };

                static const auto lessY = [](const IndexedValue<T>& _1, const IndexedValue<T>& _2)
                {
                    return  _1.y == _2.y ? _1.x < _2.x : _1.y < _2.y;
                };


                std::sort(iv.begin(), iv.end(), lessX);
                auto prevRow = _mat.insert({iv[0].y, {iv[0].x, iv[0].values}});
                

            }
        }

    private:

        MinorC<unsigned , Array<T, MajorC> > _mat{};
        MinorC<unsigned, Array<T, MajorC> > _matTx{};

    };

}

#endif