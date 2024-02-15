#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

po::options_description make_options_description() {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("leakArea", po::value<double>(), "area of the leak in square millimeters")
        ("volume", po::value<double>(), "volume of the preasure vessel in cubic meters")
        ("seconds", po::value<int>(), "seconds to simulate")
        ("interval",po::value<int>(), "output interval in seconds")
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
    std::cout << "like .\\opposedTest.exe --leakArea=44 --skillThey=40 --flipWe=1" << std::endl;
}


int main(int argc, char* argv[])
{
    double leakArea{ 44.413 };
    double volume{ 100 };
    int seconds{ 10 };
    int interval{ 1 };

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

    if (vm.count("leakArea")) {
        leakArea = vm["leakArea"].as<double>();
    }

    if (vm.count("volume")) {
        volume = vm["volume"].as<double>();
    }

    if (vm.count("seconds")) {
        if (seconds < 0) {
            std::cout << "only positive time allowed" << std::endl;
            return 1;
        }
        seconds = vm["seconds"].as<int>();
    }

    if (vm.count("interval")) {
        if (interval < 1) {
            std::cout << "only positive interval allowed" << std::endl;
            return 1;
        }
        interval = vm["interval"].as<int>();
    }

    int time{ 0 };
    double pressure{ 1 };
    double lost_volume_per_second = leakArea * 343000;
    double loss_ratio = lost_volume_per_second / (volume * 1000000000);

    std::cout << std::endl;
    std::cout << "depreasurizing " << volume << "qm through " << leakArea << "sqmm leak (" << loss_ratio << " loss ratio)" << std::endl << std::endl;

    for (int i{ 0 }; i < seconds; i++) {
        if (i % interval == 0) {
            std::cout << i << ": " << pressure << std::endl;
        }
        pressure *= (1-loss_ratio);
    }

    return 0;
}

