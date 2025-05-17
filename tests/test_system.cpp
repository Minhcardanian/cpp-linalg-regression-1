// tests/test_system.cpp
#include <catch2/catch.hpp>
#include "LinearSystem.hpp"

// 2×2 diagonal system: [2,0;0,3] x = [4,6] → x=[2,2]
TEST_CASE("Gaussian elimination solves diagonal system", "[LinearSystem]") {
    Matrix A(2,2);
    Vector b(2);
    A(1,1)=2; A(2,2)=3; b[0]=4; b[1]=6;
    LinearSystem sys(A,b);
    auto x = sys.Solve();
    REQUIRE( x[0] == Approx(2.0) );
    REQUIRE( x[1] == Approx(2.0) );
}

TEST_CASE("Conjugate Gradient solves symmetric system", "[PosSymLinSystem]") {
    Matrix A(2,2);
    Vector b(2);
    A(1,1)=3; A(1,2)=1; A(2,1)=1; A(2,2)=2;
    b[0]=5; b[1]=5;
    PosSymLinSystem cg(A,b);
    auto x = cg.Solve();
    // Known solution [1,2]
    REQUIRE( x[0] == Approx(1.0).margin(1e-6) );
    REQUIRE( x[1] == Approx(2.0).margin(1e-6) );
}
