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

namespace ledstickler {

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

        vec4 &operator=(const vec4& other) = default;

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

        vec4 &operator%=(double v) {
            this->x = std::fmod(this->x, v);
            this->y = std::fmod(this->y, v);
            this->z = std::fmod(this->z, v);
            this->w = std::fmod(this->w, v);
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

        vec4 &operator%=(const vec4 &b) {
            this->x = std::fmod(this->x, b.x);
            this->y = std::fmod(this->y, b.y);
            this->z = std::fmod(this->z, b.z);
            this->w = std::fmod(this->w, b.w);
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
            return vec4(std::fmod(this->x,v),
                        std::fmod(this->y,v),
                        std::fmod(this->z,v),
                        std::fmod(this->w,v));
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

        vec4 operator%(const vec4 &b) const {
            return vec4(fmod(this->x,b.x),
                        fmod(this->y,b.y),
                        fmod(this->z,b.z),
                        fmod(this->w,b.w));
        }
        
        double len() {
            return std::sqrt(x*x + y*y + z*z);
        }

        double len(const vec4 &a) {
            return std::sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
        }
        
        double dist(const vec4 &b) {
            double xd = std::fabs(this->x - b.x);
            double yd = std::fabs(this->y - b.y);
            double zd = std::fabs(this->z - b.z);
            return std::sqrt(xd*xd + yd*yd + zd*zd);
        }

        static double dist(const vec4 &a, const vec4 &b) {
            double xd = std::fabs(a.x - b.x);
            double yd = std::fabs(a.y - b.y);
            double zd = std::fabs(a.z - b.z);
            return std::sqrt(xd*xd + yd*yd + zd*zd);
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
        
        vec4 pow(double v) {
            return vec4(std::pow(this->x, v),
                        std::pow(this->y, v),
                        std::pow(this->z, v),
                        std::pow(this->w, v));
        }
        
        vec4 abs() {
            return vec4(std::fabs(this->x),
                        std::fabs(this->y),
                        std::fabs(this->z),
                        std::fabs(this->w));
        }

        static vec4 abs(const vec4 &a) {
            return vec4(std::fabs(a.x),
                        std::fabs(a.y),
                        std::fabs(a.z),
                        std::fabs(a.w));
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

        vec4 sqrt() {
            return vec4(std::sqrt(this->x),
                        std::sqrt(this->y),
                        std::sqrt(this->z),
                        std::sqrt(this->w));
        }

        static vec4 sqrt(const vec4 &a) {
            return vec4(std::sqrt(a.x),
                        std::sqrt(a.y),
                        std::sqrt(a.z),
                        std::sqrt(a.w));
        }
        
        vec4 rotate2d(double angle) {
            return vec4(
                this->x * std::cos(angle) - this->y * std::sin(angle),
                this->y * std::cos(angle) + this->x * std::sin(angle),
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
            return vec4((this->x != 0.0) ? (1.0/std::sqrt(this->x)) : 0.0,
                        (this->y != 0.0) ? (1.0/std::sqrt(this->y)) : 0.0,
                        (this->z != 0.0) ? (1.0/std::sqrt(this->z)) : 0.0,
                        (this->w != 0.0) ? (1.0/std::sqrt(this->w)) : 0.0);
        }

        static vec4 rsqrt(const vec4 &a) {
            return vec4((a.x != 0.0) ? (1.0/std::sqrt(a.x)) : 0.0,
                        (a.y != 0.0) ? (1.0/std::sqrt(a.y)) : 0.0,
                        (a.z != 0.0) ? (1.0/std::sqrt(a.z)) : 0.0,
                        (a.w != 0.0) ? (1.0/std::sqrt(a.w)) : 0.0);
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
    
        double reflect(double i) {
            i = std::fabs(i);
            if ((static_cast<int32_t>(i) & 1) == 0) {
                i = std::fmod(i, 1.0);
            } else {
                i = std::fmod(i, 1.0);
                i = 1.0 - i;
            }
            return i;
        }
    };
}

#endif  // #ifndef _VEC4_H_