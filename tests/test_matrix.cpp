// tests/test_matrix.cpp
#include <catch2/catch.hpp>
#include "Matrix.hpp"

TEST_CASE("Matrix construction and element access", "[Matrix]") {
    Matrix m(2,3);
    REQUIRE(m.rows() == 2);
    REQUIRE(m.cols() == 3);
    // Should initialize to zero
    for (std::size_t i=1;i<=2;++i)
      for (std::size_t j=1;j<=3;++j)
        REQUIRE(m(i,j) == Approx(0.0));
    // Out‐of‐range
    REQUIRE_THROWS_AS( m(0,1), std::out_of_range );
    REQUIRE_THROWS_AS( m(1,0), std::out_of_range );
    REQUIRE_THROWS_AS( m(3,1), std::out_of_range );
}

TEST_CASE("Matrix arithmetic", "[Matrix]") {
    Matrix A(2,2), B(2,2);
    A(1,1)=1; A(1,2)=2; A(2,1)=3; A(2,2)=4;
    B(1,1)=5; B(1,2)=6; B(2,1)=7; B(2,2)=8;

    auto C = A + B;
    REQUIRE( C(1,1) == 6 ); REQUIRE( C(2,2) == 12 );

    auto D = A * B;
    // D = [[19,22],[43,50]]
    REQUIRE( D(1,1) == 19 ); REQUIRE( D(1,2) == 22 );
    REQUIRE( D(2,1) == 43 ); REQUIRE( D(2,2) == 50 );
}

TEST_CASE("Scalar multiplication", "[Matrix]") {
    Matrix M(2,2);
    M(1,1)=2; M(1,2)=3; M(2,1)=4; M(2,2)=5;
    auto S = M * 2.0;
    REQUIRE( S(1,1) == 4 ); REQUIRE( S(1,2) == 6 );
    REQUIRE( S(2,1) == 8 ); REQUIRE( S(2,2) == 10 );
}

TEST_CASE("Matrix assignment copies values", "[Matrix]") {
    Matrix A(2,2), B(2,2);
    A(1,1)=1; A(1,2)=2; A(2,1)=3; A(2,2)=4;
    B(1,1)=5; B(1,2)=6; B(2,1)=7; B(2,2)=8;

    A = B;
    REQUIRE(A.rows() == 2);
    REQUIRE(A.cols() == 2);
    for (std::size_t i=1;i<=2;++i)
        for (std::size_t j=1;j<=2;++j)
            REQUIRE(A(i,j) == Approx(B(i,j)));
}

TEST_CASE("Matrix assignment reallocates for size change", "[Matrix]") {
    Matrix A(2,3);
    Matrix B(3,1);
    B(1,1)=1; B(2,1)=2; B(3,1)=3;

    A = B;
    REQUIRE(A.rows() == 3);
    REQUIRE(A.cols() == 1);
    for (std::size_t i=1;i<=3;++i)
        REQUIRE(A(i,1) == Approx(B(i,1)));
}
