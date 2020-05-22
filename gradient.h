#ifndef _GRADIENT_H_
#define _GRADIENT_H_

#include <array>

#include "./vec4.h"
#include "./color.h"

namespace ledstickler {

    template<size_t colors_2n = 8> class gradient {
    public:
        constexpr gradient(const vec4 stops[], const size_t n) {
            for (size_t c = 0; c < colors_n; c++) {
                double f = static_cast<double>(c) / static_cast<double>(colors_n - 1); 
                vec4 a = stops[0];
                vec4 b = stops[1];
                if (n > 2) {
                    for (int32_t d = static_cast<int32_t>(n-2); d >= 0 ; d--) {
                        if ( f >= (stops[d].w) ) {
                            a = stops[d+0];
                            b = stops[d+1];
                            break;
                        }
                    }
                }
                f -= a.w;
                f /= b.w - a.w;
                colors[c] = a.lerp(b,f);
            }
        }

        constexpr vec4 repeat(double i) const {
            i = std::fmod(i, 1.0);
            i *= colors_mul;
            return vec4::lerp(colors[(static_cast<size_t>(i))&colors_mask], colors[(static_cast<size_t>(i)+1)&colors_mask], std::fmod(i, 1.0));
        }

        constexpr vec4 reflect(double i) const {
            i = std::fabs(i);
            if ((static_cast<int32_t>(i) & 1) == 0) {
                i = std::fmod(i, 1.0);
            } else {
                i = std::fmod(i, 1.0);
                i = 1.0 - i;
            }
            i *= colors_mul;
            return vec4::lerp(colors[(static_cast<size_t>(i))&colors_mask], colors[(static_cast<size_t>(i)+1)&colors_mask], std::fmod(i, 1.0));
        }
        
        constexpr vec4 clamp(double i) const {
            if (i <= 0.0) {
                return colors[0];
            }
            if (i >= 1.0) {
                return colors[colors_n-1];
            }
            i *= colors_mul;
            return vec4::lerp(colors[(static_cast<size_t>(i))&colors_mask], colors[(static_cast<size_t>(i)+1)&colors_mask], std::fmod(i, 1.0));
        }

    private:
        static constexpr size_t colors_n = 1UL << colors_2n;
        static constexpr double colors_mul = static_cast<double>(colors_n - 1);
        static constexpr size_t colors_mask = colors_n - 1;
        std::array<vec4, colors_n> colors;
    };

}

#endif  // #ifndef _GRADIENT_H_
