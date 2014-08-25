#include <Math/Math.h>
using namespace Math;
#include <Catch/Catch.hpp>

TEST_CASE("Quaternion construction", "[Quaternion]")
{
    REQUIRE(Quaternion::AngleAxis(0, Vector3::up) == Quaternion(1, 0, 0, 0));
    REQUIRE(Quaternion::AngleAxis(45, Vector3::up) == Quaternion(0.9238795f, 0, 0.3826835f, 0));
    REQUIRE(Quaternion::AngleAxis(90, Vector3::up) == Quaternion(0.7071068f, 0, 0.7071068f, 0));
    REQUIRE(Quaternion::AngleAxis(180, Vector3::up) == Quaternion(0, 0, 1, 0));
    
    REQUIRE(Quaternion::AngleAxis(45, Vector3::right) == Quaternion(0.9238795f, 0.3826835f, 0, 0));
    REQUIRE(Quaternion::AngleAxis(90, Vector3::right) == Quaternion(0.7071068f, 0.7071068f, 0, 0));
    REQUIRE(Quaternion::AngleAxis(180, Vector3::right) == Quaternion(0, 1, 0, 0));
    
    REQUIRE(Quaternion::AngleAxis(45, Vector3::forward) == Quaternion(0.9238795f, 0, 0, 0.3826835f));
    REQUIRE(Quaternion::AngleAxis(90, Vector3::forward) == Quaternion(0.7071068f, 0, 0, 0.7071068f));
    REQUIRE(Quaternion::AngleAxis(180, Vector3::forward) == Quaternion(0, 0, 0, 1));
}

TEST_CASE("Quaternion Multiplication", "[Quaternion]")
{
    REQUIRE((Quaternion::AngleAxis(30, Vector3::up) * Quaternion::AngleAxis(45, Vector3::right)) ==
        Quaternion(0.8923991f, 0.3696438f, 0.2391176f, -0.09904577f));

    REQUIRE((Quaternion::AngleAxis(3.7f, Vector3(1, 2, 3)) * Quaternion::AngleAxis(80, Vector3::down)) ==
        Quaternion(0.7767371f, 0.02324732f, -0.6292337f, 0.01428231f));
}
