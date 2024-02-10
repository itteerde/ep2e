#pragma once

#include <random>
#include <map>

namespace ep2e {

    class Dice {

    private:

        class RandomNumberGenerator {
        public:
            RandomNumberGenerator(int from, int to) : distr(from, to) {}

        private:
            std::mt19937 generator = std::mt19937(std::random_device{}());
            std::uniform_int_distribution<int> distr;
        public:
            int roll() {
                return distr(generator);
            }
        };

    public:
        template<typename T>
        static T random(T range_from, T range_to) {
            std::random_device                  rand_dev;
            std::mt19937                        generator(rand_dev());
            std::uniform_int_distribution<T>    distr(range_from, range_to);
            return distr(generator);
        }

        static int d100() {
            static std::random_device                  rand_dev;
            static std::mt19937                        generator(rand_dev());
            static std::uniform_int_distribution<int>    distr(1, 100);
            return distr(generator);
        }

        static int dN(int n) {
            static std::map<int, RandomNumberGenerator> generators;
            if (!generators.contains(n)) {
                generators.emplace(n, RandomNumberGenerator(1, n));
            }
            return generators.find(n)->second.roll();
        }
    };
}