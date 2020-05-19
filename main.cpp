#include <iostream>
#include "./vec4.h"
#include "./matrix4x4.h"
#include "./gradient.h"
#include "./color.h"

namespace ledstickler {

static constexpr color_convert<uint8_t> convert;

static constexpr vec4 srgb8_stop(const rgba<uint8_t> &color, double stop) {
    return vec4(convert.sRGB2CIELUV(color), stop);
}

static constexpr gradient gradient_rainbow((const vec4[7]){
    srgb8_stop({0xff,0x00,0x00}, 0.00),
    srgb8_stop({0xff,0xff,0x00}, 0.16),
    srgb8_stop({0x00,0xff,0x00}, 0.33),
    srgb8_stop({0x00,0xff,0xff}, 0.50),
    srgb8_stop({0x00,0x00,0xff}, 0.66),
    srgb8_stop({0xff,0x00,0xff}, 0.83),
    srgb8_stop({0xff,0x00,0x00}, 1.00)},7);

static constexpr gradient gradient_sunset((const vec4[3]){
    srgb8_stop({0xff,0x3f,0x3f}, 0.00),
    srgb8_stop({0x00,0xcf,0xff}, 0.50),
    srgb8_stop({0xff,0x3f,0x3f}, 1.00)},3);

static constexpr gradient gradient_ramp((const vec4[2]){
    srgb8_stop({0xff,0xff,0xff}, 0.00),
    srgb8_stop({0x00,0x00,0x00}, 1.00)},2);

static void test() {
    for (double c = 0.0; c < 1.0; c += 0.02 ) {
        auto col = convert.CIELUV2sRGB(gradient_sunset.clamp(c));
        printf("%f %0f %f %f\n", 
            col.x, 
            col.y, 
            col.z, 
            col.w);
    }
}

};  // namespace ledstickler {

int main() {
    ledstickler::test();
    return 0;
}
