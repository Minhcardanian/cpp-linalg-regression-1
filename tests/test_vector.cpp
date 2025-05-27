// tests/test_vector.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Vector.hpp"

TEST_CASE("Vector construction and indexing", "[Vector]") {
    Vector v(3);
    REQUIRE(v.size() == 3);

    // 0-based access
    v[0] = 1.5; 
    v[1] = -2.5; 
    v[2] = 0.0;
    REQUIRE(v[0] == Approx(1.5));
    REQUIRE(v[1] == Approx(-2.5));
    REQUIRE(v[2] == Approx(0.0));

    // 1-based access
    v(3) = 9.0;
    REQUIRE(v(3) == Approx(9.0));
}

TEST_CASE("Vector arithmetic", "[Vector]") {
    Vector a(2), b(2);
    a[0] = 1; a[1] = 2;
    b[0] = 3; b[1] = 4;

    auto c = a + b;
    REQUIRE(c[0] == 4);
    REQUIRE(c[1] == 6);

    auto d = a * 2.5;
    REQUIRE(d[0] == Approx(2.5));
    REQUIRE(d[1] == Approx(5.0));
}

TEST_CASE("Vector unary operators", "[Vector]") {
    Vector v(2);
    v[0] = 5; v[1] = -5;

    auto pos = +v;
    auto neg = -v;

    REQUIRE(pos[0] == Approx(5.0));
    REQUIRE(pos[1] == Approx(-5.0));
    REQUIRE(neg[0] == Approx(-5.0));
    REQUIRE(neg[1] == Approx(5.0));
}

TEST_CASE("Vector out-of-range access throws", "[Vector]") {
    Vector v(2);
    REQUIRE_THROWS_AS(v[2], std::out_of_range);   // 0-based OOB
    REQUIRE_THROWS_AS(v(0), std::out_of_range);   // 1-based OOB
    REQUIRE_THROWS_AS(v(3), std::out_of_range);   // 1-based OOB
}
