
#include <catch.hpp>

#include <MonoRunner.h>
#include <MonoLogger.h>
#include <MonoUtility.h>
#include <MonoImage.h>
#include <MonoString.h>
#include <MonoAssembly.h>
#include <MonoObject.h>
#include <MonoList.h>

#include <string>

TEST_CASE("experiment.runner", "[mono]")
{
	Mono::Runner::SetupDesc runnerSetup;
    runnerSetup.m_assembliesPath = R"(..\resources)";
	runnerSetup.m_domainName = "TestDomain";
    runnerSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
    runnerSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";

	Mono::Runner runner;
	runner.Setup(runnerSetup);

	auto imgcor = runner.GetMscorlib();
    REQUIRE(imgcor);
    imgcor->CallStaticMethod("System.Console:WriteLine(string)", Mono::Args({ "Hello World" }));

#ifdef _DEBUG
	auto a = runner.LoadAssembly(R"(../resources/ink-engine-runtime-debug.dll)");
#else
	auto a = runner.LoadAssembly(R"(../resources/ink-engine-runtime-release.dll)");
#endif
    REQUIRE(a);
	a->LookupClass("Ink.Runtime", "Tag");

	auto objVoid = a->CreateObject("Ink.Runtime", "Void");

	auto tagClass = a->LookupClass("Ink.Runtime", "Tag");
	auto obj = a->CreateObject("Ink.Runtime", "Tag", "Ink.Runtime.Tag:.ctor(string)", Mono::Args({ "tester" }));
	auto objTxt = obj->GetProperty<std::string>("text");
	obj->SetProperty<std::string>("text", "foo");
	objTxt = obj->GetProperty<std::string>("text");
}

TEST_CASE("experiment.runner.multipletimes", "[mono]")
{
	{
		Mono::Runner::SetupDesc runnerSetup;
		runnerSetup.m_assembliesPath = R"(..\resources)";
		runnerSetup.m_domainName = "TestDomain";
		runnerSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
		runnerSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";
		Mono::Runner runner;
		runner.Setup(runnerSetup);
	}

	{
		Mono::Runner::SetupDesc runnerSetup;
		runnerSetup.m_assembliesPath = R"(..\resources)";
		runnerSetup.m_domainName = "TestDomain";
		runnerSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
		runnerSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";
		Mono::Runner runner;
		runner.Setup(runnerSetup);
	}
}