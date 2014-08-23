#include <Math/Math.h>
using namespace Math;
#include <Catch/Catch.hpp>

TEST_CASE("Vector3 constructors", "[Vector3]")
{
    Vector3 v1;
    REQUIRE(v1.x == 0);
    REQUIRE(v1.y == 0);
    REQUIRE(v1.z == 0);

    Vector3 v2(3);
    REQUIRE(v2.x == 3);
    REQUIRE(v2.y == 3);
    REQUIRE(v2.z == 3);
    REQUIRE(v2.x == v2.y);
    REQUIRE(v2.x == v2.z);

    Vector3 v3(1, 3, 5);
    REQUIRE(v3.x == 1);
    REQUIRE(v3.y == 3);
    REQUIRE(v3.z == 5);

    Vector3 v4(v3);
    REQUIRE(v4.x == 1);
    REQUIRE(v4.y == 3);
    REQUIRE(v4.z == 5);
}

TEST_CASE("Vector3 operators", "[Vector3]")
{
    // operator+
    Vector3 v1 = Vector3(2, 4, 7) + Vector3(6, -4, 2);
    REQUIRE(v1.x == 8);
    REQUIRE(v1.y == 0);
    REQUIRE(v1.z == 9);
    // operator==
    REQUIRE(v1 == Vector3(8, 0, 9));

    // operator+=
    v1 += Vector3(1, 2, 3);
    REQUIRE(v1 == Vector3(9, 2, 12));

    // operator*=
    v1 *= 2;
    REQUIRE(v1 == Vector3(18, 4, 24));

    // operator[]
    REQUIRE(v1.x == v1[0]);
    REQUIRE(v1.y == v1[1]);
    REQUIRE(v1.z == v1[2]);
    v1[0] = 4;
    REQUIRE(v1.x == 4);

    // operator-
    Vector3 v2 = Vector3(2, 4, 7) - Vector3(6, -4, 2);
    REQUIRE(v2.x == -4);
    REQUIRE(v2.y == 8);
    REQUIRE(v2.z == 5);
    REQUIRE(v2 == Vector3(-4, 8, 5));
    // operator!=
    REQUIRE(v2 != Vector3(8, 0, 9));

    // operator-=
    v2 -= Vector3(-4, 4, -3);
    REQUIRE(v2 == Vector3(0, 4, 8));

    // operator/=
    v2 /= 4;
    REQUIRE(v2 == Vector3(0, 1, 2));

    // operator- (unary)
    REQUIRE(Vector3(1, 2, 3) == -Vector3(-1, -2, -3));

    // operator*
    REQUIRE((2 * Vector3(1, 2, 3)) == Vector3(2, 4, 6));
    REQUIRE((Vector3(1, 2, 3) * 2) == Vector3(2, 4, 6));
    // operator/
    REQUIRE(Vector3(1, 2, 3) == Vector3(2, 4, 6) / 2);
}

TEST_CASE("Vector3 functions", "[Vector3]")
{
    //Dot and Cross Products
    REQUIRE(Vector3::Dot(Vector3(0, 2, 0), Vector3(1, 1, 0)) == 2);
    REQUIRE(Vector3::Dot(Vector3(1, 2, 3), Vector3(-4, 5, -6)) == -12);
    REQUIRE(Vector3::Cross(Vector3::up, Vector3::forward) == Vector3::right);
    REQUIRE(Vector3::Cross(Vector3::forward, Vector3::up) == Vector3::left);

    //Magnitude
    REQUIRE(Vector3(1, 0, 0).Magnitude() == 1);
    REQUIRE(Vector3(3, 4, 0).Magnitude() == 5);
    REQUIRE(Vector3(1, 0, 0).MagnitudeSqr() == 1);
    REQUIRE(Vector3(3, 4, 0).MagnitudeSqr() == 25);

    //Normalization
    Vector3 v = Vector3(1, 1, 0);
    v.Normalize();
    REQUIRE(v == Vector3(0.707106781f, 0.707106781f, 0));
    REQUIRE(Vector3(1, 1, 0).Normalized() == Vector3(0.707106781f, 0.707106781f, 0));

    //Distance
    REQUIRE(Vector3::Distance(Vector3::zero, Vector3(4, 0, 0)) == 4);
    REQUIRE(Vector3::Distance(Vector3(1, 2, 3), Vector3(4, 6, 3)) == 5);

    //Lerp
    REQUIRE(Vector3::Lerp(Vector3(7, 8, 9), Vector3::zero, 0) == Vector3(7, 8, 9));
    REQUIRE(Vector3::Lerp(Vector3::zero, Vector3(5, 6, 7), 1) == Vector3(5, 6, 7));
    REQUIRE(Vector3::Lerp(Vector3::zero, Vector3(2, 4, 6), 0.5f) == Vector3(1, 2, 3));
    REQUIRE(Vector3::Lerp(Vector3(5, 0, 0), Vector3(9, 0, 0), 0.25f) == Vector3(6, 0, 0));

    //Slerp
    REQUIRE(Vector3::Slerp(Vector3(1, 0, 0), Vector3(0, 1, 0), 0) == Vector3(1, 0, 0));
    REQUIRE(Vector3::Slerp(Vector3(1, 0, 0), Vector3(0, 1, 0), 1) == Vector3(0, 1, 0));
    REQUIRE(Vector3::Slerp(Vector3(1, 0, 0), Vector3(0, 1, 0), 0.5f) == Vector3(0.707106781f, 0.707106781f, 0));
    REQUIRE(Vector3::Slerp(Vector3(1, 0, 0), Vector3(0, 1, 0), 1.0f / 3.0f) == Vector3(0.866025404f, 0.5f, 0));

    //Nlerp
    REQUIRE(Vector3::Nlerp(Vector3(1, 0, 0), Vector3(0, 1, 0), 0) == Vector3(1, 0, 0));
    REQUIRE(Vector3::Nlerp(Vector3(1, 0, 0), Vector3(0, 1, 0), 1) == Vector3(0, 1, 0));
    REQUIRE(Vector3::Nlerp(Vector3(1, 0, 0), Vector3(0, 1, 0), 0.5f) == Vector3(0.707106781f, 0.707106781f, 0));
}
