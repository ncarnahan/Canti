#include <Math/Math.h>
using namespace Math;
#include <Catch/Catch.hpp>

TEST_CASE("Degree Trigonometric Functions", "[Common]")
{
    REQUIRE(FloatEq(SinDeg(0), 0.0f));
    REQUIRE(FloatEq(SinDeg(90), 1.0f));
    REQUIRE(FloatEq(SinDeg(180), 0.0f));
    REQUIRE(FloatEq(SinDeg(270), -1.0f));
    REQUIRE(FloatEq(SinDeg(360), 0.0f));

    REQUIRE(FloatEq(CosDeg(0), 1.0f));
    REQUIRE(FloatEq(CosDeg(90), 0.0f));
    REQUIRE(FloatEq(CosDeg(180), -1.0f));
    REQUIRE(FloatEq(CosDeg(270), 0.0f));
    REQUIRE(FloatEq(CosDeg(360), 1.0f));

    REQUIRE(FloatEq(TanDeg(0), 0.0f));
    REQUIRE(FloatEq(TanDeg(45), 1.0f));
    REQUIRE(FloatEq(TanDeg(135), -1.0f));
    REQUIRE(FloatEq(TanDeg(180), 0.0f));
}