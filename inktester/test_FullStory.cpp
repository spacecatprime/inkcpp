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
#include <MonoObject.h>
#include <MonoUtility.h>

#include <cstdlib>
#include <ctime>

// TODO move to a more solid location for integration tests
struct TestFixture
{
	static int s_testNumber;

	TestFixture()
	{
	}

	bool Prepare()
	{
		char inktest[128];
		sprintf_s(inktest, "ink_tester_%d", s_testNumber++);

		Ink::Runtime::Configuration config;
		config.m_monoSetup.m_assembliesPath = "../x64/Debug";
		config.m_monoSetup.m_domainName = inktest;
		config.m_monoSetup.m_monoEtcConfigFolder = R"(..\3rd_party\Mono\etc)";
		config.m_monoSetup.m_monoLibFolder = R"(..\3rd_party\Mono\lib)";
		REQUIRE(m_runtime.Setup(config));
		return true;
	}

	Ink::StoryPtr CreateStory(const char* json)
	{
		Ink::Factory f;
		Ink::StoryPtr story = f.Create<Ink::IStory>(m_runtime, [json](int index, void* data)
		{
			if (index == 0)
			{
				const char** ppData = reinterpret_cast<const char**>(data);
				*ppData = json;
				return true;
			}
			return false;
		});
		return story;
	}

	Ink::Runtime m_runtime;
};
int TestFixture::s_testNumber = 0;

TEST_CASE("full.withachoice", "[ink]")
{
	TestFixture fixture;
	fixture.Prepare();
	
	const char* k_JSON = 
		R"({"inkVersion":17,"root":[[["^a title description","\n",["ev",{"^->":"0.g-0.2.$r1"},{"temp=":"$r"},"str",{"->":".^.s"},[{"#n":"$r1"}],"/str","/ev",{"*":".^.c"
           ,"flg":18},{"s":["^Choice Foo",{"->":"$r","var":true},null],"c":["ev",{"^->":"0.g-0.2.c.$r2"},"/ev",{"temp=":"$r"},{"->":".^.^.s"},[{"#n":"$r2"}],"\n","\n"
           ,{"->":"0.g-1"},{"#f":5}]}],["ev",{"^->":"0.g-0.3.$r1"},{"temp=":"$r"},"str",{"->":".^.s"},[{"#n":"$r1"}],"/str","/ev",{"*":".^.c","flg":18},{"s":["^Choice Bar"
           ,{"->":"$r","var":true},null],"c":["ev",{"^->":"0.g-0.3.c.$r2"},"/ev",{"temp=":"$r"},{"->":".^.^.s"},[{"#n":"$r2"}],"\n","\n",{"->":"0.g-1"},{"#f":5}]}],{"#n":"g-0"}]
           ,{"g-1":["^the end.","\n",["end",{"#n":"g-2"}],null]}],"done",{"#f":3}],"listDefs":{}})";

	Ink::Factory f;
	auto story = f.Create<Ink::IStory>(fixture.m_runtime, [k_JSON](int index, void* data)
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
	auto fnEvaluateStory = [story]()
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
	};

	std::srand(static_cast<unsigned int>(std::time(0))); 
	int random_variable = std::rand();

	fnEvaluateStory();

	Ink::ChoiceList choiceList = story->GetCurrentChoices();
	while (false == choiceList.empty())
	{
		auto state = story->GetStoryState();
		auto ver = state->GetInkSaveStateVersion(); // TODO (why): field checks do not seem to work?
		auto stateJson = state->ToJson();
		// TODO some tests

		int randomIndex = static_cast<int>(std::rand() % choiceList.size());

		Mono::GetLogger()->Notice(Mono::GetLogger()->ToString("Random index is %d of %d", randomIndex, choiceList.size()));

		story->ChooseChoiceIndex(randomIndex);
		fnEvaluateStory();
		choiceList = story->GetCurrentChoices();
	}
}

TEST_CASE("full.visit.counts.when.choosing", "[ink]")
{
	TestFixture fixture;
	fixture.Prepare();

	const char* k_JSON = R"({"inkVersion":17,"root":["done",{"TestKnot":["^this is a test","\n",["ev","str","^Next","/str","/ev",{"*":".^.c","flg":4},{"c":["\n",{"->":"TestKnot2"},"\n",null]}],{"#f":3}],"TestKnot2":["^this is the end","\n","end",{"#f":3}],"#f":3}],"listDefs":{}})";
	auto story = fixture.CreateStory(k_JSON);

	REQUIRE(0 == story->GetStoryState()->VisitCountAtPathString("TestKnot"));
	REQUIRE(0 == story->GetStoryState()->VisitCountAtPathString("TestKnot2"));

	story->ChoosePathString("TestKnot", {});

	REQUIRE(1 == story->GetStoryState()->VisitCountAtPathString("TestKnot"));
	REQUIRE(0 == story->GetStoryState()->VisitCountAtPathString("TestKnot2"));

	story->Continue();

	REQUIRE(1 == story->GetStoryState()->VisitCountAtPathString("TestKnot"));
	REQUIRE(0 == story->GetStoryState()->VisitCountAtPathString("TestKnot2"));

	story->ChooseChoiceIndex(0);

	REQUIRE(1 == story->GetStoryState()->VisitCountAtPathString("TestKnot"));

	// At this point, we have made the choice, but the divert *within* the choice won't yet have been evaluated.
	REQUIRE(0 == story->GetStoryState()->VisitCountAtPathString("TestKnot2"));

	story->Continue();

	REQUIRE(1 == story->GetStoryState()->VisitCountAtPathString("TestKnot"));
	REQUIRE(1 == story->GetStoryState()->VisitCountAtPathString("TestKnot2"));
}

/*
[Test()]
public void TestVariableGetSetAPI()
{
var story = CompileString(@"
VAR x = 5

{x}

* [choice]
-
{x}

* [choice]
-

{x}

* [choice]
-

{x}

-> DONE
");

// Initial state
Assert.AreEqual("5\n", story.ContinueMaximally());
Assert.AreEqual(5, story.variablesState["x"]);

story.variablesState["x"] = 10;
story.ChooseChoiceIndex(0);
Assert.AreEqual("10\n", story.ContinueMaximally());
Assert.AreEqual(10, story.variablesState["x"]);

story.variablesState["x"] = 8.5f;
story.ChooseChoiceIndex(0);
Assert.AreEqual("8"+ System.Globalization.NumberFormatInfo.CurrentInfo.NumberDecimalSeparator+"5\n", story.ContinueMaximally());
Assert.AreEqual(8.5f, story.variablesState["x"]);

story.variablesState["x"] = "a string";
story.ChooseChoiceIndex(0);
Assert.AreEqual("a string\n", story.ContinueMaximally());
Assert.AreEqual("a string", story.variablesState["x"]);

Assert.AreEqual(null, story.variablesState["z"]);

// Not allowed arbitrary types
Assert.Throws<StoryException>(() =>
{
story.variablesState["x"] = new System.Text.StringBuilder();
});
}
*/
TEST_CASE("full.variable.get.set", "[ink]")
{
	TestFixture fixture;
	fixture.Prepare();

	const char* k_JSON = R"({"inkVersion":17,"root":[[["G>","ev",{"VAR?":"x"},"out","/ev","G<",null],"\n",["ev","str","^choice","/str","/ev",{"*":".^.c","flg":20},{"c":["\n","\n",{"->":"0.g-0"},{"#f":5}]}],{"g-0":[["G>","ev",{"VAR?":"x"},"out","/ev","G<",null],"\n",["ev","str","^choice","/str","/ev",{"*":".^.c","flg":20},{"c":["\n","\n",{"->":"0.g-1"},{"#f":5}]}],null],"g-1":[["G>","ev",{"VAR?":"x"},"out","/ev","G<",null],"\n",["ev","str","^choice","/str","/ev",{"*":".^.c","flg":20},{"c":["\n","\n",{"->":"0.g-2"},{"#f":5}]}],null],"g-2":[["G>","ev",{"VAR?":"x"},"out","/ev","G<",null],"\n","done",null]}],"done",{"global decl":["ev",5,{"VAR=":"x"},"/ev","end",null],"#f":3}],"listDefs":{}})";
	auto story = fixture.CreateStory(k_JSON);

	// Initial state
	REQUIRE(story->ContinueMaximally() == "5\n");
	REQUIRE(story->GetVariablesState()->GetVariable("x")->AsUint32() == 5);

	story->GetVariablesState()->SetVariable("x", Mono::BoxValue<int>(10));
	story->ChooseChoiceIndex(0);
	REQUIRE(story->ContinueMaximally() == "10\n");
	REQUIRE(story->GetVariablesState()->GetVariable("x")->AsUint32() == 10);
}

/*
https://raw.githubusercontent.com/inkle/ink/1c3308a8e4e92d88bb7a14ae899884f5b2715dde/tests/Tests.cs

{"inkVersion":17,"root":["done",{"TestKnot":["^this is a test","\n",["ev","str","^Next","/str","/ev",{"*":".^.c","flg":4},{"c":["\n",{"->":"TestKnot2"},"\n",null]}],{"#f":3}],"TestKnot2":["^this is the end","\n","end",{"#f":3}],"#f":3}],"listDefs":{}}

[Test()]
public void TestVisitCountsWhenChoosing()
{
var storyStr =
@"
== TestKnot ==
this is a test
+ [Next] -> TestKnot2

== TestKnot2 ==
this is the end
-> END
";

Story story = CompileString(storyStr);

Assert.AreEqual (0, story.state.VisitCountAtPathString ("TestKnot"));
Assert.AreEqual (0, story.state.VisitCountAtPathString ("TestKnot2"));

story.ChoosePathString ("TestKnot");

Assert.AreEqual (1, story.state.VisitCountAtPathString ("TestKnot"));
Assert.AreEqual (0, story.state.VisitCountAtPathString ("TestKnot2"));

story.Continue ();

Assert.AreEqual (1, story.state.VisitCountAtPathString ("TestKnot"));
Assert.AreEqual (0, story.state.VisitCountAtPathString ("TestKnot2"));

story.ChooseChoiceIndex (0);

Assert.AreEqual (1, story.state.VisitCountAtPathString ("TestKnot"));

// At this point, we have made the choice, but the divert *within* the choice
// won't yet have been evaluated.
Assert.AreEqual (0, story.state.VisitCountAtPathString ("TestKnot2"));

story.Continue ();

Assert.AreEqual (1, story.state.VisitCountAtPathString ("TestKnot"));
Assert.AreEqual (1, story.state.VisitCountAtPathString ("TestKnot2"));
}


protected Ink.Parsed.Story CompileStringWithoutRuntime(string str, bool testingErrors = false)
{
_testingErrors = testingErrors;
_errorMessages.Clear();
_warningMessages.Clear();

InkParser parser = new InkParser(str, null, TestErrorHandler);
var parsedStory = parser.Parse();

if (!testingErrors) {
Assert.IsNotNull (parsedStory);
Assert.IsFalse (parsedStory.hadError);
}

if (parsedStory) {
parsedStory.ExportRuntime (TestErrorHandler);
}

return parsedStory;
}


// Helper compile function
protected Story CompileString(string str, bool countAllVisits = false, bool testingErrors = false)
{
_testingErrors = testingErrors;
_errorMessages.Clear();
_warningMessages.Clear();

InkParser parser = new InkParser(str, null, TestErrorHandler);
var parsedStory = parser.Parse();
parsedStory.countAllVisits = countAllVisits;

Story story = parsedStory.ExportRuntime(TestErrorHandler);
if( !testingErrors )
Assert.AreNotEqual(null, story);

// Convert to json and back again
if (_mode == TestMode.JsonRoundTrip && story != null)
{
var jsonStr = story.ToJsonString();
story = new Story(jsonStr);
}

return story;
}

*/
