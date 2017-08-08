// ********************************************************
// Ink Story
// ********************************************************
#include "include/inkcpp/InkRuntime.h"
#include "include/inkcpp/InkFactory.h"
#include "include/inkcpp/abstract/IStory.h"

#include <MonoAssembly.h>
#include <MonoClass.h>
#include <MonoObject.h>
#include <MonoLogger.h>
#include <MonoList.h>

// https://fossies.org/linux/mono/samples/embed/test-invoke.c

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

		Story(Mono::ObjectPtr self, Mono::ClassPtr clazz)
			: m_self(self)
			, m_class(clazz)
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
			//THROW_NO_IMPL //public List<Choice> currentChoices
			// https://github.com/izuzanak/uclang/blob/75c43bc631c3171dcbed52163491343287e68dd5/uclang/mods/mono_uclm/source_files/ucl_mono.cc

			auto list = m_self->GetProperty<Mono::ObjectPtr>("currentChoices");

			Mono::List<ObjectPtr> itemList(*list);
			auto r1 = itemList.GetCapacity();
			auto r2 = itemList.GetCount();
			auto r3 = itemList.GetItem(0);

			if (itemList.GetCount() == 0)
			{
				return ChoiceList();
			}

			int index = 0;
			Mono::Args args;
			args.Add(&index);
			auto items = list->CallMethod<Mono::ObjectPtr>("System.Collections.Generic.List`1<Ink.Runtime.Choice>:get_Item (int)", args);
		
			return ChoiceList();
		}
		virtual std::string GetCurrentText() override
		{
			return m_self->GetProperty<std::string>("currentText");
		}
		virtual TagList GetCurrentTags() override
		{
			THROW_NO_IMPL;
			return TagList();
		}
		virtual StringList GetCurrentErrors() override
		{
			THROW_NO_IMPL;
			return StringList();
		}
		virtual bool HasError() override
		{
			THROW_NO_IMPL;
			return false;
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
			THROW_NO_IMPL;
			return std::string();
		}
		virtual void ResetState() override
		{
			THROW_NO_IMPL;
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
			THROW_NO_IMPL;
			return std::string();
		}
		virtual void ChoosePathString(const std::string & path, ArgumentList & arguments) override
		{
			THROW_NO_IMPL;
		}
		virtual void ChooseChoiceIndex(int choiceIdx) override
		{
			THROW_NO_IMPL;
		}
		virtual bool HasFunction(const std::string & functionName) const override
		{
			THROW_NO_IMPL;
			return false;
		}
		virtual ObjectPtr EvaluateFunction(const std::string & functionName, ArgumentList & arguments) override
		{
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

		auto s = StoryPtr(new Story(obj, storyClass));
		return s;
	}
};

// 
/*
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