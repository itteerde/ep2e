#pragma once

#include <random>
#include <map>

namespace ep2e {

    /**
    * flip-flops the roll r. r [0..99].
    */
    int flip(int r) {
        int flipped{ 0 };
        flipped += (r % 10) * 10;
        flipped += r / 10;

        return flipped;
    }

    /**
    * tests if a Test Roll is critical. true if critical, false if not.
    */
    bool is_critical(int r) {
        if (r / 10 == r % 10) {
            return true;
        }

        return false;
    }

    /**
    * 1 if we win, 0 otherwise (the defender wins if draw, so there is no draw)
    */
    int contestedTest(int skillWe, int skillThey, int rollWe, int rollThey, bool flipWe, bool flipThey) {
        if (rollWe <= skillWe && rollThey > skillThey) {
            return 1;
        }
        if (rollWe > skillWe && rollThey <= skillThey) {
            return 0;
        }

        return 0;
    }

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
            static std::uniform_int_distribution<int>    distr(0, 99);
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