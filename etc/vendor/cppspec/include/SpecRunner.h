#ifndef SPECRUNNER_H_
#define SPECRUNNER_H_

#include "CppSpecConfig.h"
#include <string>
#include <vector>

namespace boost {
	namespace program_options {
		class variables_map;
	}
}

namespace CppSpec {

class Reporter;
class Timer;

class CppSpec_EXPORT SpecRunner {
public:
	static void runSpecifications(int argc, char* argv[]);

private:
    static Reporter* createReporter(boost::program_options::variables_map& args, Timer& timer);
    static void runSpecs(const std::vector<std::string>& specifications, Reporter* reporter);
};

}

#endif /*SPECRUNNER_H_*/
