#include <vector>
#include <cstdint>
#include <chrono>
#include <thread>
#include <algorithm>
#include <iostream>

#include "./timeline.h"
#include "./artnet.h"
#include "./color.h"

namespace ledstickler {

static datagram_socket socket(artnet_port);

static size_t span_count = 0;
static size_t point_count = 0;
static vec4 color_sum = { 0 };

template <typename T> static vec4 blend(const T &target, double time, const vec4 &top, const vec4 &btm) {
    double in_f = target.tim.lead_in > 0 ? ( time != 0.0 ? std::clamp(time / target.tim.lead_in, 0.0, 1.0) : 0.0 ) : 1.0;
    double etime = time - (target.tim.duration - target.tim.lead_out);
    double out_f = target.tim.lead_out > 0 ? ( etime != 0.0 ? std::clamp(1.0 - (etime / target.tim.lead_out) , 0.0, 1.0) : 1.0) : 1.0;
    return target.blendFunc(target, top, btm, in_f, out_f);
}

void timeline::run(fixture &f, uint64_t frame_time_us) {
    std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point frame_time = start_time;

    for (;;) {
        double time = double( std::chrono::duration_cast<std::chrono::microseconds>(frame_time - start_time).count() ) / 1'000'000.0;
    
        fflush(stdout); printf("\rtime (%fs)", time);
        span_count = 0;
        point_count = 0;
        color_sum = { 0 };
        
        f.walk_points( [time, this] (const std::vector<const fixture *> &fixtures_stack, const vec4& point) {
            point_count ++;
            return calc(time, fixtures_stack, point, vec4());
        });
        
        printf(" active spans (%d)", int(span_count / point_count));

        std::this_thread::sleep_until(frame_time);
        frame_time += std::chrono::microseconds(frame_time_us);

        f.walk_fixtures( [] (const std::vector<const fixture *> &fixtures_stack) {
            if (fixtures_stack.size() == 0 || fixtures_stack.front() == nullptr) {
                return;
            }
            const auto &ft = *fixtures_stack.front();
            if (!ft.name.size()) {
                return;
            }
            auto packets = create_artnet_output_packets(ft);
            for (auto packet : packets) {
                socket.sendTo(ft.address.addr(), static_cast<const uint8_t *>(packet.data()), packet.size());
            }
        });

        f.walk_fixtures( [] (const std::vector<const fixture *> &fixtures_stack) {
            if (fixtures_stack.size() == 0 || fixtures_stack.front() == nullptr) {
                return;
            }
            const auto &ft = *fixtures_stack.front();
            if (!ft.name.size()) {
                return;
            }
            std::for_each(ft.points.begin(), ft.points.end(), [] (auto item) { color_sum += item.first; } );
            constexpr auto sync_packet = make_arnet_sync_packet();
            socket.sendTo(ft.address.addr(), sync_packet.data(), artnet_sync_packet_size);
        });

        static constexpr color_convert<uint8_t> convert;
        const rgba<uint16_t> col(convert.CIELUV2LED(color_sum / double(point_count)));
        printf(" average color (r:%04x g:%04x b:%04x)", col.r, col.g, col.b);
        
        if (time > tim.duration) {
            start_time = frame_time;
        }
    }
}

vec4 timeline::calc(double time, const std::vector<const fixture *> &fixtures_stack, const vec4& point, vec4 btm) {
    vec4 res;
    for (auto& item : spans) {
        if (time >=  item.tim.start &&
            time <  (item.tim.start + item.tim.duration) ) {
            span_count ++;
            res = blend(item, time - item.tim.start, item.calcFunc(item, fixtures_stack, point, time - item.tim.start), res);
        }
    }
    for (auto& item : timelines) {
        if (time >=  item.tim.start &&
            time <  (item.tim.start + item.tim.duration) ) {
            res = item.calc(time - item.tim.start, fixtures_stack, point, res);
        }
    }
    return blend(*this, time, res, btm);
}

}
