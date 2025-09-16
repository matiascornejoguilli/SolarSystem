#include <cmath>
#include <iostream>
#include <string>

template <typename T, typename TagT>
class StrongType
{
public:
    explicit constexpr StrongType(T v)
    : mValue{v}
    {
    }

    constexpr StrongType(StrongType const&) = default;
    constexpr StrongType(StrongType&&) = default;
    constexpr StrongType& operator=(StrongType const&) = default;
    constexpr StrongType& operator=(StrongType&&) = default;

   constexpr T get() const
   {
        return mValue;
   }

    friend constexpr StrongType operator+(StrongType a, StrongType b)
    {
        return StrongType{a.mValue + b.mValue};
    }

    friend constexpr StrongType operator-(StrongType a, StrongType b)
    {
        return StrongType{a.mValue - b.mValue};
    }

    friend constexpr StrongType operator*(StrongType a, T k)
    {
        return StrongType{a.mValue * k};
    }

    friend constexpr StrongType operator*(T k, StrongType a)
    {
        return StrongType{k * a.mValue};
    }

    friend constexpr StrongType operator/(StrongType a, T k)
    {
        return StrongType{a.mValue / k};
    }

private:
    T mValue{};
};

template <typename T>
struct Vector
{
    T x{};
    T y{};

    constexpr Vector() = default;

    // Permite Vector<Meters>{0.0, 0.0}
    template <typename U>
    constexpr Vector(U x_, U y_)
    : x{T{x_}}
    , y{T{y_}}
    {
    }

    constexpr Vector(T x_, T y_)
    : x{x_}
    , y{y_}
    {
    }

    constexpr Vector& operator+=(Vector const& o)
    {
        x = x + o.x;
        y = y + o.y;
        return *this;
    }
    constexpr Vector& operator-=(Vector const& o)
    {
        x = x - o.x;
        y = y - o.y;
        return *this;
    }

    constexpr Vector operator*(double k) const
    {
        return {x * k, y * k};
    }
    constexpr Vector operator/(double k) const
    {
        return {x / k, y / k};
    }

    friend constexpr Vector operator*(double k, Vector const& v)
    {
        return v * k;
    }
};

inline double magnitude(Vector<double> const& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}
inline Vector<double> normalize(Vector<double> const& v)
{
    double m = magnitude(v);
    if (m == 0.0)
    {
        return {0.0, 0.0};
    }
    else
    {
        return v / m;
    }
}

struct MetersTag
{
};
struct SecondsTag
{
};
struct KilogramsTag
{
};
struct NewtonsTag
{
};
struct MetersPerSecondTag
{
};

using Meters = StrongType<double, MetersTag>;
using Seconds = StrongType<double, SecondsTag>;
using Kilograms = StrongType<double, KilogramsTag>;
using Newtons = StrongType<double, NewtonsTag>;
using MetersPerSecond = StrongType<double, MetersPerSecondTag>;

using PositionVec = Vector<Meters>;
using VelocityVec = Vector<MetersPerSecond>;
using ForceVec = Vector<Newtons>;
using Vec = ForceVec;

struct Body
{
    std::string name;
    Kilograms mass;
    PositionVec position;
    VelocityVec velocity;
};

// Constant gravitational
constexpr double G = 6.67430e-11;

Vec gravityForce(Body const& a, Body const& b)
{
    double const aux = 0.0;
    double const dx = b.position.x.get() - a.position.x.get();
    double const dy = b.position.y.get() - a.position.y.get();

    Vector<double> const diff{dx, dy};
    double const dist2 = (diff.x * diff.x) + (diff.y * diff.y);
    if (dist2 == aux)
    {
        return {aux, aux}; 
    }

    double const dist = std::sqrt(dist2);
    Vector<double> const unit = diff / dist; 
    Newtons const forceMag{G * (a.mass.get() * b.mass.get()) / dist2};

    return {forceMag * unit.x, forceMag * unit.y};
}

int main()
{
    Body sun{"Sol", Kilograms{1.99e30}, PositionVec{0.0, 0.0}, VelocityVec{0.0, 0.0}};
    Body earth{"Tierra", Kilograms{5.97e24}, PositionVec{1.5e11, 0.0}, VelocityVec{0.0, 29780.0}}; // 29.78 km/s

    Vec force = gravityForce(sun, earth);
    std::cout << "Fuerza sobre la Tierra debido al Sol: " << force.x.get() << ", " << force.y.get() << ") N\n";
}
