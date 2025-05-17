// tests/test_data.cpp
#include <catch2/catch.hpp>
#include <fstream>
#include <sstream>
#include <string>

TEST_CASE("Data file parse sanity", "[Data]") {
    std::ifstream in("data/machine.data");
    REQUIRE( in.is_open() );

    std::string line;
    size_t rowCount = 0;
    while (std::getline(in, line)) {
        REQUIRE_FALSE(line.empty());
        std::stringstream ss(line);
        std::string field;
        size_t colCount = 0;
        while (std::getline(ss, field, ',')) {
            ++colCount;
        }
        REQUIRE( colCount == 10 );
        ++rowCount;
    }
    REQUIRE( rowCount == 209 );
}
