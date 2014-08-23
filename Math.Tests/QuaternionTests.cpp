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