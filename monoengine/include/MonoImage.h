#pragma once

#include "MonoTypes.h"
#include "MonoArgs.h"

namespace Mono
{
	class Image
		: public TypeContainer<MonoImage>
	{
	public:
		using TypeContainer::TypeContainer;

		ObjectPtr CallStaticMethod(const char* signature, Args& args);
	};
}
