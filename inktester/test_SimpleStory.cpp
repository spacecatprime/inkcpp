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

/*
    public class TestRunner
    {
        private Ink.Runtime.Story m_story;
        private System.Random m_rand;

        void EvaluateStory()
        {
            while (m_story.canContinue)
            {
                m_story.Continue();
                Console.Write(m_story.currentText);
            }
            if (m_story.currentChoices.Count == 0)
            {
                Console.WriteLine("--- End of story ---");
            }
            m_story.ResetErrors();
        }

        public bool Load(string aFilename)
        {
            m_rand = new System.Random();

            string json = System.IO.File.ReadAllText(aFilename);
            m_story = new Ink.Runtime.Story(json);

            EvaluateStory();

            while (m_story.currentChoices.Count > 0)
            {
                var choices = m_story.currentChoices;
                if (choices.Count == 0)
                {
                    return false;
                }
                var randomIndex = m_rand.Next() % choices.Count;
                m_story.ChooseChoiceIndex(randomIndex);
                EvaluateStory();
            }

            System.Console.ReadKey();
            return true;
        }
    }


*/