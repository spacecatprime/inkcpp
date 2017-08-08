#pragma once
// ********************************************************
// TBD
// ********************************************************

#include "InkRuntime.h"
#include <memory>
#include <functional>

namespace Ink
{
    /**
      * Creates a run time instance based on a Ink::* Type
      */      
	class Factory
	{
	public:
		// gets the parameter for construction
		// @int is the index of the parameter
		// @void* the value to be filled in
		// returns TRUE if the index is valid
		using Parameters = std::function<bool(int,void*)>;

		template <typename TRet>
		std::shared_ptr<TRet> Create(Runtime& runtime, Parameters params);
	};
};

