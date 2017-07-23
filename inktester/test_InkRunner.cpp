/**********************************************************

***********************************************************/
#include <Catch\single_include\catch.hpp>

#include <inkcpp\InkRuntime.h>

TEST_CASE("basic ink tests", "[ink]")
{
	Ink::Runtime runtime;
}

TEST_CASE("ink setup", "[ink]")
{
	Ink::Runtime::Configuration config;
	config.m_monoSetup.m_assembliesPath = "../x64/Debug"; // should be a config file
	config.m_monoSetup.m_domainName = "ink_tester";
	config.m_monoSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
	config.m_monoSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";

	Ink::Runtime runtime;
	REQUIRE(runtime.Setup(config));
}
