#pragma once
// ********************************************************
// TBD
// ********************************************************

#include "InkRuntime.h"
#include <memory>

namespace Ink
{
    /**
      * TBD
      */      
	class Factory
	{
	public:
		template <typename TRet>
		std::shared_ptr<TRet> Create(Runtime& runtime);
	};
};

