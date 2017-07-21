#pragma once

// TODO rename this module and separate all the code parts

// ********************************************************
// Ink interface classes
// ********************************************************

#include "Types.h"

namespace Ink
{
    class IObject;

	// Base class for all ink runtime content.
	class IObject
	{
	public:
		/// Runtime.Objects can be included in the main Story as a hierarchy. Usually parents are Container objects.
		virtual IObject* GetParent() const = 0;
		virtual void SetParent(IObject* parent) = 0;

		virtual bool Equals(IObject* obj) const = 0;
	};

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
		virtual void SetVariable(const std::string& variableName, IObject* obj) = 0;
		virtual IObject* GetVariable(const std::string& variableName) = 0;

		/// Enumerates iteration over all global variables by name.
		virtual std::vector<std::string> GetGlobalVariables() = 0;
	};
};

