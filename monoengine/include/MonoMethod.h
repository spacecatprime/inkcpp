#pragma once

#include "MonoTypes.h"

typedef struct MonoMethodDesc MonoMethodDesc;

namespace Mono
{
	class Method
		: public TypeContainer<MonoMethod>
	{
	public:
		using TypeContainer::TypeContainer;

		class Desc 
			: public TypeContainer<MonoMethodDesc>
		{
		public:
			using TypeContainer::TypeContainer;

			~Desc();
		};
		using DescPtr = std::shared_ptr<Desc>;

		static DescPtr CreateDesc(const char* name, bool bIncludeNamespace);
	};
}
