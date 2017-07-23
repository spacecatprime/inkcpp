#pragma once

// TODO rename this module and separate all the code parts

// ********************************************************
// Ink interface classes
// ********************************************************

#include "Types.h"

namespace Ink
{
	// Base class for all ink runtime content.
	class IObject
	{
	public:
		/// Runtime.Objects can be included in the main Story as a hierarchy. Usually parents are Container objects.
		virtual IObject* GetParent() const = 0;
		virtual void SetParent(IObject* parent) = 0;

		virtual bool Equals(IObject* obj) const = 0;
	};
};

