#pragma once

#include "MonoTypes.h"

namespace Mono
{
	class Class
		: public TypeContainer<MonoClass>
	{
	public:
		using MethodMap = std::unordered_map<std::string, MethodPtr>;

		void Reflect();

		inline const MethodMap& GetMethodMap()
		{
			return m_methodMap;
		}

	public:
		MethodMap m_methodMap;
	};
}
