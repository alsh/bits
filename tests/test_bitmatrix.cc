#include <cstdint>
#include <iostream>
#include <random>
#include <ranges>
#include <cassert>

#include <bitmatrix.hh>


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

const std::size_t size = 8;
using bitmatrix8 = bitmatrix<size>;

// Helper function to generate a random bitmatrix
bitmatrix8 random_bitmatrix()
{
    // A code to create an array of uint8_t with random numbers using modern C++
    std::random_device rd;
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    uint8_t random_array[size];

    std::ranges::generate(random_array, [&]() { return dist(rd); });
    return bitmatrix8(random_array);
}

TEST_CASE("Identity test")
{
    bitmatrix8 identity_matrix({ 1, 2, 4, 8, 16, 32, 64, 128 });

    bitmatrix8 random_matrix = random_bitmatrix();
    bitmatrix8 result1 = random_matrix;
    bitmatrix8 result2 = identity_matrix;

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

bitmatrix8 dumb_multiply(const bitmatrix8 &a, const bitmatrix8 &b)
{
    bitmatrix8 result;
    for (std::size_t i = 0; i < size; ++i)
        for (std::size_t j = 0; j < size; ++j)
            for (std::size_t k = 0; k < size; ++k)
                result.at(i, j) = result.at(i, j) || (a.at(i, k) && b.at(k, j));
    return result;
}

TEST_CASE("Check multiplication")
{
    bitmatrix8 input1 = random_bitmatrix();
    bitmatrix8 input2 = random_bitmatrix();

    SUBCASE("In-place multiplication")
    {
        bitmatrix8 result1 = input1;
        bitmatrix8 result2;

        result1.multiply(input2);
        result2 = dumb_multiply(input1, input2);

        CHECK(result1 == result2);
    }

    SUBCASE("Multiplication")
    {
        bitmatrix8 result1 = input1 * input2;
        bitmatrix8 result2 = dumb_multiply(input1, input2);

        CHECK(result1 == result2);
    }
}
