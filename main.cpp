#include <iostream>
#include <vector>
#include <cstdint>
#include <functional>
#include <chrono>
#include <thread>
#include <cstring>

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

constexpr size_t artnet_output_packet_size = 512 + 18;
constexpr uint16_t artnet_port = 6454;

static fixture make_vertical_fixture(const std::string &name, const ipv4 &ip, vec4 pos, uint16_t universe0, uint16_t universe1) {
    fixture fixture({ip, name, universe0, universe1});
    if (name.size() && ip.addr() != 0) {
        fixture.socket.bind(artnet_port, ip.addr());
    }
    for (size_t c = 0; c < 100; c++) {
        fixture.push(pos);
        pos += vec4(0.0, 0.0, -15.0, 0.0);
    }
    return fixture;
}

static fixture global_fixture(
    make_vertical_fixture("A00", {192, 168, 1, 60}, {   0.0,    0.0, 2000.0}, 0, 1),
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
    make_vertical_fixture("A11", {192, 168, 1, 71}, {2000.0, 3000.0, 2000.0}, 0, 1)
);


std::vector<std::array<uint8_t, artnet_output_packet_size>> create_artnet_output_packets(const fixture &f) {

    constexpr uint16_t artnet_output_packet_id = 0x5000;
    constexpr uint16_t artnet_output_packet_version = 14;
    constexpr size_t artnet_dmx_len = 512;

    std::vector<std::array<uint8_t, artnet_output_packet_size>> packets;

    auto iter = f.points.begin();
    for (size_t len = f.points.size(); len > 0; ) {
        size_t chunk_len = std::min(artnet_dmx_len / (3 * 2), len);
        std::vector<std::pair<vec4, vec4>> chunk(iter, iter + chunk_len);
        
        std::array<uint8_t, artnet_output_packet_size> packet = { 0 };

        packet.at( 8) = (artnet_output_packet_id >> 0) & 0xFF;
        packet.at( 9) = (artnet_output_packet_id >> 8) & 0xFF;
        packet.at(10) = ( artnet_output_packet_version >> 8 ) & 0xFF;
        packet.at(11) = ( artnet_output_packet_version >> 0 ) & 0xFF;

        memcpy(packet.data(), "Art-Net", 8);

        size_t offset = 18;
        for (auto item : chunk) {
            const rgba<uint16_t> col(convert.CIELUV2sRGB(item.first));
            packet.at(offset + 0) = ( col.r >> 8 ) & 0xFF; 
            packet.at(offset + 1) = ( col.r >> 0 ) & 0xFF; 
            packet.at(offset + 2) = ( col.g >> 8 ) & 0xFF; 
            packet.at(offset + 3) = ( col.g >> 0 ) & 0xFF; 
            packet.at(offset + 4) = ( col.b >> 8 ) & 0xFF; 
            packet.at(offset + 5) = ( col.b >> 0 ) & 0xFF; 
            offset += 6;
        }

        packets.push_back(packet);

        len -= chunk_len;
        iter += chunk_len;
    }
    
    return packets;
}

static void run() {
    std::chrono::system_clock::time_point frame_time = std::chrono::system_clock::now() + std::chrono::milliseconds(100);

    for (;;) {
        global_fixture.walk_points( [=] (const std::vector<bounds6> &bounds_stack, const vec4& point) mutable {
            return (bounds_stack[0]).map_norm(point);
        });

        std::this_thread::sleep_until(frame_time);
        frame_time += std::chrono::milliseconds(1000);

        global_fixture.walk_fixtures( [=] (const std::vector<const fixture *> &fixtures_stack) mutable {
            const fixture &f = *fixtures_stack[0];
            if (!f.name.size()) {
                return;
            }
            printf("%s\n", f.name.c_str());
            auto packets = create_artnet_output_packets(f);
            for (auto packet : packets) {
                f.socket.send(static_cast<const uint8_t *>(packet.data()), packet.size());
            }
        });
    }
}

};  // namespace ledstickler {

int main() {
    ledstickler::run();
    return 0;
}
