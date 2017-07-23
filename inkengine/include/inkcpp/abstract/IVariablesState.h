#pragma once
// ********************************************************
// Ink interface of the Ink.Runtime.Object
// ********************************************************

#include "../Types.h"

namespace Ink
{
	/// <summary>
	/// Encompasses all the global variables in an ink Story, and
	/// allows binding of a VariableChanged event so that that game
	/// code can be notified whenever the global variables change.
	/// </summary>
	class IVariablesState
	{
	public:
		// Get or set the value of a named global ink variable.
		// The types available are the standard ink types. 
		// Certain types will be implicitly casted when setting.
		// For example, doubles to floats, longs to ints, and bools to ints.
		virtual void SetVariable(const std::string& variableName, ObjectPtr obj) = 0;
		virtual ObjectPtr GetVariable(const std::string& variableName) = 0;

		/// Enumerates iteration over all global variables by name.
		virtual std::vector<std::string> GetGlobalVariables() = 0;
	};
};

