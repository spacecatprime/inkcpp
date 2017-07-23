#pragma once
// ********************************************************
// TBD
// ********************************************************

#include "../Types.h"

namespace Ink
{
	/// Exception that represents an error when running a Story at runtime.
	/// An exception being thrown of this type is typically when there's
	/// a bug in your ink, rather than in the ink engine itself!
	class IStoryException // is_a System.Exception
	{
		virtual const std::string& GetMessage() = 0;
	};
};