/**
*/
#include "include\inkcpp\abstract\IChoice.h"
#include "include\inkcpp\InkFactory.h"
#include "include\inkcpp\Types.h"

#include <MonoObject.h>

namespace Ink
{
	struct Choice : public IChoice
	{
		int m_index;
		Mono::ObjectPtr m_monoObj;
		std::string m_text;
		std::string m_pathStringOnChoice;
		std::string m_sourcePath;

		Choice()
			: m_index(-1)
		{
		}

		bool Construct(Runtime& runtime, Factory::Parameters params)
		{
			if (!params(0, &m_monoObj))
			{
				return false;
			}
			m_index = m_monoObj->GetProperty<int>("index");
			m_text = m_monoObj->GetProperty<std::string>("text");
			m_pathStringOnChoice = m_monoObj->GetProperty<std::string>("pathStringOnChoice");
			m_sourcePath = m_monoObj->GetProperty<std::string>("sourcePath");
			return true;
		}

		// Inherited via IChoice
		const std::string& GetText() override
		{
			return m_text;
		}
		const std::string& GetPathStringOnChoice() override
		{
			return m_pathStringOnChoice;
		}
		const std::string& GetSourcePath() override
		{
			return m_sourcePath;
		}
		int GetIndex() override
		{
			return m_index;
		}
		void SetIndex(int index) override
		{
			m_index = index;
		}
	};

	template <>
	ChoicePtr Factory::Create(Runtime& runtime, Factory::Parameters params)
	{
		auto choicePtr = new Choice();
		if (choicePtr->Construct(runtime, params))
		{
			return ChoicePtr(choicePtr);
		}
		return ChoicePtr();
	}
};

