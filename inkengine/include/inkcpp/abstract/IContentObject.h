#pragma once
// ********************************************************
// TBD
// ********************************************************

#include "../Types.h"

namespace Ink
{
	// Base class for all ink runtime content.
	class IContentObject
	{
	public:
		/// Runtime.Objects can be included in the main Story as a hierarchy. Usually parents are Container objects.
		virtual IContentObject* GetParent() const = 0;
		virtual void SetParent(IContentObject* parent) = 0;
		virtual bool Equals(IContentObject* obj) const = 0;
	};

	// TODO needed?
};

