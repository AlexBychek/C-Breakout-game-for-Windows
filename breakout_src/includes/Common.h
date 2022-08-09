//
// Created by https://github.com/AlexBychek
//

#ifndef BREAKOUT_COMMON_H
#define BREAKOUT_COMMON_H

#include <stdint.h>
#include <cmath>

#define INT(a) static_cast<int>(a)
#define FLOAT(a) static_cast<float>(a)

template <typename T>
class Vec2_
{
public:
    T x;
    T y;

    Vec2_() = default;
    ~Vec2_() = default;

    Vec2_( const T& aX, const T& aY );
    Vec2_( const Vec2_& aSize );

    Vec2_& operator = ( const Vec2_& aSize );
};

template<typename T>
Vec2_<T>::Vec2_( const T& aX, const T& aY )
        : x( aX )
        , y( aY )
{
}

template<typename T>
Vec2_<T>::Vec2_( const Vec2_& aSize )
        : x( aSize.x )
        , y( aSize.y )
{
}

template<typename T>
Vec2_<T>& Vec2_<T>::operator = ( const Vec2_<T> &aSize )
{
    x = aSize.x;
    y = aSize.y;
    return *this;
}

class Color
{
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    Color() = default;
    ~Color() = default;

    Color( const uint8_t& aRed
         , const uint8_t& aGreen
         , const uint8_t& aBlue )
        : red( aRed )
        , green( aGreen )
        , blue( aBlue )
    {
    }
};

typedef Vec2_<int>   Vec2d;
typedef Vec2_<float> Vec2f;

#endif //BREAKOUT_COMMON_H
