#ifndef _COLOR_H_
#define _COLOR_H_

#include <array>

#include "./vec4.h"

namespace ledstickler {

    template<typename T> struct rgba {

        T r;
        T g;
        T b;
        T a;

        constexpr rgba() :
            r(0),
            g(0),
            b(0),
            a(0){
        }
    
        constexpr rgba(const rgba &from) :
            r(from.r),
            g(from.g),
            b(from.b),
            a(from.a) {
        }

        constexpr rgba(T _r, T _g, T _b) :
            r(_r),
            g(_g),
            b(_b),
            a(0) {
        }

        constexpr rgba(T _r, T _g, T _b, T _a) :
            r(_r),
            g(_g),
            b(_b),
            a(_a) {
        }

        constexpr explicit rgba(const vec4 &from) {
            r = clamp_to_type(from.x);
            g = clamp_to_type(from.y);
            b = clamp_to_type(from.z);
            a = clamp_to_type(from.w);
        }

        constexpr rgba<T>& operator=(const rgba<T>& other) = default;

        constexpr  uint8_t *write_grb_bytes(uint8_t *dst);

    private:
        constexpr T clamp_to_type(double v);
    };

    template<> inline constexpr double rgba<double>::clamp_to_type(double v) {
        return v;
    }

    template<> inline constexpr  uint8_t rgba<uint8_t>::clamp_to_type(double v) {
        return v < 0.0 ? uint8_t(0) : ( v > 1.0 ? uint8_t(0xFF) : uint8_t( v * 255.0 ) );
    }

    template<> inline constexpr uint16_t rgba<uint16_t>::clamp_to_type(double v) {
        return v < 0.0 ? uint16_t(0) : ( v > 1.0 ? uint16_t(0xFFFF) : uint16_t( v * 65535.0 ) );
    }

    template<> inline constexpr uint8_t *rgba<uint8_t>::write_grb_bytes(uint8_t *dst) {
        *dst++ = g;
        *dst++ = r;
        *dst++ = b;
        return dst;
    }

    template<> inline constexpr uint8_t *rgba<uint16_t>::write_grb_bytes(uint8_t *dst) {
        *dst++ = (g >> 8) & 0xFF;
        *dst++ = (g >> 0) & 0xFF;
        *dst++ = (r >> 8) & 0xFF;
        *dst++ = (r >> 0) & 0xFF;
        *dst++ = (b >> 8) & 0xFF;
        *dst++ = (b >> 0) & 0xFF;
        return dst;
    }

    template<typename T> class color_convert {
    public:
    
        constexpr color_convert() : sRGB2lRGB() {
            for (size_t c = 0; c < colors_n; c++) {
                double v = double(c) / static_cast<double>(colors_n - 1);
                if (v > 0.04045) {
                    sRGB2lRGB[c] = std::pow( (v + 0.055) / 1.055, 2.4);
                } else {
                    sRGB2lRGB[c] = v * ( 25.0 / 323.0 );
                };
            }
        }

        constexpr vec4 sRGB2CIELUV(const rgba<T> &in) const  {
            double r = sRGB2lRGB[in.r];
            double g = sRGB2lRGB[in.g];
            double b = sRGB2lRGB[in.b];

            double X = 0.4124564 * r + 0.3575761 * g + 0.1804375 * b;
            double Y = 0.2126729 * r + 0.7151522 * g + 0.0721750 * b;
            double Z = 0.0193339 * r + 0.1191920 * g + 0.9503041 * b;

            const double wu = 0.197839825;
            const double wv = 0.468336303;

            double l = ( Y <= 0.008856452 ) ? ( 9.03296296296 * Y) : ( 1.16 * std::pow(Y, 1.0 / 3.0) - 0.16);
            double d = X + 15.0 * Y + 3.0 * Z;
            double di = d != 0.0 ? ( 1.0 / d ) : 0.0;

            return vec4(l,
                (d > (1.0 / 65536.0)) ? 13.0 * l * ( ( 4.0 * X * di ) - wu ) : 0.0,
                (d > (1.0 / 65536.0)) ? 13.0 * l * ( ( 9.0 * Y * di ) - wv ) : 0.0);
        }

        constexpr vec4 CIELUV2sRGB(const vec4 &in) const {
            const double wu = 0.197839825;
            const double wv = 0.468336303;

            double up_13l = in.y + wu * (13.0 * in.x);
            double vp_13l = in.z + wv * (13.0 * in.x);
            double vp_13li = 1.0 / vp_13l;
    
            double Y = ( in.x + 0.16 ) * (1.0 / 1.16);
            double y = ( in.x <= 0.08 ) ? ( in.x * 0.1107056 ) : ( Y * Y * Y );
            double x = (vp_13l > (1.0 / 65536.0)) ? ( 2.25 * y * up_13l * vp_13li ) : 0.0;
            double z = (vp_13l > (1.0 / 65536.0)) ? ( y * ( 156.0 * in.x - 3.0 * up_13l - 20.0 * vp_13l ) * (1.0 / 4.0) * vp_13li ) : 0.0;

            double r =  3.2404542 * x + -1.5371385 * y + -0.4985314 * z;
            double g = -0.9692660 * x +  1.8760108 * y +  0.0415560 * z;
            double b =  0.0556434 * x + -0.2040259 * y +  1.0572252 * z;

            auto sRGBTransfer = [] (double a) {
                if (a <= 0.0) {
                    return 0.0;
                } else if (a < 0.0031308) {
                    return a * 12.92;
                } else if ( a < 0.999982 ) {
                    return std::pow(a, 1.0 / 2.4) * 1.055 - 0.055;
                } else {
                    return 1.0;
                }
            };

            return vec4(sRGBTransfer(r),sRGBTransfer(g),sRGBTransfer(b), in.w);
        }

    private:
        static constexpr size_t colors_n = 1UL<<(sizeof(T)*8);
        std::array<double, colors_n> sRGB2lRGB;
    };

    constexpr vec4 srgb8_stop(const rgba<uint8_t> &color, double stop) {
        return vec4(color_convert<uint8_t>().sRGB2CIELUV(color), stop);
    }

};

#endif  // #ifndef _COLOR_H_
