#include <iostream>
#include <vector>
#include <cstdint>
#include <functional>
#include <chrono>

#include "./vec4.h"
#include "./matrix4x4.h"
#include "./gradient.h"
#include "./color.h"
#include "./timeline.h"
#include "./fixture.h"

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


static fixture make_vertical_fixture(const std::string &name, const ipv4 &ip, vec4 pos, uint16_t universe) {
    fixture fixture({ip, name, universe});
    for (size_t c = 0; c < 100; c++) {
        fixture.push(pos);
        pos += vec4(0.0, 0.0, -15.0, 0.0);
    }
    return fixture;
}

static fixture global_fixture(
    make_vertical_fixture("A00", {192, 168, 1, 60}, {   0.0,    0.0, 2000.0}, 0),
    make_vertical_fixture("A01", {192, 168, 1, 61}, {1000.0,    0.0, 2000.0}, 0),
    make_vertical_fixture("A02", {192, 168, 1, 62}, {2000.0,    0.0, 2000.0}, 0),

    make_vertical_fixture("A03", {192, 168, 1, 63}, {   0.0, 1000.0, 2000.0}, 0),
    make_vertical_fixture("A04", {192, 168, 1, 64}, {1000.0, 1000.0, 2000.0}, 0),
    make_vertical_fixture("A05", {192, 168, 1, 65}, {2000.0, 1000.0, 2000.0}, 0),

    make_vertical_fixture("A06", {192, 168, 1, 66}, {   0.0, 2000.0, 2000.0}, 0),
    make_vertical_fixture("A07", {192, 168, 1, 67}, {1000.0, 2000.0, 2000.0}, 0),
    make_vertical_fixture("A08", {192, 168, 1, 68}, {2000.0, 2000.0, 2000.0}, 0),

    make_vertical_fixture("A09", {192, 168, 1, 69}, {   0.0, 3000.0, 2000.0}, 0),
    make_vertical_fixture("A10", {192, 168, 1, 70}, {1000.0, 3000.0, 2000.0}, 0),
    make_vertical_fixture("A11", {192, 168, 1, 71}, {2000.0, 3000.0, 2000.0}, 0)
);

static void test() {
    for (double c = 0.0; c < 1.05; c += 0.05 ) {
        auto col = convert.CIELUV2sRGB(gradient_sunset.clamp(c));
        printf("%f %0f %f %f\n", 
            col.x, 
            col.y, 
            col.z, 
            col.w);
    }
    
    for (double c = 0.0; c < 1.05; c += 0.05 ) {
        const rgba<uint16_t> col(convert.CIELUV2sRGB(gradient_sunset.clamp(c)));
        printf("%04x %04x %04x %04x\n", 
            col.r, 
            col.g, 
            col.b, 
            col.a);
    }
    
    vec4 all;
    global_fixture.walk_points( [=] (const std::vector<bounds6> &bounds_stack, const vec4& point) mutable {
        all += (bounds_stack[0]).map_norm(point);
        return (bounds_stack[0]).map_norm(point);
    });
    printf("%f %f %f\n", all.x, all.y, all.z);

    global_fixture.walk_fixtures( [=] (const std::vector<const fixture *> &fixtures_stack) mutable {
        if (fixtures_stack[0]->name.size()) {
            printf("%s\n", fixtures_stack[0]->name.c_str());
        }
    });
    
    printf("%f %f %f %f %f %f\n",
        global_fixture.bounds.xmin,
        global_fixture.bounds.xmax,
        global_fixture.bounds.ymin,
        global_fixture.bounds.ymax,
        global_fixture.bounds.zmin,
        global_fixture.bounds.zmax);
}

};  // namespace ledstickler {

int main() {
    ledstickler::test();

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    for (;;) {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() << "\n";
    }

    return 0;
}
