/*
Copyright 2020 Tinic Uro

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef _VEC4_H_
#define _VEC4_H_

#include <cstdint>
#include <algorithm>
#include <cmath>
#include <cfloat>

#if defined(__clang__) || defined(_MSC_VER)
#if !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-float-conversion" 
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#endif  // #if !defined(_MSC_VER)
#include <gcem.hpp>
#if !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif  // #if !defined(_MSC_VER)
#define math_prefix gcem
#else // #if defined(__clang__) || defined(_MSC_VER)
#define math_prefix std
#endif // #if defined(__clang__) || defined(_MSC_VER)


namespace ledstickler {

    static constexpr inline double fabs_const(const double x) {
        return x < 0 ? -x : +x;        
    }

    struct vec4 {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        double w = 0.0;

        constexpr vec4() {
            this->x = 0.0;
            this->y = 0.0;
            this->z = 0.0;
            this->w = 0.0;
        }

        constexpr vec4(const vec4 &v, double a) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = a;
        }

        constexpr vec4(uint32_t c, double a) {
            this->x = ((c>>16)&0xFF)*(1.0/255.0);
            this->y = ((c>> 8)&0xFF)*(1.0/255.0);
            this->z = ((c>> 0)&0xFF)*(1.0/255.0);
            this->w = a;
        }

        constexpr vec4(double v) {
            this->x = v;
            this->y = v;
            this->z = v;
            this->w = v;
        }

        constexpr vec4(double _x, double _y, double _z, double _w = 0.0) {
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->w = _w;
        }

        constexpr vec4 &operator=(const vec4& other) = default;

        constexpr vec4 &operator+=(double v) {
            this->x += v;
            this->y += v;
            this->z += v;
            this->w += v;
            return *this;
        }

        constexpr vec4 &operator-=(double v) {
            this->x -= v;
            this->y -= v;
            this->z -= v;
            this->w -= v;
            return *this;
        }

        constexpr vec4 &operator*=(double v) {
            this->x *= v;
            this->y *= v;
            this->z *= v;
            this->w *= v;
            return *this;
        }

        constexpr vec4 &operator/=(double v) {
            this->x /= v;
            this->y /= v;
            this->z /= v;
            this->w /= v;
            return *this;
        }

        constexpr vec4 &operator%=(double v) {
            this->x = math_prefix::fmod(this->x, v);
            this->y = math_prefix::fmod(this->y, v);
            this->z = math_prefix::fmod(this->z, v);
            this->w = math_prefix::fmod(this->w, v);
            return *this;
        }
        
        constexpr vec4 &operator+=(const vec4 &b) {
            this->x += b.x;
            this->y += b.y;
            this->z += b.z;
            this->w += b.w;
            return *this;
        }

        constexpr vec4 &operator-=(const vec4 &b) {
            this->x += b.x;
            this->y += b.y;
            this->z += b.z;
            this->w += b.w;
            return *this;
        }

        constexpr vec4 &operator*=(const vec4 &b) {
            this->x += b.x;
            this->y += b.y;
            this->z += b.z;
            this->w += b.w;
            return *this;
        }

        constexpr vec4 &operator/=(const vec4 &b) {
            this->x += b.x;
            this->y += b.y;
            this->z += b.z;
            this->w += b.w;
            return *this;
        }

        constexpr vec4 &operator%=(const vec4 &b) {
            this->x = math_prefix::fmod(this->x, b.x);
            this->y = math_prefix::fmod(this->y, b.y);
            this->z = math_prefix::fmod(this->z, b.z);
            this->w = math_prefix::fmod(this->w, b.w);
            return *this;
        }

        constexpr vec4 operator-() const {
            return vec4(-this->x,
                        -this->y,
                        -this->z,
                        -this->w);
        }

        constexpr vec4 operator+() const {
            return vec4(+this->x,
                        +this->y,
                        +this->z,
                        +this->w);
        }

        constexpr vec4 operator+(double v) const {
            return vec4(this->x+v,
                        this->y+v,
                        this->z+v,
                        this->w+v);
        }

        constexpr vec4 operator-(double v) const {
            return vec4(this->x-v,
                        this->y-v,
                        this->z-v,
                        this->w-v);
        }

        constexpr vec4 operator*(double v) const {
            return vec4(this->x*v,
                          this->y*v,
                          this->z*v,
                          this->w*v);
        }

        constexpr vec4 operator/(double v) const {
            return vec4(this->x/v,
                        this->y/v,
                        this->z/v,
                        this->w/v);
        }

        vec4 operator%(double v) const {
            return vec4(math_prefix::fmod(this->x,v),
                        math_prefix::fmod(this->y,v),
                        math_prefix::fmod(this->z,v),
                        math_prefix::fmod(this->w,v));
        }
        
        constexpr vec4 operator+(const vec4 &b) const {
            return vec4(this->x+b.x,
                        this->y+b.y,
                        this->z+b.z,
                        this->w+b.w);
        }

        constexpr vec4 operator-(const vec4 &b) const {
            return vec4(this->x-b.x,
                        this->y-b.y,
                        this->z-b.z,
                        this->w-b.w);
        }

        constexpr vec4 operator*(const vec4 &b) const {
            return vec4(this->x*b.x,
                        this->y*b.y,
                        this->z*b.z,
                        this->w*b.w);
        }

        constexpr vec4 operator/(const vec4 &b) const {
            return vec4(this->x/b.x,
                        this->y/b.y,
                        this->z/b.z,
                        this->w/b.w);
        }

        constexpr vec4 operator%(const vec4 &b) const {
            return vec4(math_prefix::fmod(this->x,b.x),
                        math_prefix::fmod(this->y,b.y),
                        math_prefix::fmod(this->z,b.z),
                        math_prefix::fmod(this->w,b.w));
        }
        
        constexpr double len() {
            return math_prefix::sqrt(x*x + y*y + z*z);
        }

        constexpr double len(const vec4 &a) {
            return math_prefix::sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
        }
        
        constexpr double dist(const vec4 &b) {
            double xd = fabs_const(this->x - b.x);
            double yd = fabs_const(this->y - b.y);
            double zd = fabs_const(this->z - b.z);
            return math_prefix::sqrt(xd*xd + yd*yd + zd*zd);
        }

        static double dist(const vec4 &a, const vec4 &b) {
            double xd = fabs_const(a.x - b.x);
            double yd = fabs_const(a.y - b.y);
            double zd = fabs_const(a.z - b.z);
            return math_prefix::sqrt(xd*xd + yd*yd + zd*zd);
        }

        constexpr double max() const {
            return std::max(this->x,
                        std::max(this->y,
                            std::max(this->z, this->w)));
        }

        constexpr double min() const {
            return std::min(this->x,
                        std::min(this->y,
                            std::min(this->z, this->w)));
        }

        constexpr vec4 min(const vec4 &b) const {
            return vec4(std::min(this->x, b.x),
                        std::min(this->y, b.y),
                        std::min(this->z, b.z),
                        std::min(this->w, b.w));
        }

        constexpr static vec4 min(const vec4 &a, const vec4 &b) {
            return vec4(std::min(a.x, b.x),
                        std::min(a.y, b.y),
                        std::min(a.z, b.z),
                        std::min(a.w, b.w));
        }

        constexpr vec4 max(const vec4 &b) const {
            return vec4(std::max(this->x, b.x),
                        std::max(this->y, b.y),
                        std::max(this->z, b.z),
                        std::max(this->w, b.w));
        }

        constexpr static vec4 max(const vec4 &a, const vec4 &b) {
            return vec4(std::max(a.x, b.x),
                        std::max(a.y, b.y),
                        std::max(a.z, b.z),
                        std::max(a.w, b.w));
        }
        
        constexpr vec4 pow(double v) const {
            return vec4(math_prefix::pow(this->x, v),
                        math_prefix::pow(this->y, v),
                        math_prefix::pow(this->z, v),
                        math_prefix::pow(this->w, v));
        }
        
        constexpr vec4 abs() const {
            return vec4(fabs_const(this->x),
                        fabs_const(this->y),
                        fabs_const(this->z),
                        fabs_const(this->w));
        }

        static constexpr vec4 abs(const vec4 &a) {
            return vec4(fabs_const(a.x),
                        fabs_const(a.y),
                        fabs_const(a.z),
                        fabs_const(a.w));
        }

        constexpr vec4 xx00() const {
            return vec4(this->x, this->x, 0.0, 0.0);
        }

        constexpr vec4 yy00() const {
            return vec4(this->y, this->y, 0.0, 0.0);
        }

        constexpr vec4 zz00() const {
            return vec4(this->z, this->z, 0.0, 0.0);
        }
        
        constexpr vec4 xy00() const {
            return vec4(this->x, this->y, 0.0, 0.0);
        }

        constexpr vec4 yx00() const {
            return vec4(this->y, this->x, 0.0, 0.0);
        }

        constexpr vec4 xz00() const {
            return vec4(this->x, this->z, 0.0, 0.0);
        }

        constexpr vec4 zx00() const {
            return vec4(this->z, this->x, 0.0, 0.0);
        }

        constexpr vec4 yz00() const {
            return vec4(this->y, this->z, 0.0, 0.0);
        }

        constexpr vec4 zy00() const {
            return vec4(this->z, this->y, 0.0, 0.0);
        }

        constexpr vec4 sqrt() {
            return vec4(math_prefix::sqrt(this->x),
                        math_prefix::sqrt(this->y),
                        math_prefix::sqrt(this->z),
                        math_prefix::sqrt(this->w));
        }

        static constexpr vec4 sqrt(const vec4 &a) {
            return vec4(math_prefix::sqrt(a.x),
                        math_prefix::sqrt(a.y),
                        math_prefix::sqrt(a.z),
                        math_prefix::sqrt(a.w));
        }
        
        constexpr vec4 rotate2d(double angle) {
            return vec4(
                this->x * math_prefix::cos(angle) - this->y * math_prefix::sin(angle),
                this->y * math_prefix::cos(angle) + this->x * math_prefix::sin(angle),
                this->z,
                this->w);
        }
        
        constexpr vec4 reflect() {
            return vec4(
                reflect(this->x),
                reflect(this->y),
                reflect(this->z),
                reflect(this->w));
        }
        
        constexpr vec4 rsqrt() {
            return vec4((this->x != 0.0) ? (1.0/math_prefix::sqrt(this->x)) : 0.0,
                        (this->y != 0.0) ? (1.0/math_prefix::sqrt(this->y)) : 0.0,
                        (this->z != 0.0) ? (1.0/math_prefix::sqrt(this->z)) : 0.0,
                        (this->w != 0.0) ? (1.0/math_prefix::sqrt(this->w)) : 0.0);
        }

        static vec4 rsqrt(const vec4 &a) {
            return vec4((a.x != 0.0) ? (1.0/math_prefix::sqrt(a.x)) : 0.0,
                        (a.y != 0.0) ? (1.0/math_prefix::sqrt(a.y)) : 0.0,
                        (a.z != 0.0) ? (1.0/math_prefix::sqrt(a.z)) : 0.0,
                        (a.w != 0.0) ? (1.0/math_prefix::sqrt(a.w)) : 0.0);
        }

        constexpr vec4 rcp() const {
            return vec4((this->x != 0.0) ? (1.0/this->x) : 0.0,
                        (this->y != 0.0) ? (1.0/this->y) : 0.0,
                        (this->z != 0.0) ? (1.0/this->z) : 0.0,
                        (this->w != 0.0) ? (1.0/this->w) : 0.0);
        }

        static vec4 rcp(const vec4 &a) {
            return vec4((a.x != 0.0) ? (1.0/a.x) : 0.0,
                        (a.y != 0.0) ? (1.0/a.y) : 0.0,
                        (a.z != 0.0) ? (1.0/a.z) : 0.0,
                        (a.w != 0.0) ? (1.0/a.w) : 0.0);
        }

        constexpr vec4 lerp(const vec4 &b, double v) const {
            return vec4(
                this->x * (1.0 - v) + b.x * v, 
                this->y * (1.0 - v) + b.y * v, 
                this->z * (1.0 - v) + b.z * v, 
                this->w * (1.0 - v) + b.w * v);
        }

        constexpr static vec4 lerp(const vec4 &a, const vec4 &b, double v) {
            return vec4(
                a.x * (1.0 - v) + b.x * v, 
                a.y * (1.0 - v) + b.y * v, 
                a.z * (1.0 - v) + b.z * v, 
                a.w * (1.0 - v) + b.w * v);
        }

        constexpr vec4 clamp() const {
            return vec4(
                std::clamp(this->x, 0.0, 1.0),
                std::clamp(this->y, 0.0, 1.0),
                std::clamp(this->z, 0.0, 1.0),
                std::clamp(this->w, 0.0, 1.0)
            );
        }

        constexpr static vec4 zero() {
            return vec4(0.0, 0.0, 0.0, 0.0);
        }

        constexpr static vec4 one() {
            return vec4(1.0, 1.0, 1.0, 1.0);
        }

        constexpr static vec4 half() {
            return vec4(0.5, 0.5, 0.5, 0.5);
        }

    private:
    
        constexpr double reflect(double i) {
            i = fabs_const(i);
            if ((static_cast<int32_t>(i) & 1) == 0) {
                i -= math_prefix::floor(i);
            } else {
                i -= math_prefix::floor(i);
                i = 1.0 - i;
            }
            return i;
        }
    };
}

#endif  // #ifndef _VEC4_H_