
#include <catch.hpp>

#include <MonoRunner.h>
#include <MonoLogger.h>
#include <MonoUtility.h>
#include <MonoImage.h>
#include <MonoString.h>
#include <MonoAssembly.h>
#include <MonoObject.h>

#include <string>

TEST_CASE("experiment.runner", "[mono]")
{
	Mono::Runner::SetupDesc runnerSetup;
	runnerSetup.m_assembliesPath = R"(F:\projects\research\mono_experiements\monorunner\mono_ink\ink-engine-runtime\Debug)";
	runnerSetup.m_domainName = "TestDomain";
	runnerSetup.m_monoEtcConfigFolder = R"(F:\projects\research\mono_experiements\Mono\etc)";
	runnerSetup.m_monoLibFolder = R"(F:\projects\research\mono_experiements\Mono\lib)";

	Mono::Runner runner;
	runner.Setup(runnerSetup);

	auto imgcor = runner.GetMscorlib();
	imgcor->CallStaticMethod("System.Console:WriteLine(string)", Mono::Args({ "Hello World" }));

	auto a = runner.LoadAssembly("ink-engine-runtime.dll");
	a->LookupClass("Ink.Runtime", "Tag");

	auto objVoid = a->CreateObject("Ink.Runtime", "Void");

	auto tagClass = a->LookupClass("Ink.Runtime", "Tag");
	auto obj = a->CreateObject("Ink.Runtime", "Tag", "Ink.Runtime.Tag:.ctor(string)", Mono::Args({ "tester" }));
	auto objTxt = obj->GetProperty<std::string>("text");
	obj->SetProperty<std::string>("text", "foo");
	objTxt = obj->GetProperty<std::string>("text");
}
