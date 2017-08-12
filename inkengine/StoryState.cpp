/**
	All story state information is included in the StoryState class,
	including global variables, read counts, the pointer to the current
	point in the story, the call stack (for tunnels, functions, etc),
	and a few other smaller bits and pieces. You can save the current
	state using the JSON serialisation functions ToJson and LoadJson.
*/
#include "include\inkcpp\abstract\IStoryState.h"
#include "include\inkcpp\InkFactory.h"
#include "include\inkcpp\Types.h"

#include <MonoObject.h>

namespace Ink
{
	struct StoryState : public IStoryState
	{
		Mono::ObjectPtr m_self;

		StoryState()
		{
		}

		bool Construct(Runtime& runtime, Factory::Parameters params)
		{
			if (!params(0, &m_self))
			{
				return false;
			}
			return m_self != false;
		}

		// Inherited via IStoryState
		int GetInkSaveStateVersion() override
		{
			return m_self->GetStaticField<int>("kInkSaveStateVersion");
		}
		std::string ToJson() override
		{
			return m_self->CallMethod<std::string>("ToJson");
		}
		void LoadJson(const std::string & json) override
		{
			Mono::Args args;
			args.Add(json);
			m_self->CallMethod<std::string>("LoadJson", args);
		}
		int VisitCountAtPathString(const std::string& pathString) override
		{
			Mono::Args args;
			args.Add(pathString);
			return m_self->CallMethod<int>("VisitCountAtPathString", args);
		}
		void ForceEnd() override
		{
			return m_self->CallMethod<void>("ForceEnd");
		}
	};

	template <>
	StoryStatePtr Factory::Create(Runtime& runtime, Factory::Parameters params)
	{
		auto ptr = new StoryState();
		if (ptr->Construct(runtime, params))
		{
			return StoryStatePtr(ptr);
		}
		return StoryStatePtr();
	}
};

