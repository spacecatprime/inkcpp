#pragma once

#include "MonoTypes.h"

namespace Mono
{
	class String
		: public TypeContainer<MonoString>
	{
	public:
		explicit String(const char* string);
		explicit String(std::string string);
		~String();

		std::string ToString() const;
	};
}
