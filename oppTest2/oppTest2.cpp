#include <chrono>
#include <iostream>
#include <CLI\CLI.hpp>
#include "../ep2e/ep2e.h"

const static std::string INDENT = "   ";

static void display_example() {
    std::cout << INDENT << "not implemented yet";
}

int main(int argc, char* argv[])
{
    bool silent{ false };
    bool example{ false };
    bool scenarios{ true };
    bool timed{ false };

    int skillWe{ 0 };
    int skillThey{ 0 };
    bool flipWe{ false };
    bool flipThey{ false };

    CLI::App app{ "Eclipse Phase 2nd Edition: Opposed Tests" };

    app.add_option("--skillWe", skillWe, "Skill We/Attacker")->required();
    app.add_option("--skillThey", skillThey, "Skill they/Defender")->required();
    app.add_flag("--poolWe", flipWe, "Pool We/Attacker");
    app.add_flag("--poolThey", flipThey, "Pool They/Attacker");

    app.add_flag("--silent", silent, "Silent");
    app.add_flag("--example", example, "Display example use")->default_val(true);
    app.add_option("--scenarios", scenarios, "Display Scenarios");
    app.add_flag("--timed", timed, "Display Timing")->default_val(true);

    CLI11_PARSE(app, argc, argv);

    if (example) {
        display_example();
        return 0;
    }

    if (!silent) {
        std::cout << std::endl;
        std::cout << INDENT << "Skill We:   " << skillWe << std::endl;
        std::cout << INDENT << "Skill They: " << skillThey << std::endl;
        std::cout << INDENT << "Pool We: " << flipWe << std::endl;
        std::cout << INDENT << "Pool They: " << flipThey << std::endl;
    }

    if (!scenarios) {
        if (!silent) {
            std::cout << std::endl;
            std::cout << INDENT << "running all possible results with" << std::endl;
            std::cout << INDENT << "{" << skillWe << "," << skillThey << "," << flipWe << "," << flipThey << "}" << std::endl;
            std::cout << std::endl;
        }

        auto start = std::chrono::high_resolution_clock::now();
        int successes{ 0 };
        for (int i{ 0 }; i <= 99; i++) {
            for (int j{ 0 }; j <= 99; j++) {
                if (ep2e::opposed_test(skillWe, skillThey, i, j, flipWe, flipThey) == 1) {
                    successes++;
                }
            }
        }
        auto stop = std::chrono::high_resolution_clock::now();

        if (!silent) {
            std::cout << INDENT;
        }
        std::cout << ((double)successes / 10000.0);

        if (timed && !silent) {
            std::string unitString = "ms";
            auto duration = (duration_cast<std::chrono::milliseconds>(stop - start)).count();
            if (duration < 10) {
                duration = (duration_cast<std::chrono::microseconds>(stop - start)).count();;
                unitString = "us";
            }
            if (duration < 10) {
                duration = (duration_cast<std::chrono::nanoseconds>(stop - start)).count();;
                unitString = "ns";
            }
            std::cout << std::endl << std::endl;
            std::cout << INDENT << "execution time around " << duration << unitString << std::endl;
        }
    }

    if (scenarios) {
        auto start = std::chrono::high_resolution_clock::now();
        int best{ 0 };
        double res[]{ 0,0,0,0 };
        res[0] = ep2e::opposed_test_p(skillWe, skillThey, flipWe, flipThey);
        //std::thread t1{ep2e::opposed_test_p_concurrent,std::ref(skillWe, skillThey, flipWe, flipThey, res[0])};
        res[1] = ep2e::opposed_test_p(skillWe, skillThey, 0, flipThey);
        res[2] = ep2e::opposed_test_p(skillWe + 20, skillThey, 0, flipThey);
        res[3] = ep2e::opposed_test_p(skillWe, skillThey, 1, flipThey);
        for (int i{ 0 }; i < 4; i++) {
            if (res[i] >= res[best]) {
                best = i;
            }
        }
        auto stop = std::chrono::high_resolution_clock::now();

        if (!silent) {
            std::cout << std::endl;
            std::cout << INDENT << "{" << skillWe << "," << skillThey << "," << flipWe << "," << flipThey << "}: " << res[0] << ((0 == best) ? " (best)" : "") << std::endl;
            std::cout << std::endl;
            std::cout << INDENT << "{" << skillWe << "," << skillThey << "," << 0 << "," << flipThey << "}: " << res[1] << ((1 == best) ? " (best)" : "") << std::endl;
            std::cout << INDENT << "{" << skillWe + 20 << "," << skillThey << "," << 0 << "," << flipThey << "}: " << res[2] << ((2 == best) ? " (best)" : "") << std::endl;
            std::cout << INDENT << "{" << skillWe << "," << skillThey << "," << 1 << "," << flipThey << "}: " << res[3] << ((3 == best) ? " (best)" : "") << std::endl;
        }
        else {
            std::cout << res[0] << "," << res[1] << "," << res[2] << "," << res[3];
            return 0;
        }

        if (timed && !silent) {
            std::string unitString = "ms";
            auto duration = (duration_cast<std::chrono::milliseconds>(stop - start)).count();
            if (duration < 10) {
                duration = (duration_cast<std::chrono::microseconds>(stop - start)).count();;
                unitString = "us";
            }
            if (duration < 10) {
                duration = (duration_cast<std::chrono::nanoseconds>(stop - start)).count();;
                unitString = "ns";
            }
            std::cout << std::endl << std::endl;
            std::cout << INDENT << "execution time around " << duration << unitString << std::endl;
        }
    }


    if (!silent) {
        std::cout << std::endl;
    }

    return 0;
}

