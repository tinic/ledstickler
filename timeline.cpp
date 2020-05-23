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

template <typename T> static vec4 blend(const T &target, const timing &tim, double time, const vec4 &top, const vec4 &btm) {
    double in_f = tim.lead_in > 0 ? ( time != 0.0 ? std::clamp(time / tim.lead_in, 0.0, 1.0) : 0.0 ) : 1.0;
    double etime = time - (tim.duration - tim.lead_out);
    double out_f = tim.lead_out > 0 ? ( etime != 0.0 ? std::clamp(1.0 - (etime / tim.lead_out) , 0.0, 1.0) : 1.0) : 1.0;
    return target.blendFunc(target, top, btm, in_f, out_f);
}

void timeline::run(fixture &f, uint64_t frame_time_us) {
    std::chrono::system_clock::time_point start_time = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point frame_time = start_time;

    for (;;) {
        double time = std::chrono::duration_cast<std::chrono::microseconds>(frame_time - start_time).count() / 1'000'000.0;
    
        fflush(stdout); printf("\rtime (%fs)", time);
        span_count = 0;
        point_count = 0;
        color_sum = { 0 };
        
        f.walk_points( [time, this] (const std::vector<fixture *> &fixtures_stack, const vec4& point) {
            point_count ++;
            std::vector<const fixture *> fixtures_stack_const;
            fixtures_stack_const.insert(fixtures_stack_const.begin(), fixtures_stack.begin(), fixtures_stack.end());
            return calc(time, fixtures_stack_const, point, vec4());
        });
        
        printf(" active spans (%d)", int(span_count / point_count));

        std::this_thread::sleep_until(frame_time);
        frame_time += std::chrono::microseconds(frame_time_us);

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
            
            std::for_each(ft.points.begin(), ft.points.end(), [] (auto item) { color_sum += item.first; } );
            
            constexpr auto sync_packet = make_arnet_sync_packet();
            socket.sendTo(ft.address.addr(), static_cast<const uint8_t *>(sync_packet.data()), artnet_sync_packet_size);
        });

        static constexpr color_convert<uint8_t> convert;
        const rgba<uint16_t> col(convert.CIELUV2sRGB(color_sum / double(point_count)));
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
            res = blend(item, item.tim, time - item.tim.start, item.calcFunc(item, fixtures_stack, point, time - item.tim.start), res);
        }
    }
    for (auto& item : timelines) {
        if (time >=  item.tim.start &&
            time <  (item.tim.start + item.tim.duration) ) {
            res = item.calc(time - item.tim.start, fixtures_stack, point, res);
        }
    }
    return blend(*this, tim, time, res, btm);
}

}
