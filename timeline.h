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

    class timeline {
    public:
        void run(fixture &fixture);

        vec4 calc(double time, const std::vector<fixture *> &fixtures_stack, const vec4& point); 
           
        struct span {
            vec4 timing;

            std::function<void (span &s)> startFunc;
            std::function<vec4 (span &s)> calcFunc;
            std::function<vec4 (span &s, vec4 &btm)> blendFunc;

            vec4 param0;
            vec4 param1;
            vec4 param2;
            vec4 param3;
        };

        template<typename T, typename ... Tplus> void push(T item, Tplus ... rest) {
            push(item);
            push(rest ...);
        }

        template<typename ... Tplus> timeline(Tplus ... rest) {
            push(rest ...);
        }

        void push(const vec4 &t) {
            timing = t;
        }
    
        void push(const span &s) {
            spans.push_back(s);
        }

        void push(const timeline &t) {
            timelines.push_back(t);
        }

        vec4 timing;
        std::vector<timeline> timelines;
        std::vector<span> spans;

    private:
    
        void sort();
    };

};

#endif /* TIMELINE_H_ */
