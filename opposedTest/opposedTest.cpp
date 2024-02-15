#include <chrono>
//#include <functional>
#include <iostream>
//#include <thread>
#include "ep2e.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

const auto& terminal_padding = "   ";

po::options_description make_options_description() {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("skillWe", po::value<int>(), "our/attacker (modified) skill")
        ("skillThey", po::value<int>(), "their/defender (modified) skill")
        ("flipWe", po::value<int>(), "do we have flip-flop available? (1|0)")
        ("flipThey", po::value<int>(), "do they have flip-flop available? (1|0)")
        ("scenarios", "run scenarios")
        ("silent", "surpress all output except the result (for interoperability)")
        ("timed", "time the run")
        ;
    return desc;
}

void displayHelp(po::options_description desc) {
    for (auto o : desc.options()) {
        std::cout << "--" << o.get()->canonical_display_name() << " " << o.get()->description() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "like .\\opposedTest.exe --skillWe=80 --skillThey=40 --flipWe=1" << std::endl;
}

int main(int argc, char* argv[])
{
    int skillWe{ 0 };
    int skillThey{ -1 };
    bool flipWe{ false };
    bool flipThey{ false };

    po::options_description desc = make_options_description();
    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch (const po::error& e) {
        std::cerr << e.what();
        return -1;
    }

    if (vm.size() < 1) {
        displayHelp(desc);
        return 0;
    }

    if (vm.count("help")) {
        displayHelp(desc);
        return 0;
    }

    if (!vm.count("skillWe") && !vm.count("silent")) {
        std::cout << terminal_padding << "no --skillWe provided, using default " << skillWe << std::endl;
    }
    else {
        skillWe = vm["skillWe"].as<int>();
    }

    if (!vm.count("skillThey") && !vm.count("silent")) {
        std::cout << terminal_padding << "no --skillThey provided, using default " << skillThey << std::endl;
    }
    else {
        skillThey = vm["skillThey"].as<int>();
    }

    if (!vm.count("flipWe") && !vm.count("silent")) {
        std::cout << terminal_padding << "no --flipWe provided, using default " << (flipWe ? 1 : 0) << std::endl;
    }
    else {
        flipWe = vm["flipWe"].as<int>() == 1 ? true : false;
    }

    if (!vm.count("flipThey") && !vm.count("silent")) {
        std::cout << terminal_padding << "no --flipThey provided, using default " << (flipThey ? 1 : 0) << std::endl;
    }
    else {
        flipThey = vm["flipThey"].as<int>() == 1 ? true : false;
    }

    if (!vm.count("scenarios")) {
        if (!vm.count("silent")) {
            std::cout << std::endl;
            std::cout << terminal_padding << "running all possible results with" << std::endl;
            std::cout << terminal_padding << "{" << skillWe << "," << skillThey << "," << flipWe << "," << flipThey << "}" << std::endl;
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

        std::cout << (vm.count("silent") ? "" : terminal_padding) << ((double)successes / 10000.0);

        if (vm.count("timed") && !vm.count("silent")) {
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
            std::cout << terminal_padding << "execution time around " << duration << unitString << std::endl;
        }

        return 0;
    }

    if (vm.count("scenarios")) {
        auto start = std::chrono::high_resolution_clock::now();
        double best{ 0 };
        double res[]{ 0,0,0,0,0 };
        res[0] = ep2e::opposed_test_p(skillWe, skillThey, flipWe, flipThey);
        //std::thread t1{ep2e::opposed_test_p_concurrent,std::ref(skillWe, skillThey, flipWe, flipThey, res[0])};
        res[1] = ep2e::opposed_test_p(skillWe, skillThey, 0, flipThey);
        res[2] = ep2e::opposed_test_p(skillWe + 20, skillThey, 0, flipThey);
        res[3] = ep2e::opposed_test_p(skillWe, skillThey, 1, flipThey);
        for (int i{ 0 }; i < sizeof(res); i++) {
            if (res[i] > best) {
                best = res[i];
            }
        }
        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << std::endl;
        std::cout << terminal_padding << "{" << skillWe << "," << skillThey << "," << flipWe << "," << flipThey << "}: " << res[0] << ((res[0]==best)?" (best)":"") << std::endl;
        std::cout << std::endl;
        std::cout << terminal_padding << "{" << skillWe << "," << skillThey << "," << 0 << "," << flipThey << "}: " << res[1] << ((res[1] == best) ? " (best)" : "") << std::endl;
        std::cout << terminal_padding << "{" << skillWe+20 << "," << skillThey << "," << 0 << "," << flipThey << "}: " << res[2] << ((res[2] == best) ? " (best)" : "") << std::endl;
        std::cout << terminal_padding << "{" << skillWe << "," << skillThey << "," << 1 << "," << flipThey << "}: " << res[3] << ((res[3] == best) ? " (best)" : "") << std::endl;

        if (vm.count("timed") && !vm.count("silent")) {
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
            std::cout << terminal_padding << "execution time around " << duration << unitString << std::endl;
        }
        return 0;
    }

    std::cout << terminal_padding << "no command found." << std::endl;
    return 0;
}

