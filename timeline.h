#ifndef TIMELINE_H_
#define TIMELINE_H_

#include <cstdint>
#include <functional>

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

#endif /* TIMELINE_H_ */

