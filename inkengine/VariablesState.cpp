/**
	Encompasses all the global variables in an ink Story, and
	allows binding of a VariableChanged event so that that game
	code can be notified whenever the global variables change.
*/
#include "include\inkcpp\abstract\IVariablesState.h"
#include "include\inkcpp\InkFactory.h"
#include "include\inkcpp\Types.h"

#include <MonoObject.h>

namespace Ink
{
	struct VariablesState : public IVariablesState
	{
		Mono::ObjectPtr m_self;

		VariablesState()
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

		// Inherited via IVariablesState
		void SetVariable(const std::string & variableName, Mono::ObjectPtr obj) override
		{
			Mono::Args args;
			args.Add(variableName);
			args.Add(*obj);
			m_self->CallMethod<Mono::ObjectPtr>("set_Item", args);
		}
		Mono::ObjectPtr GetVariable(const std::string& variableName) override
		{
			Mono::Args args;
			args.Add(variableName);
			return m_self->CallMethod<Mono::ObjectPtr>("get_Item", args);
		}
		std::vector<std::string> GetGlobalVariables() override
		{
			// public IEnumerator<string> GetEnumerator()
			return std::vector<std::string>();
		}
	};

	template <>
	VariablesStatePtr Factory::Create(Runtime& runtime, Factory::Parameters params)
	{
		auto ptr = new VariablesState();
		if (ptr->Construct(runtime, params))
		{
			return VariablesStatePtr(ptr);
		}
		return VariablesStatePtr();
	}
};

