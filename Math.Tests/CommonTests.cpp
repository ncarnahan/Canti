#include <Math/Math.h>
using namespace Math;
#include <Catch/Catch.hpp>

TEST_CASE("Degree Trigonometric Functions", "[Common]")
{
    REQUIRE(SinDeg(0) == 0);
    REQUIRE(SinDeg(90) == 1);
    REQUIRE(SinDeg(180) == 0);
    REQUIRE(SinDeg(270) == -1);
    REQUIRE(SinDeg(360) == 0);

    REQUIRE(CosDeg(0) == 1);
    REQUIRE(CosDeg(90) == 0);
    REQUIRE(CosDeg(180) == -1);
    REQUIRE(CosDeg(270) == 0);
    REQUIRE(CosDeg(360) == 1);

    REQUIRE(TanDeg(0) == 0);
    REQUIRE(TanDeg(45) == 1);
    REQUIRE(TanDeg(135) == -1);
    REQUIRE(TanDeg(180) == 0);
}