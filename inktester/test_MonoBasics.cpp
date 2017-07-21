/**********************************************************


***********************************************************/
#include <Catch\single_include\catch.hpp>

#include <MonoRunner.h>

TEST_CASE("basic mono tests", "[mono]")
{
	Mono::Runner runner;
	runner.GetMscorlib();

	REQUIRE(true);
}
