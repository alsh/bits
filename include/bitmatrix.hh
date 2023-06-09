#ifndef BITMATRIX
#define BITMATRIX

#include <bitset>
#include <algorithm>
#include <array>
#include <ranges>
#include <ostream>

template< std::size_t Size >
struct bitmatrix
{
    using row_type = std::bitset<Size>;
    using _matrix_t = std::array<row_type, Size>;

    alignas(alignof(row_type) * Size) _matrix_t _matrix;

    // Default constructor
    bitmatrix() = default;

    template< typename T >
    explicit bitmatrix( const T (&rows)[Size] )
    {
        std::copy(std::ranges::cbegin(rows),
                  std::ranges::cend(rows),
                  std::ranges::begin(_matrix));
    }

    template< std::ranges::range R >
    bitmatrix( const R &rows )
    {
        std::copy(std::ranges::cbegin(rows),
                  std::ranges::cend(rows),
                  std::ranges::begin(_matrix));
    }

    // Default copy constructor
    bitmatrix(const bitmatrix& other) = default;
    // Default move constructor
    bitmatrix(bitmatrix&& other) = default;

    // Default assignment operators
    bitmatrix& operator=(const bitmatrix& other) = default;
    // Default move assignment operators
    bitmatrix& operator=(bitmatrix&& other) = default;
    
    // Default comparison operators
    bool operator==(const bitmatrix& other) const = default;
    bool operator!=(const bitmatrix& other) const = default;

    bitmatrix& operator|=( const bitmatrix& __restrict other )
    {
        for (auto i = 0; i < Size; i++)
            _matrix[i] |= other._matrix[i];
        return *this;
    }

    bitmatrix& operator&=( const bitmatrix& __restrict other )
    {
        for (auto i = 0; i < Size; i++)
            _matrix[i] &= other._matrix[i];
        return *this;
    }

    // operator |
    friend bitmatrix operator|( const bitmatrix& __restrict lhs,
                                const bitmatrix& __restrict rhs )
    {
        bitmatrix result = lhs;
        result |= rhs;
        return result;
    }

    // operator &
    friend bitmatrix operator&( const bitmatrix& __restrict lhs,
                                const bitmatrix& __restrict rhs )
    {
        bitmatrix result = lhs;
        result &= rhs;
        return result;
    }

    // operator +, alias for operator |
    friend bitmatrix operator+( const bitmatrix& __restrict lhs,
                                const bitmatrix& __restrict rhs )
    {
        return lhs | rhs;
    }

    // operator *, used for multiplication
    friend bitmatrix operator*( const bitmatrix& __restrict lhs,
                                const bitmatrix& __restrict rhs )
    {
        bitmatrix result = lhs;
        result.multiply(rhs);
        return result;
    }
    

    constexpr row_type row( std::size_t row ) const
    {
        return _matrix[row];
    }

    constexpr bool at( std::size_t row, std::size_t col ) const
    {
        return _matrix[row][col];
    }

    typename row_type::reference at( std::size_t row, std::size_t col )
    {
        return _matrix[row][col];
    }

    row_type column(std::size_t n) const
    {
        row_type column = 0;
        for (std::size_t i = 0; i < Size; i++)
            column[i] = _matrix[i][n];
        return column;
    }

    bitmatrix& multiply( const bitmatrix& other )
    {
        const row_type zeros = row_type().reset();
        const row_type ones = row_type().set();

        const _matrix_t& X = _matrix;
        const _matrix_t& Y = other._matrix;
        _matrix_t result{ 0 };

        
        for (std::size_t n=0; n < Size; n++)
            for (std::size_t i = 0; i < Size; i++)
                result[i] |= (X[i][n] ? ones : zeros) & Y[n];

        _matrix = result;
        return *this;
    }

    template< typename RT >
    friend std::ostream& operator<< (std::ostream&, const bitmatrix&);
};

template< std::size_t Size >
std::ostream& operator<< (std::ostream& stream, const bitmatrix<Size>& matrix)
{    
    const char digits[2] = {'0', '1'};
    char row_string[Size + 2] = {0};

    for(std::size_t i=0; i < Size; i++) {
        for (std::size_t j = 0; j < Size; j++) {
            row_string[j] = digits[matrix.at(i, j)];
        }
        stream << row_string << std::endl;
    }
    return stream;
}

#endif
