#ifndef BITMATRIX
#define BITMATRIX

#include <bitset>
#include <limits>
#include <algorithm>
#include <array>
#include <ranges>
#include <iostream>
#include <cstdint>

template< typename T >
struct bitmatrix
{
    using Row_type = T;
    
    static constexpr
    std::size_t Row_bits_size = std::numeric_limits<Row_type>::digits;
    
    static constexpr
    std::size_t Rows_number = Row_bits_size;

    using _row_packed_t = std::bitset<Row_bits_size>;

    using _matrix_t = std::array<Row_type, Rows_number>;
    alignas(alignof(Row_type) * Rows_number) _matrix_t _matrix;

    bitmatrix( const Row_type (&rows)[Rows_number] )
    {
        std::copy(std::ranges::cbegin(rows),
                  std::ranges::cend(rows),
                  std::ranges::begin(_matrix));
    }

    template< typename Range >
    bitmatrix( const Range &rows )
    {
        std::copy(std::ranges::cbegin(rows),
                  std::ranges::cend(rows),
                  std::ranges::begin(_matrix));
    }

    bitmatrix(const bitmatrix& other) = default;

    bitmatrix& operator|=( const bitmatrix& __restrict other )
    {
        for (auto i = 0; i < Rows_number; i++)
            _matrix[i] |= other._matrix[i];
        return *this;
    }

    constexpr Row_type get_row( std::size_t row ) const
    {
        return _matrix[row];
    }

    constexpr bool at( std::size_t row, std::size_t col ) const
    {
        return (_matrix[row] >> col) & 1;
    }

    Row_type _get_column(std::size_t n) const
    {
        Row_type mask = 1 << n;
        union {
            Row_type result;
            _row_packed_t packed_result;
        } row { .result = 0 };
        for( std::size_t i=0; i < row.packed_result.size(); i++ )
            row.packed_result[i] = get_row(i) & mask;
        return row.result;
    }
    
    static _matrix_t _partial_multiply_add( _matrix_t& __restrict result,
                                            const _matrix_t& X,
                                            const _matrix_t& Y,
                                            std::size_t n )
    {
        _matrix_t Xcopy;
        std::ranges::transform(X, std::ranges::begin(Xcopy),
                               [=](auto x){ return x >> n; });
        _matrix_t Ycopy = Y;
        
        // for (auto i = 0; i < Rows_number; i++)
        //     Xcopy[i] >>= n;

        _matrix_t X_n;
        for (auto i = 0; i < Rows_number; i++)
            X_n[i] = 0 - (Xcopy[i] & 1);

        for (unsigned i = 0; i < Rows_number; i++)
            result[i] |= X_n[i] & Ycopy[n];

        return result;
        
        // Row_type mask = 1 << n;
        // for (unsigned i = 0; i < Rows_number; i++) {
        //     Row_type Xin = (X._array[i] & mask) ? -1 : 0;
        //     Row_type Yn_ = Y._array[n];
        //     result._array[i] |= Xin & Yn_;
        // }
        // return result;
    }

    bitmatrix& multiply( const bitmatrix& other )
    {
        const _matrix_t& X = _matrix;
        const _matrix_t& Y = other._matrix;
        _matrix_t result{ 0 };

        for (std::size_t i=0; i < Rows_number; i++)
            _partial_multiply_add( result, X, Y, i );

        _matrix = result;
        return *this;
    }

    template< typename RT >
    friend std::ostream& operator<< (std::ostream&, const bitmatrix&);
};

template< typename Row_type >
std::ostream& operator<< (std::ostream& stream, const bitmatrix<Row_type>& matrix)
{
    constexpr std::size_t rows_number = bitmatrix<Row_type>::Rows_number;
    
    const char digits[2] = {'0', '1'};
    char row_string[rows_number*2 + 2] = {0};

    for(auto i=0; i < rows_number; i++) {
        for (auto j = 0; j < rows_number; j++) {
            row_string[j*2] = digits[matrix.at(i, j)];
            row_string[j*2+1] = ' ';
        }
        stream << row_string << std::endl;
    }
    return stream;
}

#endif
