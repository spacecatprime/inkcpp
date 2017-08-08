/**********************************************************


***********************************************************/
#include <Catch\single_include\catch.hpp>

#include <MonoRunner.h>

// TODO: FIXIT running too many Mono Runners seems to be a bad thing.

TEST_CASE("mono.compile.tests", "[mono][!hide]")
{
	//Mono::Runner runner;
	//runner.GetMscorlib();
	//REQUIRE(true);
}

TEST_CASE("mono.setup", "[mono]")
{
	Mono::Runner::SetupDesc desc;
	desc.m_assembliesPath = ".";
	desc.m_domainName = "mono_setup2";
	desc.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
	desc.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";

	Mono::Runner runner;
	REQUIRE(runner.Setup(desc));
}
