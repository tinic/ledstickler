#include <iostream>
#include <vector>
#include <cstdint>
#include <functional>
#include <cstring>

#include "./vec4.h"
#include "./matrix4x4.h"
#include "./gradient.h"
#include "./color.h"
#include "./timeline.h"
#include "./fixture.h"
#include "./artnet.h"

namespace ledstickler {

static uint64_t frame_time_us = 10'000;

static constexpr gradient gradient_rainbow((const vec4[]){
    srgb8_stop({0xff,0x00,0x00}, 0.00),
    srgb8_stop({0xff,0xff,0x00}, 0.16),
    srgb8_stop({0x00,0xff,0x00}, 0.33),
    srgb8_stop({0x00,0xff,0xff}, 0.50),
    srgb8_stop({0x00,0x00,0xff}, 0.66),
    srgb8_stop({0xff,0x00,0xff}, 0.83),
    srgb8_stop({0xff,0x00,0x00}, 1.00)},7);

static constexpr gradient gradient_engine((const vec4[]){
    srgb8_stop({0x00,0x00,0x00}, 0.00),
    srgb8_stop({0xff,0x00,0x00}, 0.01),
    srgb8_stop({0xff,0xff,0x00}, 0.015),
    srgb8_stop({0xff,0xff,0xff}, 0.02),
    srgb8_stop({0xff,0xff,0x00}, 0.025),
    srgb8_stop({0xff,0x00,0x00}, 0.03),
    srgb8_stop({0x10,0x00,0x80}, 0.04),
    srgb8_stop({0x10,0x00,0x20}, 0.06),
    srgb8_stop({0x00,0x00,0x00}, 0.50),
    srgb8_stop({0x00,0x00,0x00}, 1.00)},10);

static constexpr gradient gradient_engine_bg((const vec4[]){
    srgb8_stop({0x80,0xff,0x80}, 0.00),
    srgb8_stop({0x80,0x00,0x00}, 1.00)},2);

static constexpr gradient gradient_ramp((const vec4[]){
    srgb8_stop({0xff,0xff,0xff}, 0.00),
    srgb8_stop({0x00,0x00,0x00}, 1.00)},2);

static vec4 engineBlastoff(const span &, const std::vector<const fixture *> &fixtures, const vec4 &pos, double time) {
    if (fixtures.size() == 0 || fixtures.front() == nullptr) {
        return vec4();
    }
    return gradient_engine.repeat(-((fixtures.front()->bounds.map_unit(pos) * 0.075 - time * 0.2000)).z);
}

static vec4 justARainbow(const span &, const std::vector<const fixture *> &fixtures, const vec4 &pos, double time) {
    if (fixtures.size() == 0 || fixtures.front() == nullptr) {
        return vec4();
    }
    return gradient_rainbow.repeat(-((fixtures.front()->bounds.map_unit(pos) * 0.75 - time * 0.2000)).z);
}

static vec4 justAGradient(const span &, const std::vector<const fixture *> &fixtures, const vec4 &pos, double time) {
    if (fixtures.size() == 0 || fixtures.front() == nullptr) {
        return vec4();
    }
    return gradient_ramp.reflect(-((fixtures.front()->bounds.map_unit(pos) * 4.0 + time * 0.2000)).z);
}

static vec4 background(const span &, const std::vector<const fixture *> &fixtures, const vec4 &pos, double) {
    if (fixtures.size() == 0 || fixtures.front() == nullptr) {
        return vec4();
    }
    return gradient_engine_bg.clamp((fixtures.front()->bounds.map_unit(pos)).z) * 0.011111;
}

static vec4 crossFade(const timeline &, const vec4 &top, const vec4 &btm, double in_f, double out_f) {
    return top * in_f * out_f + btm * (1.0 - ( in_f * out_f) );
};

static timeline effect0({
    timing { 0.0, 600.0 },
    span{ timing {    0.0,   600.0 }, background },
    span{ timing {    0.0,   600.0 }, engineBlastoff }
});

static timeline effect1({
    timing { 0.0, 600.0 },
    span{ timing {    0.0,   600.0 }, justARainbow },
    span{ timing {    0.0,   600.0 }, justAGradient }
});

static timeline master({
    timing { 0.0, 120.0 },
    timeline { timing {    0.0,  62.0, 2.0, 2.0 }, effect1, std::function(crossFade) },
    timeline { timing {   60.0,  62.0, 2.0, 2.0 }, effect0, std::function(crossFade) },
});

static fixture make_vertical_fixture(const std::string &name, const ipv4 &ip, vec4 pos, uint16_t universe0, uint16_t universe1) {
    fixture fixture{ip, name, universe0, universe1};
    for (size_t c = 0; c < 100; c++) {
        fixture.push(pos);
        pos += vec4(0.0, 0.0, -15.0, 0.0);
    }
    return fixture;
}

static fixture global_fixture(
    make_vertical_fixture("A00", {192, 168, 1,139}, {   0.0,    0.0, 2000.0}, 0, 1), // http://lightkraken-fcba318c/
    make_vertical_fixture("A01", {192, 168, 1, 61}, {1000.0,    0.0, 2000.0}, 0, 1), // http://lightkraken-44a12e19/
    make_vertical_fixture("A02", {192, 168, 1, 62}, {2000.0,    0.0, 2000.0}, 0, 1), // http://lightkraken-a0c5a315/

    make_vertical_fixture("A03", {192, 168, 1, 63}, {   0.0, 1000.0, 2000.0}, 0, 1), // http://lightkraken-755806a7/
    make_vertical_fixture("A04", {192, 168, 1, 64}, {1000.0, 1000.0, 2000.0}, 0, 1), // http://lightkraken-a8665d7e/
    make_vertical_fixture("A05", {192, 168, 1, 65}, {2000.0, 1000.0, 2000.0}, 0, 1), // http://lightkraken-0c0078f5/

    make_vertical_fixture("A06", {192, 168, 1, 66}, {   0.0, 2000.0, 2000.0}, 0, 1), // http://lightkraken-b196252d/
    make_vertical_fixture("A07", {192, 168, 1, 67}, {1000.0, 2000.0, 2000.0}, 0, 1), // http://lightkraken-3e15a8d1/
    make_vertical_fixture("A08", {192, 168, 1, 68}, {2000.0, 2000.0, 2000.0}, 0, 1), // http://lightkraken-3b9c4f9d/

    make_vertical_fixture("A09", {192, 168, 1, 69}, {   0.0, 3000.0, 2000.0}, 0, 1), // http://lightkraken-31980c5c/
    make_vertical_fixture("A10", {192, 168, 1, 70}, {1000.0, 3000.0, 2000.0}, 0, 1), // http://lightkraken-f3abccd6/
    make_vertical_fixture("A11", {192, 168, 1, 71}, {2000.0, 3000.0, 2000.0}, 0, 1)  // http://lightkraken-9f54b2db/
);

}  // namespace ledstickler {

int main() {
    ledstickler::master.run(ledstickler::global_fixture, ledstickler::frame_time_us);
    return 0;
}
