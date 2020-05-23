#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>
#include <algorithm>

#include "./timeline.h"
#include "./artnet.h"

namespace ledstickler {

static datagram_socket socket(artnet_port);

vec4 span::blend(double time, const vec4 &top, const vec4 &btm) const {
    double in_f = tim.lead_in > 0 ? ( time != 0.0 ? std::clamp(time / tim.lead_in, 0.0, 1.0) : 0.0 ) : 1.0;
    double etime = time - (tim.duration - tim.lead_out);
    double out_f = tim.lead_out > 0 ? ( etime != 0.0 ? std::clamp(1.0 - (etime / tim.lead_out) , 0.0, 1.0) : 1.0) : 1.0;
    return blendFunc(*this, top, btm, in_f, out_f);
}

void timeline::run(fixture &f) {
    std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point frame_time = start_time;

    for (;;) {
        double time = std::chrono::duration_cast<std::chrono::microseconds>(frame_time - start_time).count() / 1'000'000.0;
    
        printf("%f ", time);
    
        f.walk_points( [time, this] (const std::vector<fixture *> &fixtures_stack, const vec4& point) {
            std::vector<const fixture *> fixtures_stack_const;
            fixtures_stack_const.insert(fixtures_stack_const.begin(), fixtures_stack.begin(), fixtures_stack.end());
            return calc(time, fixtures_stack_const, point, vec4());
        });

        std::this_thread::sleep_until(frame_time);
        frame_time += std::chrono::milliseconds(100);

        f.walk_fixtures( [] (const std::vector<const fixture *> &fixtures_stack) {
            const auto &ft = *fixtures_stack[0];
            if (!ft.name.size()) {
                return;
            }
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
        
        if (time > tim.duration) {
            start_time = frame_time;
        }
    }
}

vec4 timeline::blend(double time, const vec4 &top, const vec4 &btm) const {
    double in_f = tim.lead_in > 0 ? ( time != 0.0 ? std::clamp(time / tim.lead_in, 0.0, 1.0) : 0.0 ) : 1.0;
    double etime = time - (tim.duration - tim.lead_out);
    double out_f = tim.lead_out > 0 ? ( etime != 0.0 ? std::clamp(1.0 - (etime / tim.lead_out) , 0.0, 1.0) : 1.0) : 1.0;
    return blendFunc(top, btm, in_f, out_f);
}

vec4 timeline::calc(double time, const std::vector<const fixture *> &fixtures_stack, const vec4& point, vec4 btm) {
    vec4 res;
    for (auto& item : spans) {
        if (time >=  item.tim.start &&
            time <  (item.tim.start + item.tim.duration) ) {
            res = item.blend(time - item.tim.start, item.calcFunc(item, fixtures_stack, point, time - item.tim.start), res);
        }
    }
    for (auto& item : timelines) {
        if (time >=  item.tim.start &&
            time <  (item.tim.start + item.tim.duration) ) {
            res = item.calc(time - item.tim.start, fixtures_stack, point, res);
        }
    }
    return blend(time, res, btm);
}

};
