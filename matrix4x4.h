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
#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <cstdint>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cfloat>

#include "./array.h"
#include "./vec4.h"

namespace ledstickler {
    struct matrix4x4 {
        multi_array<double, 4, 4> m = { 
            1.0, 0.0, 0.0, 0.0, 
            0.0, 1.0, 0.0, 0.0, 
            0.0, 0.0, 1.0, 0.0, 
            0.0, 0.0, 0.0, 1.0 };

        matrix4x4 &operator=(const matrix4x4& other) = default;
        
        constexpr matrix4x4 &operator*=(const matrix4x4 &b) {
            matrix4x4 a = *this;
            zero();
            for (size_t c = 0; c < 4; c++) {
                for (size_t d = 0; d < 4; d++) {
                    for (size_t e = 0; e < 4; e++) {
                        (*this).m[c][d] += a.m[c][e] * b.m[c][d];
                    }
                }
            }
            return *this;
        }

        constexpr matrix4x4 operator*(const matrix4x4 &b) const {
            matrix4x4 o; o.zero();
            for (size_t c = 0; c < 4; c++) {
                for (size_t d = 0; d < 4; d++) {
                    for (size_t e = 0; e < 4; e++) {
                        o.m[c][d] += (*this).m[c][e] * b.m[c][d];
                    }
                }
            }
            return o;
        }
        
        constexpr bool operator==(const matrix4x4 &b) const {
            for (size_t c = 0; c < 4; c++) {
                for (size_t d = 0; d < 4; d++) {
                    if (std::fabs((*this).m[c][d] - b.m[c][d]) >= std::numeric_limits<double>::epsilon()) {
                        return false;
                    }
                }
            }
            return true;
        }

        constexpr bool operator!=(const matrix4x4 &b) const {
            for (size_t c = 0; c < 4; c++) {
                for (size_t d = 0; d < 4; d++) {
                    if (std::fabs((*this).m[c][d] - b.m[c][d]) < std::numeric_limits<double>::epsilon()) {
                        return false;
                    }
                }
            }
            return true;
        }

        constexpr matrix4x4 &zero() {
            for (size_t c = 0; c < 4; c++) {
                for (size_t d = 0; d < 4; d++) {
                    (*this).m[c][d] = 0.0;                   
                }
            }
            return *this;
        }        
        
        static constexpr matrix4x4 make_zero() {
            matrix4x4 o;
            for (size_t c = 0; c < 4; c++) {
                for (size_t d = 0; d < 4; d++) {
                    o.m[c][d] = 0.0;                   
                }
            }
            return o;
        }

        constexpr matrix4x4 &scale(double x, double y, double z) {
            for (size_t c = 0; c < 4; c++) {
                (*this).m[0][c] *= x;
                (*this).m[1][c] *= y;
                (*this).m[2][c] *= z;
            }
            return *this;
        }
        
        static constexpr matrix4x4 make_scale(double x, double y, double z) {
            matrix4x4 m;
            m.m[0][0] = x;
            m.m[1][1] = y;
            m.m[2][2] = z;
            return m;
        };

        constexpr matrix4x4 &scale(const vec4 &vec) {
            for (size_t c = 0; c < 4; c++) {
                (*this).m[0][c] *= vec.x;
                (*this).m[1][c] *= vec.y;
                (*this).m[2][c] *= vec.z;
            }
            return *this;
        }
        
        static constexpr matrix4x4 make_scale(const vec4 &vec) {
            matrix4x4 o;
            o.m[0][0] = vec.x;
            o.m[1][1] = vec.y;
            o.m[2][2] = vec.z;
            return o;
        };

        constexpr matrix4x4 &translate(double x, double y, double z) {
            for (size_t c = 0; c < 4; c++) {
                (*this).m[3][c] += (*this).m[0][c] * x + (*this).m[1][c] * y + (*this).m[2][c] * z;
            }
            return *this;
        }
        
        static matrix4x4 make_translate(double x, double y, double z) {
            matrix4x4 o;
            o.m[3][0] = x;
            o.m[3][1] = y;
            o.m[3][2] = z;
            return o;
        };

        constexpr matrix4x4 &translate(const vec4 &vec) {
            for (size_t c = 0; c < 4; c++) {
                (*this).m[3][c] += (*this).m[0][c] * vec.x + (*this).m[1][c] * vec.y + (*this).m[2][c] * vec.z;
            }
            return *this;
        }

        static constexpr matrix4x4 make_translate(const vec4 &vec) {
            matrix4x4 o;
            o.m[3][0] = vec.x;
            o.m[3][1] = vec.y;
            o.m[3][2] = vec.z;
            return o;
        };
        
        
    };
};


#endif  // #ifndef _MATRIX4_H_
