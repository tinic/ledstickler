#ifndef _BOUNDS_H_
#define _BOUNDS_H_

#include "./vec4.h"

namespace ledstickler {

    struct bounds6 {
        double xmin = std::numeric_limits<double>::max();
        double xmax = std::numeric_limits<double>::min();
        double ymin = std::numeric_limits<double>::max();
        double ymax = std::numeric_limits<double>::min();
        double zmin = std::numeric_limits<double>::max();
        double zmax = std::numeric_limits<double>::min();
        
        constexpr void add(const bounds6 &b) {
            xmin = std::min(xmin, b.xmin);
            xmax = std::max(xmax, b.xmax);
            ymin = std::min(ymin, b.ymin);
            ymax = std::max(ymax, b.ymax);
            zmin = std::min(zmin, b.zmin);
            zmax = std::max(zmax, b.zmax);
        }

        constexpr void add(const vec4 &v) {
            xmin = std::min(xmin, v.x);
            xmax = std::max(xmax, v.x);
            ymin = std::min(ymin, v.y);
            ymax = std::max(ymax, v.y);
            zmin = std::min(zmin, v.z);
            zmax = std::max(zmax, v.z);
        }
        
        constexpr vec4 extent() const {
            if (xmin > xmax ||
                ymin > ymax ||
                zmin > zmax) {
                return vec4();
            }
            return vec4(
                xmax - xmin,
                ymax - ymin,
                zmax - zmin);
        }        

        constexpr bounds6 norm_uniform() const {
            bounds6 nu;
            nu.xmin = (- extent().x * 0.5 ) / extent().max();
            nu.xmax = (+ extent().x * 0.5 ) / extent().max();
            nu.ymin = (- extent().y * 0.5 ) / extent().max();
            nu.ymax = (+ extent().y * 0.5 ) / extent().max();
            nu.zmin = (- extent().z * 0.5 ) / extent().max();
            nu.zmax = (+ extent().z * 0.5 ) / extent().max();
            return nu;
        }

        constexpr vec4 map_norm_uniform(const vec4 &v) const {
            return vec4(
               ( extent().max() >= std::numeric_limits<double>::epsilon() ) ? ( ( ( ( v.x - xmin - ( extent().x * 0.5 ) ) / extent().max() ) ) * 2.0 ) : 0.0,
               ( extent().max() >= std::numeric_limits<double>::epsilon() ) ? ( ( ( ( v.y - ymin - ( extent().y * 0.5 ) ) / extent().max() ) ) * 2.0 ) : 0.0,
               ( extent().max() >= std::numeric_limits<double>::epsilon() ) ? ( ( ( ( v.z - zmin - ( extent().z * 0.5 ) ) / extent().max() ) ) * 2.0 ) : 0.0);
        }

        constexpr vec4 map_norm(const vec4 &v) const {
            return vec4(
               ( extent().x >= std::numeric_limits<double>::epsilon() ) ? ( ( ( ( v.x - xmin ) / extent().x ) - 0.5 ) * 2.0 ) : 0.0,
               ( extent().y >= std::numeric_limits<double>::epsilon() ) ? ( ( ( ( v.y - ymin ) / extent().y ) - 0.5 ) * 2.0 ) : 0.0,
               ( extent().z >= std::numeric_limits<double>::epsilon() ) ? ( ( ( ( v.z - zmin ) / extent().z ) - 0.5 ) * 2.0 ) : 0.0);
        }

        constexpr vec4 map_unit(const vec4 &v) const {
            return vec4(
               ( extent().x >= std::numeric_limits<double>::epsilon() ) ? ( ( v.x - xmin ) / extent().x ) : 0.0,
               ( extent().y >= std::numeric_limits<double>::epsilon() ) ? ( ( v.y - ymin ) / extent().y ) : 0.0,
               ( extent().z >= std::numeric_limits<double>::epsilon() ) ? ( ( v.z - zmin ) / extent().z ) : 0.0);
        }
    };
}

#endif  // #ifndef _BOUNDS_H_
