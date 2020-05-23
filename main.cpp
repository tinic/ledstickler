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

static uint64_t frame_time_us = 100'000;

static constexpr gradient gradient_rainbow((const vec4[]){
    srgb8_stop({0xff,0x00,0x00}, 0.00),
    srgb8_stop({0xff,0xff,0x00}, 0.16),
    srgb8_stop({0x00,0xff,0x00}, 0.33),
    srgb8_stop({0x00,0xff,0xff}, 0.50),
    srgb8_stop({0x00,0x00,0xff}, 0.66),
    srgb8_stop({0xff,0x00,0xff}, 0.83),
    srgb8_stop({0xff,0x00,0x00}, 1.00)},7);

static constexpr gradient gradient_sunset((const vec4[]){
    srgb8_stop({0xff,0x3f,0x3f}, 0.00),
    srgb8_stop({0x00,0xcf,0xff}, 0.50),
    srgb8_stop({0xff,0x3f,0x3f}, 1.00)},3);

static constexpr gradient gradient_ramp((const vec4[]){
    srgb8_stop({0xff,0xff,0xff}, 0.00),
    srgb8_stop({0x00,0x00,0x00}, 1.00)},2);

static vec4 basicRamp(const span &, const std::vector<const fixture *> &fixtures, const vec4 &pos, double time) {
    return gradient_ramp.reflect((fixtures[0]->bounds.map_unit(pos) + (time * 0.25)).z);
}

static vec4 nullCalc(const span &, const std::vector<const fixture *> &, const vec4&, double) {
    return vec4();
}

static timeline effect0({
    timing { 0.0, 30.0 },
    span{ timing {    0.0,   30.0 }, basicRamp },
    span{ timing {    0.0,   30.0 }, nullCalc }
});

static timeline master({
    timing { 0.0, 30.0, 2.0, 2.0 },
    timeline { timing {    0.0,   10.0, 0.0, 2.0 }, effect0 },
    timeline { timing {   10.0,   10.0, 2.0, 2.0 }, effect0 }
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
    make_vertical_fixture("A00", {192, 168, 1, 60}, {   0.0,    0.0, 2000.0}, 0, 1)/*,
    make_vertical_fixture("A01", {192, 168, 1, 61}, {1000.0,    0.0, 2000.0}, 0, 1),
    make_vertical_fixture("A02", {192, 168, 1, 62}, {2000.0,    0.0, 2000.0}, 0, 1),

    make_vertical_fixture("A03", {192, 168, 1, 63}, {   0.0, 1000.0, 2000.0}, 0, 1),
    make_vertical_fixture("A04", {192, 168, 1, 64}, {1000.0, 1000.0, 2000.0}, 0, 1),
    make_vertical_fixture("A05", {192, 168, 1, 65}, {2000.0, 1000.0, 2000.0}, 0, 1),

    make_vertical_fixture("A06", {192, 168, 1, 66}, {   0.0, 2000.0, 2000.0}, 0, 1),
    make_vertical_fixture("A07", {192, 168, 1, 67}, {1000.0, 2000.0, 2000.0}, 0, 1),
    make_vertical_fixture("A08", {192, 168, 1, 68}, {2000.0, 2000.0, 2000.0}, 0, 1),

    make_vertical_fixture("A09", {192, 168, 1, 69}, {   0.0, 3000.0, 2000.0}, 0, 1),
    make_vertical_fixture("A10", {192, 168, 1, 70}, {1000.0, 3000.0, 2000.0}, 0, 1),
    make_vertical_fixture("A11", {192, 168, 1, 71}, {2000.0, 3000.0, 2000.0}, 0, 1)*/
);

}  // namespace ledstickler {

int main() {
    ledstickler::master.run(ledstickler::global_fixture, ledstickler::frame_time_us);
    return 0;
}
