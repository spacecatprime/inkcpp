// ********************************************************
// Ink Story
// ********************************************************
#include "include/inkcpp/InkRuntime.h"
#include "include/inkcpp/InkFactory.h"
#include "include/inkcpp/abstract/IStory.h"
#include "include/inkcpp/InkRuntime.h"

#include <MonoAssembly.h>
#include <MonoClass.h>
#include <MonoObject.h>
#include <MonoLogger.h>
#include <MonoList.h>

#include <assert.h> 

#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)
#define THROW_NO_IMPL throw std::exception(__FILE__ ":" S__LINE__);

namespace Ink
{
	class Story : public IStory
	{
	public:
		Mono::ObjectPtr m_self;
		Mono::ClassPtr m_class;
		Ink::Runtime& m_runtime;

		Story(Mono::ObjectPtr self, Mono::ClassPtr clazz, Ink::Runtime& rt)
			: m_self(self)
			, m_class(clazz)
			, m_runtime(rt)
		{
		}

		// Inherited via IStory
		virtual int GetInkVersionCurrent() override
		{
			THROW_NO_IMPL
			return 0;
		}

		virtual ChoiceList GetCurrentChoices() override
		{
			auto list = m_self->GetProperty<Mono::ObjectPtr>("currentChoices");
			Mono::List<Mono::ObjectPtr> itemList(*list);

			int itemCount = itemList.GetCount();
			if (itemCount <= 0)
			{
				return ChoiceList();
			}

			Factory factory;
			ChoiceList choiceList;

			for (int i = 0; i < itemCount; ++i)
			{
				auto fnParameters = [i, &itemList](int index, void* arg) -> bool
				{
					if (index == 0)
					{
						auto item = itemList.GetItem(i);
						auto ppMonoObj = reinterpret_cast<Mono::ObjectPtr*>(arg);
						*ppMonoObj = item;
						return true;
					}
					return false;
				};

				auto ch = factory.Create<IChoice>(m_runtime, fnParameters);
				if (ch)
				{
					choiceList.emplace_back(ch);
				}
			}
			return choiceList;
		}
		virtual std::string GetCurrentText() override
		{
			return m_self->GetProperty<std::string>("currentText");
		}
		virtual TagList GetCurrentTags() override
		{
			THROW_NO_IMPL; // write a List<string> extraction method
			return TagList();
		}
		virtual StringList GetCurrentErrors() override
		{
			THROW_NO_IMPL; // write a List<string> extraction method
			return StringList();
		}
		virtual bool HasError() override
		{
			return m_self->GetProperty<bool>("hasError");
		}
		virtual VariablesStatePtr GetVariablesState() override
		{
			THROW_NO_IMPL;
			return VariablesStatePtr();
		}
		virtual StoryStatePtr GetStoryState() override
		{
			THROW_NO_IMPL;
			return StoryStatePtr();
		}
		virtual bool LoadStory(const std::string & jsonString) override
		{
			THROW_NO_IMPL;
			return false;
		}
		virtual std::string ToJsonString() override
		{
			return m_self->CallMethod<std::string>("ToJsonString");
		}
		virtual void ResetState() override
		{
			m_self->CallMethod<void>("ResetState");
		}
		virtual void ResetErrors() override
		{
			m_self->CallMethod<void>("ResetErrors");
		}
		virtual void ResetGlobals() override
		{
			m_self->CallMethod<void>("ResetGlobals");
		}
		virtual bool CanContinue() override
		{
			return m_self->GetProperty<bool>("canContinue");
		}
		virtual std::string ContinueMaximally() override
		{
			return m_self->CallMethod<std::string>("ContinueMaximally");
		}
		virtual void ChoosePathString(const std::string & path, ArgumentList & arguments) override
		{
			//public void ChoosePathString (string path, params object [] arguments)
			THROW_NO_IMPL;
		}
		virtual void ChooseChoiceIndex(int choiceIdx) override
		{
			Mono::Args args;
			args.Add(&choiceIdx);
			return m_self->CallMethod<void>("ChooseChoiceIndex", args);
		}
		virtual bool HasFunction(const std::string& functionName) const override
		{
			Mono::Args args;
			args.Add(functionName);
			return m_self->CallMethod<bool>("HasFunction", args);
		}
		virtual ObjectPtr EvaluateFunction(const std::string & functionName, ArgumentList & arguments) override
		{
			//public object EvaluateFunction (string functionName, params object [] arguments)
			THROW_NO_IMPL;
			return ObjectPtr();
		}
		virtual std::string Continue() override
		{
			return m_self->CallMethod<std::string>("Continue");
		}

		Mono::ObjectPtr m_object;
	};

	template <>
	StoryPtr Factory::Create(Runtime& runtime, Factory::Parameters params)
	{
		auto assm = runtime.GetInkAssembly();
		if (!assm)
		{
			return StoryPtr();
		}

		auto storyClass = assm->LookupClass("Ink.Runtime", "Story");
		if (!storyClass)
		{
			return StoryPtr();
		}

		storyClass->Reflect();

		const char* jsonData = nullptr;
		if (false == params(0, &jsonData))
		{
			return StoryPtr();
		}

		Mono::Args args({ jsonData });
		Mono::ObjectPtr obj = assm->CreateObject("Ink.Runtime", "Story", "Ink.Runtime.Story:.ctor(string)", args);

		auto s = StoryPtr(new Story(obj, storyClass, runtime));
		return s;
	}
};

// 
/*

github.com/spacecatprime/ink/blob/master/ink-engine-runtime/Story.cs

https://fossies.org/linux/mono/samples/embed/test-invoke.c
https://github.com/izuzanak/uclang/blob/75c43bc631c3171dcbed52163491343287e68dd5/uclang/mods/mono_uclm/source_files/ucl_mono.cc
https://github.com/izuzanak/uclang/blob/25398ce5e6924b8263e9fd4bf3ee3795ed548269/uclang/mods/mono_uclm/source_files/mono_module.cc

if (mono_class == list_class)
mono_c::mono_exc = NULL;
MonoArray *mono_array = (MonoArray *)mono_runtime_invoke(list_to_array, mono_obj, NULL, &mono_exc);
BIC_MONO_CHECK_EXCEPTION_RETURN_NULL();

uintptr_t length = mono_array_length(mono_array);

pointer_array_s *array_ptr = it.get_new_array_ptr();
BIC_CREATE_NEW_LOCATION(arr_location, c_bi_class_array, array_ptr);

if (length > 0)
{
	uintptr_t idx = 0;
	do {
		MonoObject *mono_item = mono_array_get(mono_array, MonoObject *, idx);
		location_s *item_location = mono_object_value(it, mono_item, source_pos);

		// - ERROR -
		if (item_location == NULL)
		{
			it.release_location_ptr(arr_location);
			return NULL;
		}

		// - insert item to array -
		array_ptr->push(item_location);

	} while (++idx < length);
}

return arr_location;
  }
*/