#include <cstdint>
#include <iostream>
#include <string>
#include <random>
#include <ranges>
#include <cassert>

#include <bitmatrix.hh>


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

// Helper function to generate a random bits::small_matrix
template<std::size_t Size>
bits::small_matrix<Size> random_small_matrix()
{
    std::random_device rd;
    std::uniform_int_distribution<uint8_t> dist(0, 1);

    std::string random_string(Size, '0');
    std::array<std::bitset<Size>, Size> random_bitset_array;
    
    for (auto &bitset : random_bitset_array)
    {
        std::ranges::generate(random_string, [&]() { return dist(rd) + '0'; });
        bitset = std::bitset<Size>(random_string);
    }        

    return bits::small_matrix<Size>(random_bitset_array);
}

#define TEST_MATRIX_TYPES                       \
    bits::small_matrix<8>, bits::small_matrix<16>, bits::small_matrix<32>, bits::small_matrix<64>

TEST_CASE_TEMPLATE("Identity test", Bitmatrix, TEST_MATRIX_TYPES)
{
    constexpr std::size_t size = Bitmatrix().size();
    Bitmatrix identity_matrix = Bitmatrix::identity();

    auto random_Bitmatrix = random_small_matrix<size>;

    Bitmatrix random_matrix = random_Bitmatrix();
    Bitmatrix result1 = random_matrix;
    Bitmatrix result2 = identity_matrix;

    SUBCASE("In-place multiplication by identity matrix")
    {
        result1.multiply(identity_matrix);
        result2.multiply(random_matrix);

        CHECK(result1 == random_matrix);
        CHECK(result2 == random_matrix);
    }

    SUBCASE("Multiplication by identity matrix")
    {
        result1 = identity_matrix * random_matrix;
        result2 = random_matrix * identity_matrix;

        CHECK(result1 == random_matrix);
        CHECK(result2 == random_matrix);
    }
}

template< std::size_t Size >
bits::small_matrix<Size> dumb_multiply(const bits::small_matrix<Size> &a, const bits::small_matrix<Size> &b)
{
    bits::small_matrix<Size> result;
    for (std::size_t i = 0; i < Size; ++i)
        for (std::size_t j = 0; j < Size; ++j)
            for (std::size_t k = 0; k < Size; ++k)
                result.at(i, j) = result.at(i, j) || (a.at(i, k) && b.at(k, j));
    return result;
}

TEST_CASE_TEMPLATE("Multiplication functionality", Bitmatrix, TEST_MATRIX_TYPES)
{
    constexpr std::size_t size = Bitmatrix().size();
    Bitmatrix input1 = random_small_matrix<size>();
    Bitmatrix input2 = random_small_matrix<size>();

    SUBCASE("In-place multiplication")
    {
        Bitmatrix result1 = input1;
        Bitmatrix result2;

        result1.multiply(input2);
        result2 = dumb_multiply(input1, input2);

        CHECK(result1 == result2);
    }

    SUBCASE("Multiplication")
    {
        Bitmatrix result1 = input1 * input2;
        Bitmatrix result2 = dumb_multiply(input1, input2);

        CHECK(result1 == result2);
    }
}

TEST_CASE("Improper initialization")
{
    using bitmatrix8 = bits::small_matrix<8>;

    bitmatrix8 too_small_initializer({1, 2, 3, 4});
    CHECK(too_small_initializer == bitmatrix8({1, 2, 3, 4, 0, 0, 0, 0}));

    bitmatrix8 too_big_initializer({1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK(too_big_initializer == bitmatrix8({1, 2, 3, 4, 5, 6, 7, 8}));

    std::array<uint8_t, 4> small_initializer = {1, 2, 3, 4};
    bitmatrix8 too_small_ranged_initializer2(small_initializer);
    CHECK(too_small_ranged_initializer2 == bitmatrix8({1, 2, 3, 4, 0, 0, 0, 0}));
    
    std::array<uint8_t, 9> big_initializer = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    bitmatrix8 too_big_ranged_initializer(big_initializer);
    CHECK(too_big_ranged_initializer == bitmatrix8({1, 2, 3, 4, 5, 6, 7, 8}));
}
