#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <chrono>
#include <iostream>

#include "ep2e.h"


po::options_description make_options_description() {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ;
    return desc;
}

void displayHelp(po::options_description desc) {
    for (auto o : desc.options()) {
        std::cout << "--" << o.get()->canonical_display_name() << " " << o.get()->description() << std::endl;
    }
}

int main(int argc, char* argv[])
{
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

    if (vm.count("help")) {
        displayHelp(desc);
        return 0;
    }


    std::cout << "Good Bye Earth." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    uint_fast64_t sampleSize{ 1000000 };
    uint_fast64_t total{ 0 };
    for (uint_fast64_t i = 0; i < sampleSize; i++) {
        total += ep2e::Dice::d100();
    }
    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "rolled " << sampleSize << " d100, total: " << total << ", average: " << ((double)total) / sampleSize << std::endl;

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
    std::cout << std::endl;
    std::cout << "execution: " << duration << unitString << std::endl;
}