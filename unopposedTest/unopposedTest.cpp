#include <iostream>
#include "CLI\CLI.hpp"
#include "../ep2e/ep2e.h"

const static std::string INDENT = "   ";

int main(int argc, char* argv[]){
	int skill{ 0 };
	bool flip{ false };
	bool silent{ false };

	CLI::App app{ "Eclipse Phase 2nd Edition: Unopposed Tests" };
    
    app.add_option("--skill", skill, "Skill")->required();
    app.add_flag("--flip", flip, "Flip-flop");
	app.add_flag("--silent", silent, "Silent (for use as input to other programs)");

	CLI11_PARSE(app, argc, argv);

	int best{ 0 };
	double res[]{ 0,0,0};
	res[0] = ep2e::unopposed_test_p(skill, flip);
	res[1] = ep2e::unopposed_test_p(skill+20, false);
	res[2] = ep2e::unopposed_test_p(skill, true);

	for (int i{ 0 }; i < 3; i++) {
		if (res[i] >= res[best]) {
			best = i;
		}
	}

	if (!silent) {
		std::cout << std::endl;
		std::cout << INDENT << "{ skill: " << skill << ", " << "flip: " << flip << " } : " << res[0] << ((0 == best) ? " (best)" : "") << std::endl;
		std::cout << INDENT << "{ skill: " << skill + 20 << ", " << "flip: " << false << " } : " << res[1] << ((1 == best) ? " (best)" : "") << std::endl;
		std::cout << INDENT << "{ skill: " << skill << ", " << "flip: " << true << " } : " << res[2] << ((2 == best) ? " (best)" : "") << std::endl;
		std::cout << std::endl;
	}
	else {
		std::cout << "{" << res[0] << "," << res[1] << "," << res[2] << "}" << std::endl;
	}

	return 0;
}

