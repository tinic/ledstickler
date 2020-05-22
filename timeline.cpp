#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>

#include "./timeline.h"
#include "./artnet.h"

namespace ledstickler {

static datagram_socket socket(artnet_port);

void timeline::run(fixture &f) {
    std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point frame_time = start_time;

    for (;;) {
        f.walk_points( [start_time, frame_time, this] (const std::vector<fixture *> &fixtures_stack, const vec4& point) {
            std::vector<const fixture *> fixtures_stack_const;
            fixtures_stack_const.insert(fixtures_stack_const.begin(), fixtures_stack.begin(), fixtures_stack.end());
            return calc(std::chrono::duration_cast<std::chrono::microseconds>(frame_time - start_time).count() / 1'000'000.0, fixtures_stack_const, point);
        });

        std::this_thread::sleep_until(frame_time);
        frame_time += std::chrono::milliseconds(1000);

        f.walk_fixtures( [] (const std::vector<const fixture *> &fixtures_stack) {
            const auto &ft = *fixtures_stack[0];
            if (!ft.name.size()) {
                return;
            }
            printf("%s\n", ft.name.c_str());
            auto packets = create_artnet_output_packets(ft);
            for (auto packet : packets) {
                socket.sendTo(ft.address.addr(), static_cast<const uint8_t *>(packet.data()), packet.size());
            }
        });

        f.walk_fixtures( [] (const std::vector<const fixture *> &fixtures_stack) {
            const auto &ft = *fixtures_stack[0];
            if (!ft.name.size()) {
                return;
            }
            constexpr auto sync_packet = make_arnet_sync_packet();
            socket.sendTo(ft.address.addr(), static_cast<const uint8_t *>(sync_packet.data()), artnet_sync_packet_size);
        });
    }
}

vec4 timeline::calc(double time, const std::vector<const fixture *> &fixtures_stack, const vec4& point) {
    vec4 res = { 0 };
    for (auto& item : spans) {
        if (time >=  item.timing.x &&
            time <  (item.timing.x + item.timing.y) ) {
            vec4 col = item.calcFunc(item, fixtures_stack, point, time - item.timing.x);
            res += col; // blend here!           
        }
    }
    for (auto& item : timelines) {
        if (time >=  timing.x &&
            time <  (timing.x + timing.y) ) {
            res += item.calc(time - timing.x, fixtures_stack, point);
        }
    }
    return res;
}

};
