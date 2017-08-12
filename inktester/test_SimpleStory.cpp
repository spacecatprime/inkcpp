/**********************************************************


***********************************************************/
#include <Catch\single_include\catch.hpp>

#include <inkcpp/Types.h>
#include <inkcpp/abstract/IInkListItem.h>
#include <inkcpp/abstract/IInkList.h>
#include <inkcpp/abstract/IStoryException.h>
#include <inkcpp/abstract/IVariablesState.h>
#include <inkcpp/abstract/IStoryException.h>
#include <inkcpp/abstract/IStory.h>
#include <inkcpp/abstract/IStoryState.h>
#include <inkcpp/abstract/IChoice.h>
#include <inkcpp/InkRuntime.h>
#include <inkcpp/InkFactory.h>

#include <MonoLogger.h>
#include <MonoList.h>

TEST_CASE("simple.a.list", "[ink]")
{
	auto MakeObj = [](MonoObject* obj) -> Mono::ObjectPtr
	{
		return Mono::ObjectPtr();
	};

	MonoObject* objList = nullptr;
	Mono::List<Mono::ObjectPtr, decltype(MakeObj)> list(objList);
}

TEST_CASE("simple.construction", "[ink]")
{
	// TODO move to a more solid location for integration tests
	Ink::Runtime runtime;
	Ink::Runtime::Configuration config;
	config.m_monoSetup.m_assembliesPath = "../x64/Debug"; // should be a config file
	config.m_monoSetup.m_domainName = "ink_tester3";
	config.m_monoSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
	config.m_monoSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";
	REQUIRE(runtime.Setup(config));

	Ink::Factory f;
	auto excp = f.Create<Ink::IStoryException>(runtime, {});

	const char* k_JSON = R"({"inkVersion":17,"root":["^This is a convenience ink file where you can test out changes in the compiler code.","\n","done",{"#f":3}],"listDefs":{}})";

	auto story = f.Create<Ink::IStory>(runtime, [k_JSON](int index, void* data)
	{ 
		if (index == 0)
		{
			const char** ppData = reinterpret_cast<const char**>(data);
			*ppData = k_JSON;
			return true;
		}
		return false;
	});

	// EvaluateStory
	Mono::GetLogger()->Notice("--- EvaluateStory ---");
	{
		while (story->CanContinue())
		{
			story->Continue();
			Mono::GetLogger()->Notice(story->GetCurrentText().c_str());
		}
		if( story->GetCurrentChoices().empty() )
		{
			Mono::GetLogger()->Notice("--- End of story ---");
		}
		story->ResetErrors();
	}
}

TEST_CASE("simple.withachoice", "[ink]")
{
	// TODO move to a more solid location for integration tests
	Ink::Runtime runtime;
	Ink::Runtime::Configuration config;
	config.m_monoSetup.m_assembliesPath = "../x64/Debug"; // should be a config file
	config.m_monoSetup.m_domainName = "ink_tester3";
	config.m_monoSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
	config.m_monoSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";
	REQUIRE(runtime.Setup(config));

	Ink::Factory f;
	auto excp = f.Create<Ink::IStoryException>(runtime, {});

	const char* k_JSON = 
		R"({"inkVersion":17,"root":[[["^a title description","\n",["ev",{"^->":"0.g-0.2.$r1"},{"temp=":"$r"},"str",{"->":".^.s"},[{"#n":"$r1"}],"/str","/ev",{"*":".^.c"
           ,"flg":18},{"s":["^Choice Foo",{"->":"$r","var":true},null],"c":["ev",{"^->":"0.g-0.2.c.$r2"},"/ev",{"temp=":"$r"},{"->":".^.^.s"},[{"#n":"$r2"}],"\n","\n"
           ,{"->":"0.g-1"},{"#f":5}]}],["ev",{"^->":"0.g-0.3.$r1"},{"temp=":"$r"},"str",{"->":".^.s"},[{"#n":"$r1"}],"/str","/ev",{"*":".^.c","flg":18},{"s":["^Choice Bar"
           ,{"->":"$r","var":true},null],"c":["ev",{"^->":"0.g-0.3.c.$r2"},"/ev",{"temp=":"$r"},{"->":".^.^.s"},[{"#n":"$r2"}],"\n","\n",{"->":"0.g-1"},{"#f":5}]}],{"#n":"g-0"}]
           ,{"g-1":["^the end.","\n",["end",{"#n":"g-2"}],null]}],"done",{"#f":3}],"listDefs":{}})";

	auto story = f.Create<Ink::IStory>(runtime, [k_JSON](int index, void* data)
	{
		if (index == 0)
		{
			const char** ppData = reinterpret_cast<const char**>(data);
			*ppData = k_JSON;
			return true;
		}
		return false;
	});

	// EvaluateStory
	Mono::GetLogger()->Notice("--- withachoice ---");
	{
		while (story->CanContinue())
		{
			story->Continue();
			Mono::GetLogger()->Notice(story->GetCurrentText().c_str());
		}
		if (story->GetCurrentChoices().empty())
		{
			Mono::GetLogger()->Notice("--- End of story ---");
		}
		story->ResetErrors();
	}

	int ver = story->GetInkVersionCurrent();
	REQUIRE(ver == 17);
	//TODO story->GetStoryState();
	//TODO story->GetVariablesState();
	Ink::TagList tags = story->GetCurrentTags();
	Ink::StringList errs = story->GetCurrentErrors();
	std::string data = story->ToJsonString();
	bool test = story->HasFunction("not_there");
	std::string cont = story->ContinueMaximally();
	story->ResetState();
	story->ResetGlobals();
	story->ChooseChoiceIndex(0);
}
