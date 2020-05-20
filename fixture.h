#ifndef _FIXTURE_H_
#define _FIXTURE_H_

#include <array>

#include "./vec4.h"
#include "./bounds.h"

namespace ledstickler {

    struct ipv4 {    
        uint8_t a0 = 0;
        uint8_t a1 = 0;
        uint8_t a2 = 0;
        uint8_t a3 = 0;
    };
    
    struct fixture {
        fixture() = default;

        template<typename T, typename ... Tplus> void push(T item, Tplus ... rest) {
            push(item);
            push(rest ...);
        }

        template<typename ... Tplus> fixture(Tplus ... rest) {
            push(rest ...);
        }

        void push(const ipv4 &a) {
            address = a;
        }

        void push(const std::string &s) {
            name = s;
        }

        void push(const fixture &f) {
            bounds.add(f.bounds);
            fixtures.push_back(f);

        }
    
        void push(const vec4 &p) {
            bounds.add(p);
            points.push_back(p);
        }

        void walk_points(std::function<void (const std::vector<bounds6> &bounds_stack, const vec4& point)> func) const {
            std::vector<bounds6> bounds_stack;
            walk_points(func, *this, bounds_stack);
        }

        void walk_points(std::function<void (const std::vector<bounds6> &bounds_stack, const vec4& point)> func, 
            const fixture &f, 
            std::vector<bounds6> &bounds_stack) const {
            bounds_stack.insert(bounds_stack.begin(), f.bounds);
            for (auto item : f.fixtures) {
                walk_points(func, item, bounds_stack);
            }
            for (auto item : f.points) {
                func(bounds_stack, item);
            }
            bounds_stack.erase(bounds_stack.begin());
        }

        void walk_fixtures(std::function<void (const std::vector<const fixture *> &fixture_stack)> func) const {
            std::vector<const fixture *> fixture_stack;
            fixture_stack.insert(fixture_stack.begin(), this);
            walk_fixtures(func, fixture_stack);
            func(fixture_stack);
            fixture_stack.erase(fixture_stack.begin());
        }

        void walk_fixtures(std::function<void (const std::vector<const fixture *> &fixture_stack)> func, 
            std::vector<const fixture *> &fixture_stack) const {
            for (auto item : fixture_stack.front()->fixtures) {
                fixture_stack.insert(fixture_stack.begin(), &item);
                walk_fixtures(func, fixture_stack);
                func(fixture_stack);
                fixture_stack.erase(fixture_stack.begin());
            }
        }

        std::string name;
        bounds6 bounds;
        ipv4 address;
        std::vector<fixture> fixtures;
        std::vector<vec4> points;
    };

};

#endif  // #ifndef _FIXTURE_H_
