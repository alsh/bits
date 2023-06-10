#ifndef BITMATRIX
#define BITMATRIX

#include <algorithm>
#include <array>
#include <bitset>
#include <ostream>
#include <ranges>

namespace bits
{

template <std::size_t Size> struct alignas(alignof(std::bitset<Size>)) small_matrix
{
    using row_type = std::bitset<Size>;
    using size_type = std::size_t;
    using _matrix_t = std::array<row_type, Size>;

    alignas(alignof(row_type)) _matrix_t _matrix;

    // Default constructor
    small_matrix() = default;

    template <typename T, std::size_t S> explicit constexpr small_matrix(const T (&rows)[S])
    {
        auto safe_size = std::min(std::ranges::size(rows), std::ranges::size(_matrix));
        std::copy(std::ranges::cbegin(rows), std::ranges::cbegin(rows) + safe_size,
                  std::ranges::begin(_matrix));
    }

    template <std::ranges::range R> constexpr small_matrix(const R &rows)
    {
        auto safe_size = std::min(std::ranges::size(rows), std::ranges::size(_matrix));
        std::copy(std::ranges::cbegin(rows), std::ranges::cbegin(rows) + safe_size,
                  std::ranges::begin(_matrix));
    }

    // Default copy constructor
    constexpr small_matrix(const small_matrix &other) = default;
    // Default move constructor
    constexpr small_matrix(small_matrix &&other) = default;

    // Default assignment operators
    constexpr small_matrix &operator=(const small_matrix &other) = default;
    // Default move assignment operators
    constexpr small_matrix &operator=(small_matrix &&other) = default;

    constexpr size_type size() const
    {
        return Size;
    }

    static constexpr small_matrix identity()
    {
        std::size_t start = 0;
        std::size_t bound = Size;
        auto gen_f = [](unsigned i) constexpr { return std::bitset<Size>{1} << i; };
        auto init_range = std::views::iota(start, bound) | std::views::transform(gen_f);

        return small_matrix(init_range);
    }

    // Default comparison operators
    bool operator==(const small_matrix &other) const = default;
    bool operator!=(const small_matrix &other) const = default;

    constexpr small_matrix &operator|=(const small_matrix &__restrict other)
    {
        for (auto i = 0; i < Size; i++)
            _matrix[i] |= other._matrix[i];
        return *this;
    }

    constexpr small_matrix &operator&=(const small_matrix &__restrict other)
    {
        for (auto i = 0; i < Size; i++)
            _matrix[i] &= other._matrix[i];
        return *this;
    }

    // operator |
    constexpr friend small_matrix operator|(const small_matrix &__restrict lhs,
                                            const small_matrix &__restrict rhs)
    {
        small_matrix result = lhs;
        result |= rhs;
        return result;
    }

    // operator &
    constexpr friend small_matrix operator&(const small_matrix &__restrict lhs,
                                            const small_matrix &__restrict rhs)
    {
        small_matrix result = lhs;
        result &= rhs;
        return result;
    }

    // operator +, alias for operator |
    constexpr friend small_matrix operator+(const small_matrix &__restrict lhs,
                                            const small_matrix &__restrict rhs)
    {
        return lhs | rhs;
    }

    // operator *, used for multiplication
    constexpr friend small_matrix operator*(const small_matrix &__restrict lhs,
                                            const small_matrix &__restrict rhs)
    {
        small_matrix result = lhs;
        result.multiply(rhs);
        return result;
    }

    // operator *=
    constexpr small_matrix &operator*=(const small_matrix &__restrict other)
    {
        return multiply(other);
    }

    constexpr row_type row(std::size_t row) const
    {
        return _matrix[row];
    }

    constexpr bool at(std::size_t row, std::size_t col) const
    {
        return _matrix[row][col];
    }

    constexpr typename row_type::reference at(std::size_t row, std::size_t col)
    {
        return _matrix[row][col];
    }

    constexpr row_type column(std::size_t n) const
    {
        row_type column = 0;
        for (std::size_t i = 0; i < Size; i++)
            column[i] = _matrix[i][n];
        return column;
    }

    constexpr small_matrix &multiply(const small_matrix &other)
    {
        const row_type zeros = row_type().reset();
        const row_type ones = row_type().set();

        const _matrix_t &X = _matrix;
        const _matrix_t &Y = other._matrix;
        _matrix_t result{0};

        for (std::size_t n = 0; n < Size; n++)
            for (std::size_t i = 0; i < Size; i++)
                result[i] |= (X[i][n] ? ones : zeros) & Y[n];

        _matrix = result;
        return *this;
    }

    template <typename RT> friend std::ostream &operator<<(std::ostream &, const small_matrix &);
};

template <std::size_t Size>
std::ostream &operator<<(std::ostream &stream, const small_matrix<Size> &matrix)
{
    const char digits[2] = {'0', '1'};
    char row_string[Size + 2] = {0};

    for (std::size_t i = 0; i < Size; i++)
    {
        for (std::size_t j = 0; j < Size; j++)
        {
            row_string[j] = digits[matrix.at(i, j)];
        }
        stream << row_string << std::endl;
    }
    return stream;
}

} // namespace bits

#endif
