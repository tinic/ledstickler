#ifndef _FIXTURE_H_
#define _FIXTURE_H_

#include <array>

#include "./vec4.h"
#include "./bounds.h"
#include "./datagram.h"

namespace ledstickler {

    struct ipv4 {    
        uint8_t a0 = 0;
        uint8_t a1 = 0;
        uint8_t a2 = 0;
        uint8_t a3 = 0;
        
        uint32_t addr() const {
            return  (uint32_t(a0)<<24)|
                    (uint32_t(a0)<<16)|
                    (uint32_t(a0)<< 8)|
                    (uint32_t(a0)<< 0);
        }
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
        
        void push(uint16_t universe) {
            universes.push_back(universe);
        }

        void push(const fixture &f) {
            bounds.add(f.bounds);
            fixtures.push_back(f);

        }
    
        void push(const vec4 &p) {
            bounds.add(p);
            points.push_back({0,p});
        }

        void walk_points(std::function<vec4 (const std::vector<fixture *> &fixture_stack, const vec4& point)> func) {
            std::vector<fixture *> fixtures_stack;
            fixtures_stack.insert(fixtures_stack.begin(), this);
            walk_points(func, fixtures_stack);
            fixtures_stack.erase(fixtures_stack.begin());
        }

        void walk_points(std::function<vec4 (const std::vector<fixture *> &fixture_stack, const vec4& point)> func, 
            std::vector<fixture *> &fixtures_stack) {
            for (auto& item : fixtures_stack.front()->fixtures) {
                fixtures_stack.insert(fixtures_stack.begin(), &item);
                walk_points(func, fixtures_stack);
                fixtures_stack.erase(fixtures_stack.begin());
            }
            for (auto& item : fixtures_stack.front()->points) {
                item.first = func(fixtures_stack, item.second);
            }
        }

        void walk_fixtures(std::function<void (const std::vector<const fixture *> &fixture_stack)> func) const {
            std::vector<const fixture *> fixtures_stack;
            fixtures_stack.insert(fixtures_stack.begin(), this);
            walk_fixtures(func, fixtures_stack);
            func(fixtures_stack);
            fixtures_stack.erase(fixtures_stack.begin());
        }

        void walk_fixtures(std::function<void (const std::vector<const fixture *> &fixture_stack)> func, 
            std::vector<const fixture *> &fixtures_stack) const {
            for (auto& item : fixtures_stack.front()->fixtures) {
                fixtures_stack.insert(fixtures_stack.begin(), &item);
                walk_fixtures(func, fixtures_stack);
                func(fixtures_stack);
                fixtures_stack.erase(fixtures_stack.begin());
            }
        }

        std::string name;
        bounds6 bounds;
        ipv4 address;
        std::vector<uint16_t> universes;
        std::vector<fixture> fixtures;
        std::vector<std::pair<vec4, vec4>> points;
    };

};

#endif  // #ifndef _FIXTURE_H_
