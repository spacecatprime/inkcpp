
#include <catch.hpp>

#include <MonoRunner.h>
#include <MonoLogger.h>
#include <MonoUtility.h>
#include <MonoImage.h>
#include <MonoString.h>
#include <MonoAssembly.h>
#include <MonoObject.h>
#include <MonoList.h>
#include <MonoDomain.h>
#include <MonoLogger.h>
#include <MonoArray.h>

#include <string>

TEST_CASE("experiment.compile", "[mono][!hide]")
{
	Mono::SetLogger(Mono::CreateDefaultLogger());

	Mono::Runner::SetupDesc runnerSetup;
    runnerSetup.m_assembliesPath = R"(..\resources)";
	runnerSetup.m_domainName = "TestDomain";
    runnerSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
    runnerSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";

	Mono::Runner runner;
	runner.Setup(runnerSetup);

	auto imgcor = runner.GetMscorlib();
    REQUIRE(imgcor);

	// https://github.com/CRYTEK/CRYENGINE/tree/26524289c15a660965a447dcb22628643917c820/Code/CryManaged/CryMonoBridge

	auto klassCompilerResults = runner.GetSystemAssembly()->LookupClass("System.CodeDom.Compiler", "CompilerResults");
	auto klassCompilerParameters = runner.GetSystemAssembly()->LookupClass("System.CodeDom.Compiler", "CompilerParameters");
//	auto objCodeDomProvider = runner.GetSystemAssembly()->GetImage()->CallStaticMethod("System.CodeDom.Compiler.CodeDomProvider:CreateProvider(string)", Mono::Args({ "CSharp" }));
	auto objCodeDomProvider = runner.GetSystemAssembly()->CreateObject("Microsoft.CSharp", "CSharpCodeProvider");
	REQUIRE(objCodeDomProvider);
	REQUIRE(objCodeDomProvider->RegisterVirtualMethod("System.CodeDom.Compiler", "CodeDomProvider", "CompileAssemblyFromSource", 2));

	auto objCompilerParameters = runner.GetSystemAssembly()->CreateObject("System.CodeDom.Compiler", "CompilerParameters");
	objCompilerParameters->SetProperty("GenerateInMemory", true);
	objCompilerParameters->SetProperty("GenerateExecutable", false);
	objCompilerParameters->SetProperty("TreatWarningsAsErrors", false);
//	objCompilerParameters->SetProperty("CompilerOptions", std::string("/target:library")); 
//	objCompilerParameters->SetProperty("CompilerOptions", std::string("-target:library")); 

#if false
	const char* script[] = 
	{
		"using System;",
		"namespace HelloWorld",
		"{",
			"public class Hello",
			"{",
				"public Hello()",
				"{",
					"System.Console.WriteLine(\"Hello World!\");",
				"}",
			"}",
		"}",
	};

	auto scriptLines = static_cast<std::uint32_t>(sizeof script / sizeof script[0]);
	Mono::ArrayPointers<Mono::StringPtr> arry(scriptLines, mono_get_string_class());
	for (std::uint32_t i = 0; i < arry.Count(); ++i)
	{
		auto data = new Mono::String(script[i]);
		auto line = Mono::StringPtr(data);
		arry.Add(i, line);
	}
#else
	std::string data = R"((
	using System;
	namespace HelloWorld
	{
		public class Hello
		{
			public Hello()
			{
				System.Console.WriteLine(""Hello World!"");
			}
		}
	}
		))";

	data = 
R"((
	namespace Foo
	{
		public class Bar
		{
			public Bar()
			{
			}
		}
	}
))";

	Mono::String monoString(data);
	Mono::Array<MonoString*> arry(1);
	arry.Add(0, monoString);
#endif

	Mono::Args args;
	args.Add(objCompilerParameters->GetInstance<MonoObject>());
	args.Add(arry);
	auto results = objCodeDomProvider->CallMethod<Mono::ObjectPtr>("CompileAssemblyFromSource", args);
	REQUIRE(results);

	auto objErrors = results->GetProperty<Mono::ObjectPtr>("Errors");
	REQUIRE(objErrors);
	int errorItem = 0;
	while (errorItem >= 0)
	{
		args.Clear();
		args.Add(&errorItem);
		auto error = objErrors->CallMethod <Mono::ObjectPtr>("get_Item", args);
		if (error)
		{
			auto errorText = error->GetProperty<std::string>("ErrorText");
			std::printf("%s\n", errorText.c_str());
			errorItem++;
		}
		else
		{
			break;
		}
	}
	REQUIRE_FALSE(objErrors->GetProperty<bool>("HasErrors"));
	auto objAssmby = results->GetProperty<Mono::ObjectPtr>("CompiledAssembly");
	auto objHello = objAssmby->CallMethod<Mono::ObjectPtr>("CreateInstance", Mono::Args({"HelloWorld.Hello"}));

	// TODO parse the results
	// TODO execute the entry point
}

/*
 static void Main()
{
    Console.WriteLine("test for compile code");
    var results = Compiler.Compile(s_HelloWorldScript);
    Console.WriteLine(string.Format("CompiledAssembly.FullName = {0}", results.CompiledAssembly.FullName));
    object obj = results.CompiledAssembly.CreateInstance("HelloWorld.Hello");
    Console.ReadKey();
}
*/