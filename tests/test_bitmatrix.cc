#include <cstdint>
#include <iostream>
#include <random>
#include <ranges>
#include <cassert>

#include <bitmatrix.hh>


#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>


TEST_CASE("Identity test")
{
    bitmatrix<uint8_t> identity_matrix({ 1, 2, 4, 8, 16, 32, 64, 128 });

    // A code to create an array of uint8_t with random numbers using modern C++
    std::random_device rd;
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    uint8_t random_array[8];

    std::ranges::generate(random_array, [&]() { return dist(rd); });
    bitmatrix<uint8_t> random_matrix(random_array);

    bitmatrix<uint8_t> result1 = random_matrix;
    bitmatrix<uint8_t> result2 = identity_matrix;

    result1.multiply(identity_matrix);
    result2.multiply(random_matrix);

    CHECK(result1 == random_matrix);
    CHECK(result2 == random_matrix);
}
