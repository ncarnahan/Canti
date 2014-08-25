#include <Math/Math.h>
using namespace Math;
#include <Catch/Catch.hpp>

TEST_CASE("Matrix4x4 construction + multiplication", "[Matrix4x4]")
{
    REQUIRE(Matrix4x4::zero != Matrix4x4::identity);

    Matrix4x4 m1 = Matrix4x4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    REQUIRE((m1 * Matrix4x4::identity) == m1);
    REQUIRE((m1 * Matrix4x4::zero) == Matrix4x4::zero);
    REQUIRE((m1 * m1) == Matrix4x4(90, 100, 110, 120, 202, 228, 254, 280, 314, 356, 398, 440, 426, 484, 542, 600));

    //Position
    REQUIRE(Matrix4x4::FromPosition(Vector3(1, 2, 3)) ==
        Matrix4x4(
            1, 0, 0, 1,
            0, 1, 0, 2,
            0, 0, 1, 3,
            0, 0, 0, 1));
    REQUIRE(Matrix4x4::FromTransform(Vector3(1, 2, 3), Quaternion(), Vector3::one) ==
        Matrix4x4(
            1, 0, 0, 1,
            0, 1, 0, 2,
            0, 0, 1, 3,
            0, 0, 0, 1));

    //Rotation
    REQUIRE(Matrix4x4::FromRotation(Quaternion::AngleAxis(45, Vector3::up)) ==
        Matrix4x4(
            0.7071067f, 0, 0.7071068f, 0,
            0, 1, 0, 0,
            -0.7071068f, 0, 0.7071067f, 0,
            0, 0, 0, 1));
    REQUIRE(Matrix4x4::FromTransform(Vector3::zero, Quaternion::AngleAxis(45, Vector3::up), Vector3::one) ==
        Matrix4x4(
            0.7071067f, 0, 0.7071068f, 0,
            0, 1, 0, 0,
            -0.7071068f, 0, 0.7071067f, 0,
            0, 0, 0, 1));

    //Scaling
    REQUIRE(Matrix4x4::FromScale(Vector3(2, 3, 4)) ==
        Matrix4x4(
            2, 0, 0, 0,
            0, 3, 0, 0,
            0, 0, 4, 0,
            0, 0, 0, 1));
    REQUIRE(Matrix4x4::FromTransform(Vector3::zero, Quaternion(), Vector3(2, 3, 4)) ==
        Matrix4x4(
            2, 0, 0, 0,
            0, 3, 0, 0,
            0, 0, 4, 0,
            0, 0, 0, 1));

    //Position + Rotation
    REQUIRE(Matrix4x4::FromTransform(Vector3(1, 2, 3), Quaternion::AngleAxis(45, Vector3::up), Vector3::one) ==
        Matrix4x4(
            0.7071067f, 0, 0.7071068f, 1,
            0, 1, 0, 2,
            -0.7071068f, 0, 0.7071067f, 3,
            0, 0, 0, 1));

    //Position + Scaling
    REQUIRE(Matrix4x4::FromTransform(Vector3(1, 2, 3), Quaternion(), Vector3(2, 3, 4)) ==
        Matrix4x4(
            2, 0, 0, 1,
            0, 3, 0, 2,
            0, 0, 4, 3,
            0, 0, 0, 1));

    //Rotation + Scaling
    REQUIRE(Matrix4x4::FromTransform(Vector3::zero, Quaternion::AngleAxis(45, Vector3::up), Vector3(2, 3, 4)) ==
        Matrix4x4(
            1.414213f, 0, 2.828427f, 0, 
            0, 3, 0, 0, 
            -1.414214f, 0, 2.828427f, 0, 
            0, 0, 0, 1));

    //Position + Rotation + Scaling
    REQUIRE(
        Matrix4x4::FromTransform(
            Vector3(1, 2, 3),
            Quaternion::AngleAxis(45, Vector3(1, 2, 3)),
            Vector3(0.1f, 0.2f, 0.3f)) == 
        Matrix4x4(
            0.07280277f, -0.105021f, 0.1322182f, 1, 
            0.06087886f, 0.1581581f, -0.01903697f, 2, 
            -0.03152017f, 0.06290158f, 0.2686186f, 3, 
            0, 0, 0, 1));
}

TEST_CASE("Matrix4x4 projection functions", "[Matrix4x4]")
{
    REQUIRE(Matrix4x4::Orthographic(-1, 1, -1, 1, -1, 1) ==
        Matrix4x4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, -1, 0,
            0, 0, 0, 1));

    REQUIRE(Matrix4x4::Orthographic(-3, 3, -2, 2, -7, 7) ==
        Matrix4x4(
            0.3333333f, 0, 0, 0, 
            0, 0.5f, 0, 0, 
            0, 0, -0.1428571f, 0, 
            0, 0, 0, 1));

    REQUIRE(Matrix4x4::Orthographic(-2, 3, -1, 2, -6, 7) ==
        Matrix4x4(
            0.4f, 0, 0, -0.2f, 
            0, 0.6666667f, 0, -0.3333333f, 
            0, 0, -0.1538462f, -0.07692308f, 
            0, 0, 0, 1));

    REQUIRE(Matrix4x4::Perspective(60, 1, 1, 100) ==
        Matrix4x4(
            1.732051f, 0, 0, 0, 
            0, 1.732051f, 0, 0, 
            0, 0, -1.020202f, -2.020202f, 
            0, 0, -1, 0));

    REQUIRE(Matrix4x4::Perspective(60, 4.0f / 3.0f, 0.001f, 1000.0f) ==
        Matrix4x4(
            1.299038f, 0, 0, 0, 
            0, 1.732051f, 0, 0, 
            0, 0, -1.000002f, -0.002000002f, 
            0, 0, -1, 0));
}

TEST_CASE("Matrix4x4 Transposing and Inverting", "[Matrix4x4]")
{
    REQUIRE(Matrix4x4(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15).GetTranspose() ==
        Matrix4x4(0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15));
}
