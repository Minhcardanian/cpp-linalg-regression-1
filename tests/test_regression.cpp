// tests/test_regression.cpp
#include <catch2/catch.hpp>
#include "Matrix.hpp"
#include "Vector.hpp"
#include "LinearSystem.hpp"

// Synthetic regression: y = 2*x + 3
TEST_CASE("Regression solver recovers synthetic line", "[Regression]") {
    const size_t N = 5;
    // Build X (N×2) and y (length N)
    Matrix X(N,2);
    Vector y(N);
    for (size_t i=1; i<=N; ++i) {
        double xi = double(i);
        X(i,1) = xi;
        X(i,2) = 1.0;
        y[i-1] = 2.0*xi + 3.0;
    }
    // Normal equations A = X^T X, b = X^T y
    Matrix AtA(2,2);
    Vector Aty(2);
    for (size_t i=1;i<=2;++i)
      for (size_t j=1;j<=2;++j)
        for (size_t k=1;k<=N;++k)
          AtA(i,j) += X(k,i) * X(k,j);
    for (size_t i=1;i<=2;++i)
      for (size_t k=1;k<=N;++k)
        Aty[i-1] += X(k,i) * y[k-1];

    PosSymLinSystem sys(AtA, Aty);
    auto coeff = sys.Solve();
    REQUIRE( coeff[0] == Approx(2.0).margin(1e-6) );  // slope
    REQUIRE( coeff[1] == Approx(3.0).margin(1e-6) );  // intercept
}
