#ifndef TIMELINE_H_
#define TIMELINE_H_

#include <cstdint>
#include <functional>

#include "./fixture.h"

namespace ledstickler {

    class quad {
    public:
        static constexpr double easeIn(double t, double b, double c, double d) {
            t /= d;
            return c*t*t+b;
        }

        static constexpr double easeOut(double t, double b, double c, double d) {
            t /= d;
            return -c*t*(t-2.0)+b;
        }
    
        static constexpr double easeInOut(double t, double b, double c, double d) {
            t /= d/2.0;
            if (t < 1.0) {
                return ((c/2.0)*t*t)+b;
            } else {
                t -= 1.0;
                return -c/2.0*(((t-2.0)*t)-1.0)+b;
            }
        }
    
    };

    class cubic {
    public:
        static constexpr double easeIn(double t, double b, double c, double d) {
            t /= d;
            return c*t*t*t+b;
        }
    
        static constexpr double easeOut(double t, double b, double c, double d) {
            t = t/d-1.0;
            return c*(t*t*t+1.0)+b;
        }
    
        static constexpr double easeInOut(double t, double b, double c, double d) {
            t /= d/2.0;
            if (t < 1.0) {
                return c/2.0*t*t*t+b;
            } else {
                t -= 2.0;
                return c/2.0*(t*t*t+2.0)+b;    
            }
        }
    };

    struct timing {
        double start;
        double duration;
        double lead_in = 0.0;
        double lead_out = 0.0;
    };
    
    struct span {
        timing tim;

        std::function<vec4 (const span &s, const std::vector<const fixture *> &fixtures_stack, const vec4& point, double time)> calcFunc;
        
        std::function<vec4 (const span &s, const vec4 &top, const vec4 &btm, double in_f, double out_f)> blendFunc = 
            [] (const span &, const vec4 &top, const vec4 &btm, double in_f, double out_f) {
                return btm + top * in_f * out_f;
            };

        vec4 param0 = { 0 };
        vec4 param1 = { 0 };
        vec4 param2 = { 0 };
        vec4 param3 = { 0 };
    };

    class timeline {
    public:
        void run(fixture &fixture, uint64_t frame_time_us);

        vec4 calc(double time, const std::vector<const fixture *> &fixtures_stack, const vec4& point, vec4 btm); 

        template<typename T, typename ... Tplus> void push(T item, Tplus ... rest) {
            push(item);
            push(rest ...);
        }

        template<typename ... Tplus> timeline(Tplus ... rest) {
            push(rest ...);
        }

        void push(const timing &t) {
            tim = t;
        }
    
        void push(const span &s) {
            spans.push_back(s);
        }

        void push(const timeline &t) {
            timelines.push_back(t);
        }

        void push(std::function<vec4 (const timeline &t, const vec4 &top, const vec4 &btm, double in_f, double out_f)> f) {
            blendFunc = f;
        }

        std::string json(fixture &f) const;

        timing tim;
        std::vector<timeline> timelines;
        std::vector<span> spans;
        std::function<vec4 (const timeline &t, const vec4 &top, const vec4 &btm, double in_f, double out_f)> blendFunc =
            [] (const timeline &, const vec4 &top, const vec4 &btm, double in_f, double out_f) {
                return btm + top * in_f * out_f;
            };
    };

}

#endif /* TIMELINE_H_ */
