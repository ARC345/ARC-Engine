#include "doctest.h"
#include <ARC.h>
#include <ARC/Core/Yaml.h>
#include <glm/glm.hpp>

using namespace ARC;

// =============================================================
// FVec2 Tests
// =============================================================
TEST_CASE("FVec2 construction")
{
    FVec2 v0;
    CHECK(v0.x == doctest::Approx(0.0f));
    CHECK(v0.y == doctest::Approx(0.0f));

    FVec2 v1(3.0f, 4.0f);
    CHECK(v1.x == doctest::Approx(3.0f));
    CHECK(v1.y == doctest::Approx(4.0f));

    FVec2 v2(5.0f);
    CHECK(v2.x == doctest::Approx(5.0f));
    CHECK(v2.y == doctest::Approx(5.0f));

    // GLM interop
    glm::vec2 gv(1.0f, 2.0f);
    FVec2 v3(gv);
    CHECK(v3.x == doctest::Approx(1.0f));
    CHECK(v3.y == doctest::Approx(2.0f));

    // RGBA aliases
    CHECK(v1.r == doctest::Approx(3.0f));
    CHECK(v1.g == doctest::Approx(4.0f));
}

TEST_CASE("FVec2 ZeroVector / OneVector")
{
    CHECK(FVec2::ZeroVector() == FVec2(0, 0));
    CHECK(FVec2::OneVector() == FVec2(1, 1));
}

TEST_CASE("FVec2 operators")
{
    FVec2 a(1, 2);
    FVec2 b(3, 4);

    CHECK(a + b == FVec2(4, 6));
    CHECK(a - b == FVec2(-2, -2));
    CHECK(a * b == FVec2(3, 8));
    CHECK(a / b == FVec2(1.0f / 3.0f, 2.0f / 4.0f));

    CHECK(a + 2.0f == FVec2(3, 4));
    CHECK(a - 1.0f == FVec2(0, 1));
    CHECK(a * 3.0f == FVec2(3, 6));
    CHECK(a / 2.0f == FVec2(0.5f, 1.0f));

    FVec2 c = a;
    c += b;
    CHECK(c == FVec2(4, 6));
    c = a; c -= b;
    CHECK(c == FVec2(-2, -2));
    c = a; c *= b;
    CHECK(c == FVec2(3, 8));
    c = a; c /= b;
    CHECK(c.x == doctest::Approx(1.0f / 3.0f));
    CHECK(c.y == doctest::Approx(2.0f / 4.0f));

    c = a; c += 2.0f;
    CHECK(c == FVec2(3, 4));
    c = a; c -= 1.0f;
    CHECK(c == FVec2(0, 1));
    c = a; c *= 3.0f;
    CHECK(c == FVec2(3, 6));
    c = a; c /= 2.0f;
    CHECK(c == FVec2(0.5f, 1.0f));
}

TEST_CASE("FVec2 comparison")
{
    FVec2 a(1, 2);
    FVec2 b(1, 2);
    FVec2 c(1, 3);

    CHECK(a == b);
    CHECK(a != c);
    CHECK_FALSE(a == c);
    CHECK_FALSE(a != b);
}

TEST_CASE("FVec2 math")
{
    FVec2 a(3, 4);
    CHECK(a.Length() == doctest::Approx(5.0f));
    CHECK(a.Length() == doctest::Approx(a.Dist(FVec2::ZeroVector())));

    FVec2 n = a.Normalize();
    CHECK(n.Length() == doctest::Approx(1.0f));
    CHECK(n.x == doctest::Approx(3.0f / 5.0f));
    CHECK(n.y == doctest::Approx(4.0f / 5.0f));

    // Normalizing zero vector should not crash (return zero)
    FVec2 zero = FVec2::ZeroVector().Normalize();
    CHECK(zero == FVec2::ZeroVector());

    CHECK(a.DistSqr(FVec2(0, 0)) == doctest::Approx(25.0f));
    CHECK(a.Dist(FVec2(0, 0)) == doctest::Approx(5.0f));
    CHECK(a.DistSqr(FVec2(3, 4)) == doctest::Approx(0.0f));

    // Min/Max components
    CHECK(FVec2(1, 2).MinComponent() == doctest::Approx(1.0f));
    CHECK(FVec2(1, 2).MaxComponent() == doctest::Approx(2.0f));
    CHECK(FVec2(-5, 3).MinComponent() == doctest::Approx(-5.0f));
    CHECK(FVec2(-5, 3).MaxComponent() == doctest::Approx(3.0f));
}

TEST_CASE("FVec2 glm consistency")
{
    // Operations should match GLM exactly
    FVec2 a(1.5f, -2.5f);
    FVec2 b(3.0f, 4.0f);
    glm::vec2 ga(1.5f, -2.5f);
    glm::vec2 gb(3.0f, 4.0f);

    auto r1 = a + b;
    auto r2 = ga + gb;
    CHECK(r1.x == doctest::Approx(r2.x));
    CHECK(r1.y == doctest::Approx(r2.y));

    auto r3 = a * b;
    auto r4 = ga * gb;
    CHECK(r3.x == doctest::Approx(r4.x));
    CHECK(r3.y == doctest::Approx(r4.y));

    CHECK(glm::length(ga) == doctest::Approx(a.Length()));
    CHECK(glm::distance(ga, gb) == doctest::Approx(a.Dist(b)));
    CHECK(glm::distance2(ga, gb) == doctest::Approx(a.DistSqr(b)));

    auto gn = glm::normalize(ga);
    auto fn = a.Normalize();
    CHECK(fn.x == doctest::Approx(gn.x));
    CHECK(fn.y == doctest::Approx(gn.y));
}

// =============================================================
// FVec3 Tests
// =============================================================
TEST_CASE("FVec3 construction and operators")
{
    FVec3 v0;
    CHECK(v0.x == 0.0f);
    CHECK(v0.y == 0.0f);
    CHECK(v0.z == 0.0f);

    FVec3 v1(1, 2, 3);
    FVec3 v2(4, 5, 6);
    CHECK(v1 + v2 == FVec3(5, 7, 9));
    CHECK(v1 * v2 == FVec3(4, 10, 18));
    CHECK(v1 * 2.0f == FVec3(2, 4, 6));

    CHECK(v1 == v1);
    CHECK(v1 != v2);
    CHECK_FALSE(v1 == v2);
}

TEST_CASE("FVec3 cross product")
{
    FVec3 a(1, 0, 0);
    FVec3 b(0, 1, 0);
    FVec3 cross = a.Cross(a, b);  // Note: Cross is a member taking 2 args
    CHECK(cross == FVec3(0, 0, 1));

    // Orthogonal: cross(a,b) should be perpendicular to both
    FVec3 c(3, 1, -2);
    FVec3 d(2, 4, 5);
    FVec3 cd = c.Cross(c, d);
    // Dot with original vectors should be close to 0
    CHECK(SMath::Dot(cd, c) == doctest::Approx(0.0f));
    CHECK(SMath::Dot(cd, d) == doctest::Approx(0.0f));

    // GLM consistency
    glm::vec3 ga(3, 1, -2), gb(2, 4, 5);
    auto gc = glm::cross(ga, gb);
    CHECK(cd.x == doctest::Approx(gc.x));
    CHECK(cd.y == doctest::Approx(gc.y));
    CHECK(cd.z == doctest::Approx(gc.z));
}

TEST_CASE("FVec3 math")
{
    FVec3 v(1, 2, 3);
    float lenSq = 1 + 4 + 9;
    CHECK(v.DistSqr(FVec3::ZeroVector()) == doctest::Approx(lenSq));
    CHECK(v.Length() == doctest::Approx(std::sqrt(lenSq)));

    FVec3 n = v.Normalize();
    CHECK(n.Length() == doctest::Approx(1.0f));

    // Zero normalize
    FVec3 zero = FVec3::ZeroVector().Normalize();
    CHECK(zero == FVec3::ZeroVector());

    // Min/Max
    CHECK(FVec3(3, 1, 2).MinComponent() == 1.0f);
    CHECK(FVec3(3, 1, 2).MaxComponent() == 3.0f);
}

// =============================================================
// FVec4 Tests
// =============================================================
TEST_CASE("FVec4 construction and operators")
{
    FVec4 v0;
    CHECK(v0.x == 0.0f); CHECK(v0.y == 0.0f);
    CHECK(v0.z == 0.0f); CHECK(v0.w == 0.0f);

    FVec4 v1(1, 2, 3, 4);
    FVec4 v2(5, 6, 7, 8);

    CHECK(v1 + v2 == FVec4(6, 8, 10, 12));
    CHECK(v1 * v2 == FVec4(5, 12, 21, 32));
    CHECK(v1 * 2.0f == FVec4(2, 4, 6, 8));

    CHECK(v1 == v1);
    CHECK(v1 != v2);
    CHECK_FALSE(v1 == v2);

    // RGBA aliases
    CHECK(v1.r == v1.x);
    CHECK(v1.g == v1.y);
    CHECK(v1.b == v1.z);
    CHECK(v1.a == v1.w);
}

TEST_CASE("FVec4 zero/one vectors")
{
    CHECK(FVec4::ZeroVector() == FVec4(0, 0, 0, 0));
    CHECK(FVec4::OneVector() == FVec4(1, 1, 1, 1));
}

TEST_CASE("FVec4 math")
{
    FVec4 v(1, 2, 3, 4);
    float lenSq = 1 + 4 + 9 + 16;
    CHECK(v.Length() == doctest::Approx(std::sqrt(lenSq)));
    CHECK(v.DistSqr(FVec4::ZeroVector()) == doctest::Approx(lenSq));

    FVec4 n = v.Normalize();
    CHECK(n.Length() == doctest::Approx(1.0f));

    FVec4 zero = FVec4::ZeroVector().Normalize();
    CHECK(zero == FVec4::ZeroVector());
}

// =============================================================
// Non-float TVec Tests (integer types)
// =============================================================
TEST_CASE("TVec2<uint32_t> operations")
{
    TVec2<uint32_t> a(1, 2);
    TVec2<uint32_t> b(3, 4);

    CHECK(a + b == TVec2<uint32_t>(4, 6));
    CHECK(a * b == TVec2<uint32_t>(3, 8));
    CHECK(a / b == TVec2<uint32_t>(0, 0));  // integer division

    CHECK(a.MinComponent() == 1);
    CHECK(a.MaxComponent() == 2);

    CHECK(TVec2<uint32_t>::ZeroVector() == TVec2<uint32_t>(0, 0));
    CHECK(TVec2<uint32_t>::OneVector() == TVec2<uint32_t>(1, 1));
}

TEST_CASE("TVec2<int> comparison (regression: comma operator bug)")
{
    TVec2<int> a(1, 2);
    TVec2<int> b(1, 2);
    TVec2<int> c(1, 3);
    CHECK(a == b);
    CHECK(a != c);
}

TEST_CASE("TVec4<int> comparison (regression: comma operator bug in <4,T>)")
{
    TVec4<int> a(1, 2, 3, 4);
    TVec4<int> b(1, 2, 3, 4);
    TVec4<int> c(1, 2, 3, 5);
    TVec4<int> d(1, 2, 0, 4);

    CHECK(a == b);
    CHECK(a != c);
    CHECK(a != d);

    // The specific bug: z == _.z, w == _.w (comma) meant only w was compared
    // Test that all components matter
    CHECK_FALSE(a == TVec4<int>(9, 2, 3, 4));  // x differs
    CHECK_FALSE(a == TVec4<int>(1, 9, 3, 4));  // y differs
    CHECK_FALSE(a == TVec4<int>(1, 2, 9, 4));  // z differs
    CHECK_FALSE(a == TVec4<int>(1, 2, 3, 9));  // w differs
}

TEST_CASE("TVec generic operator* (regression: subtract vs multiply)")
{
    // The bug was on line 96: this->Data()[i] - _.Data()[i] instead of *
    // This only affects the primary template TVec<N,T> with N != 1,2,3,4
    // Test with TVec<5,int> which uses the primary template
    TVec<5, int> a; a = TVec<5, int>(3);
    TVec<5, int> b; b = TVec<5, int>(4);
    auto r = a * b;
    for (size_t i = 0; i < 5; i++)
    {
        INFO("Element ", i);
        CHECK(r[i] == 12);  // should be 3*4=12, not 3-4=-1
    }
}

TEST_CASE("TVec<3,T> Cross (regression: comma operator)")
{
    // Cross should return a proper vector, not a comma-expression
    TVec3<int> a(1, 0, 0);
    TVec3<int> b(0, 1, 0);
    TVec3<int> c = a.Cross(a, b);
    CHECK(c.x == 0);
    CHECK(c.y == 0);
    CHECK(c.z == 1);
}

// =============================================================
// SMath Tests
// =============================================================
TEST_CASE("SMath::Dot")
{
    FVec2 a(1, 0);
    FVec2 b(0, 1);
    CHECK(SMath::Dot(a, b) == doctest::Approx(0.0f));

    FVec2 c(1, 2);
    FVec2 d(3, 4);
    CHECK(SMath::Dot(c, d) == doctest::Approx(11.0f));  // 1*3 + 2*4

    FVec3 e(1, 2, 3);
    FVec3 f(4, 5, 6);
    CHECK(SMath::Dot(e, f) == doctest::Approx(32.0f));  // 1*4 + 2*5 + 3*6
}

TEST_CASE("SMath::Dot with integral types")
{
    TVec2<int> a(1, 2);
    TVec2<int> b(3, 4);
    CHECK(SMath::Dot(a, b) == 11);  // should use generic fallback
}

TEST_CASE("SMath::Dist and DistSqr")
{
    FVec2 a(0, 0);
    FVec2 b(3, 4);
    CHECK(SMath::Dist(a, b) == doctest::Approx(5.0f));
    CHECK(SMath::DistSqr(a, b) == doctest::Approx(25.0f));

    FVec3 c(0, 0, 0);
    FVec3 d(1, 2, 3);
    float lenSq = 1 + 4 + 9;
    CHECK(SMath::Dist(c, d) == doctest::Approx(std::sqrt(lenSq)));
    CHECK(SMath::DistSqr(c, d) == doctest::Approx(lenSq));
}

TEST_CASE("SMath::Length and Normalize")
{
    FVec3 v(3, 4, 0);
    CHECK(SMath::Length(v) == doctest::Approx(5.0f));

    FVec3 n = FVec3(SMath::Normalize(v));
    CHECK(n.Length() == doctest::Approx(1.0f));
}

TEST_CASE("SMath::Cross")
{
    FVec3 a(1, 0, 0);
    FVec3 b(0, 1, 0);
    FVec3 c = SMath::Cross(a, b);
    CHECK(c == FVec3(0, 0, 1));

    CHECK(SMath::Dot(SMath::Cross(a, b), a) == doctest::Approx(0.0f));
    CHECK(SMath::Dot(SMath::Cross(a, b), b) == doctest::Approx(0.0f));
}

TEST_CASE("SMath::Clamp")
{
    CHECK(SMath::Clamp(5, 0, 10) == 5);
    CHECK(SMath::Clamp(-1, 0, 10) == 0);
    CHECK(SMath::Clamp(15, 0, 10) == 10);
    CHECK(SMath::Clamp(3.14f, 0.0f, 1.0f) == doctest::Approx(1.0f));
}

TEST_CASE("SMath::Min/Max variadic")
{
    CHECK(SMath::Min(3, 1, 5, 2, 4) == 1);
    CHECK(SMath::Max(3, 1, 5, 2, 4) == 5);
    CHECK(SMath::Min(-1.0f, 0.0f, 2.5f) == doctest::Approx(-1.0f));
}

TEST_CASE("SMath::Abs")
{
    CHECK(SMath::Abs(5) == 5);
    CHECK(SMath::Abs(-5) == 5);
    CHECK(SMath::Abs(0) == 0);
    CHECK(SMath::Abs(-3.14f) == doctest::Approx(3.14f));
}

TEST_CASE("SMath::Sqrt")
{
    CHECK(SMath::Sqrt(25.0f) == doctest::Approx(5.0f));
    CHECK(SMath::Sqrt(2.0) == doctest::Approx(std::sqrt(2.0)));
    CHECK(SMath::Sqrt(0.0f) == doctest::Approx(0.0f));
}

TEST_CASE("SMath angle conversions")
{
    CHECK(SMath::Radians(180.0f) == doctest::Approx(PIf));
    CHECK(SMath::Degrees(PIf) == doctest::Approx(180.0f));
    CHECK(SMath::Radians(0.0f) == doctest::Approx(0.0f));
    CHECK(SMath::Degrees(0.0f) == doctest::Approx(0.0f));
    // Full circle wrap
    CHECK(SMath::Radians(360.0f) == doctest::Approx(0.0f).epsilon(0.001f));
}

// =============================================================
// FColor4 Tests
// =============================================================
TEST_CASE("FColor4 construction")
{
    FColor4 white = FColor4::White();
    CHECK(white.r == doctest::Approx(1.0f));
    CHECK(white.g == doctest::Approx(1.0f));
    CHECK(white.b == doctest::Approx(1.0f));
    CHECK(white.a == doctest::Approx(1.0f));

    FColor4 red = FColor4::Red();
    CHECK(red.r == doctest::Approx(1.0f));
    CHECK(red.g == doctest::Approx(0.0f));
    CHECK(red.b == doctest::Approx(0.0f));
    CHECK(red.a == doctest::Approx(1.0f));

    FColor4 black = FColor4::Black();
    CHECK(black.r == doctest::Approx(0.0f));
    CHECK(black.a == doctest::Approx(1.0f));

    FColor4 custom(0.2f, 0.4f, 0.6f, 0.8f);
    CHECK(custom.r == doctest::Approx(0.2f));
    CHECK(custom.g == doctest::Approx(0.4f));
    CHECK(custom.b == doctest::Approx(0.6f));
    CHECK(custom.a == doctest::Approx(0.8f));
}

TEST_CASE("FColor4 inherits FVec4 operations")
{
    FColor4 a(0.5f, 0.5f, 0.5f, 1.0f);
    FColor4 b(0.5f, 0.5f, 0.5f, 1.0f);
    CHECK(a == b);

    FColor4 c = a + FColor4(0.1f, 0.1f, 0.1f, 0.0f);
    CHECK(c.r == doctest::Approx(0.6f));
}

// =============================================================
// Transform Tests
// =============================================================
TEST_CASE("FTransform2D default")
{
    FTransform2D t;
    CHECK(t.Location == FVec3::ZeroVector());
    CHECK(t.Rotation == doctest::Approx(0.0f));
    CHECK(t.Scale == FVec2::OneVector());
}

TEST_CASE("FTransform2D custom")
{
    FVec3 loc(10, 20, 0);
    float rot = PIf / 4.0f;
    FVec2 scale(2, 3);
    FTransform2D t(loc, rot, scale);
    CHECK(t.Location == loc);
    CHECK(t.Rotation == doctest::Approx(rot));
    CHECK(t.Scale == scale);
}

TEST_CASE("FTransform3D default")
{
    FTransform3D t;
    CHECK(t.Location == FVec3::ZeroVector());
    CHECK(t.Rotation == FVec3::ZeroVector());
    CHECK(t.Scale == FVec3::OneVector());
}

// =============================================================
// YAML Serialization Tests
// =============================================================
TEST_CASE("YAML FVec2 round-trip")
{
    FVec2 original(3.5f, -7.25f);
    YAML::Node node = YAML::convert<FVec2>::encode(original);
    FVec2 decoded = node.as<FVec2>();
    CHECK(decoded.x == doctest::Approx(original.x));
    CHECK(decoded.y == doctest::Approx(original.y));
}

TEST_CASE("YAML FVec3 round-trip")
{
    FVec3 original(1.0f, 2.5f, -3.0f);
    YAML::Node node = YAML::convert<FVec3>::encode(original);
    FVec3 decoded = node.as<FVec3>();
    CHECK(decoded.x == doctest::Approx(original.x));
    CHECK(decoded.y == doctest::Approx(original.y));
    CHECK(decoded.z == doctest::Approx(original.z));
}

TEST_CASE("YAML FVec4 round-trip")
{
    FVec4 original(0.1f, 0.2f, 0.3f, 0.4f);
    YAML::Node node = YAML::convert<FVec4>::encode(original);
    FVec4 decoded = node.as<FVec4>();
    CHECK(decoded.x == doctest::Approx(original.x));
    CHECK(decoded.y == doctest::Approx(original.y));
    CHECK(decoded.z == doctest::Approx(original.z));
    CHECK(decoded.w == doctest::Approx(original.w));
}

TEST_CASE("YAML FColor4 round-trip")
{
    FColor4 original(0.8f, 0.3f, 0.1f, 0.5f);
    YAML::Node node = YAML::convert<FColor4>::encode(original);
    FColor4 decoded = node.as<FColor4>();
    CHECK(decoded.r == doctest::Approx(original.r));
    CHECK(decoded.g == doctest::Approx(original.g));
    CHECK(decoded.b == doctest::Approx(original.b));
    CHECK(decoded.a == doctest::Approx(original.a));
}

TEST_CASE("YAML Emitter FVec2")
{
    FVec2 v(1, 2);
    YAML::Emitter out;
    out << v;
    std::string yaml = out.c_str();
    // Flow-style sequence [1, 2]
    CHECK(yaml == "[1.000000, 2.000000]");
}
