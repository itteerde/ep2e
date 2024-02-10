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
    * 
    * When the GM calls for a test, roll percentile dice and compare the result to the target number. The target number is based on the character’s skill, 
      aptitude check, or rep network score. If the result is equal to or less than the target number, the test succeeds. If the result is higher, the test fails.
      Though you want to roll equal to or under the target number, you also want to roll as high as possible in order to get a superior success or to beat your opponent in opposed tests.
    */
    int opposed_test(int skillWe, int skillThey, int rollWe, int rollThey, bool flipWe, bool flipThey) {

        // criticals can be evaluated without caring about flip-flop
        if (is_critical(rollWe)) {
            if (rollWe <= skillWe) {
                if (is_critical(rollThey)) {
                    if (rollWe > rollThey) {
                        return 1;
                    }
                }
            }
        }

        // after criticals are done, for flip-flop, there is a best result individually for the opponents, so lets figure out those

        int bestWe{ rollWe };
        int bestThey{ rollThey };

        if (flipWe) {
            int flippedWe{ rollWe };
            if (rollWe > skillWe && flippedWe <= skillWe) {
                bestWe = flippedWe;
            }
            if (rollWe <= skillWe && flippedWe <= skillWe) {
                bestWe = rollWe > flippedWe ? rollWe : flippedWe;
            }
        }

        if (flipThey) {
            int flippedThey{ rollThey };
            if (rollThey > skillThey && flippedThey <= skillThey) {
                bestThey = flippedThey;
            }
            if (rollThey <= skillThey && flippedThey <= skillThey) {
                bestThey = rollThey > flippedThey ? rollThey : flippedThey;
            }
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