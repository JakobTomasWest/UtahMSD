extern "C" {
#include "run.hpp"
};
#define CATCH_CONFIG_RUNNER
#include "../TestCoverageLab/catch.hpp"
bool run_tests() {
const char *argv[] = { "test" };
return (Catch::Session().run(1, argv) == 0);
}
