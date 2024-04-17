#pragma once

#include <random>
#include <map>

namespace ep2e {

    /**
    * flip-flops the roll r. r [0..99].
    */
    inline int flip(int r) {
        return (r % 10) * 10 + r / 10;
    }

    /**
    * tests if a Test Roll is critical. true if critical, false if not.
    */
    inline bool is_critical(int r) {
        return (r / 10 == r % 10);
    }

    /**
    * 1 if we win, 0 otherwise (the defender wins if draw, so there is no draw)
    * 
    * When the GM calls for a test, roll percentile dice and compare the result to the target number. The target number is based on the character’s skill, 
      aptitude check, or rep network score. If the result is equal to or less than the target number, the test succeeds. If the result is higher, the test fails.
      Though you want to roll equal to or under the target number, you also want to roll as high as possible in order to get a superior success or to beat your opponent in opposed tests.
    */
    int opposed_test(int skillWe, int skillThey, int rollWe, int rollThey, bool flipWe, bool flipThey) {

        // start criticals
        // only ours is a critical success
        if (
            (is_critical(rollWe) && rollWe <= skillWe) &&
            (!is_critical(rollThey) || rollThey > skillThey)
            ) {
            return 1;
        }
        // only theirs is a critical success
        if (
            (is_critical(rollThey) && rollThey <= skillThey) &&
            (!is_critical(rollWe) || rollWe > skillWe)
            ) {
            return 0;
        }
        // both are critical success
        if (
            (is_critical(rollWe) && rollWe <= skillWe) &&
            (is_critical(rollThey) && rollThey <= skillThey)
            ) {
            if (rollWe > rollThey) {
                return 1;
            }
            return 0;
        }
        // end criticals

        // after criticals are done, for flip-flop, there is a best result individually for the opponents, so lets figure out those

        int bestWe{ rollWe };
        int bestThey{ rollThey };

        if (flipWe) {
            int flippedWe = flip(rollWe);
            if (rollWe > skillWe && flippedWe <= skillWe) {
                bestWe = flippedWe;
            }
            if (rollWe <= skillWe && flippedWe <= skillWe) {
                bestWe = rollWe > flippedWe ? rollWe : flippedWe;
            }
        }

        if (flipThey) {
            int flippedThey = flip(rollThey);
            if (rollThey > skillThey && flippedThey <= skillThey) {
                bestThey = flippedThey;
            }
            if (rollThey <= skillThey && flippedThey <= skillThey) {
                bestThey = rollThey > flippedThey ? rollThey : flippedThey;
            }
        }

        // only we succeed
        if (bestWe <= skillWe && bestThey > skillThey) {
            return 1;
        }

        // only they succeed
        if (bestThey <= skillThey && bestWe > skillWe) {
            return 0;
        }
        
        // both succeed
        if (bestWe <= skillWe && bestThey <= skillThey) {
            if (bestWe > bestThey) {
                return 1;
            }
            return 0;
        }
        
        // both fail
        if (skillWe > skillThey) {
            return 1;
        }

        return 0;
    }

    double opposed_test_p(int skillWe, int skillThey, bool flipWe, bool flipThey) {
        int successes{ 0 };
        for (int i{ 0 }; i < 99; i++) {//99 always fails
            for (int j{ 0 }; j <= 99; j++) {
                if (ep2e::opposed_test(skillWe, skillThey, i, j, flipWe, flipThey) == 1) {
                    successes++;
                }
            }
        }
        return successes / 10000.0;
    }

    void opposed_test_p_concurrent(int skillWe, int skillThey, bool flipWe, bool flipThey, double& result) {
        result = opposed_test_p(skillWe, skillThey, flipWe, flipThey);
    }

    int unopposed_test(int skill, bool flipFlop, int roll) {
        if (is_critical(roll)) {
            if (roll <= skill) {
                return 1;
            }
            else {
                return 0;
            }
        }

        if (roll <= skill) {
            return 1;
        }

        if (flipFlop && flip(roll) <= skill) {
            return 1;
        }

        return 0;
    }

    double unopposed_test_p(int skill, bool flip) {
        int successes{ 0 };
        for (int r{ 0 }; r < 99; r++) {//99 always fails
            if (unopposed_test(skill, flip, r) == 1) {
                successes++;
            }
        }
        return successes / 100.0;
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