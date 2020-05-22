#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>

#include "./timeline.h"
#include "./artnet.h"

namespace ledstickler {

static datagram_socket socket(artnet_port);

void timeline::run(fixture &f) {
    std::chrono::system_clock::time_point frame_time = std::chrono::system_clock::now() + std::chrono::milliseconds(100);

    for (;;) {
        f.walk_points( [this] (const std::vector<fixture *> &fixtures_stack, const vec4& point) {
            return calc(0.0, fixtures_stack, point);
        });

        std::this_thread::sleep_until(frame_time);
        frame_time += std::chrono::milliseconds(1000);

        f.walk_fixtures( [] (const std::vector<const fixture *> &fixtures_stack) {
            const fixture &ft = *fixtures_stack[0];
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
            const fixture &ft = *fixtures_stack[0];
            if (!ft.name.size()) {
                return;
            }
            constexpr auto sync_packet = make_arnet_sync_packet();
            socket.sendTo(ft.address.addr(), static_cast<const uint8_t *>(sync_packet.data()), artnet_sync_packet_size);
        });
    }
}

};
