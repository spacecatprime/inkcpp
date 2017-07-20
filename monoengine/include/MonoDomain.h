#pragma once

#include "MonoTypes.h"

namespace Mono
{
	class Domain
		: public TypeContainer<MonoDomain>
	{
	public:
		Domain();
		~Domain() = default;
	};
}
