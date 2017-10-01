#pragma once

#include "MonoTypes.h"
#include "MonoArgs.h"

namespace Mono
{
	class Class
		: public std::enable_shared_from_this<Class>
		, public TypeContainer<MonoClass>
	{
	public:
		using TypeContainer::TypeContainer;
		using MethodMap = std::unordered_map<std::string, MethodPtr>;

		void Reflect();

		inline const MethodMap& GetMethodMap()
		{
			return m_methodMap;
		}

		inline static MethodPtr FindMethod(const MethodMap& methodMap, const std::string& methodname)
		{
			auto m = std::find_if(std::begin(methodMap), std::end(methodMap), [methodname](auto kvp)
			{
				return kvp.first.find(methodname) != kvp.first.npos;
			});
			if (m != methodMap.end())
			{
				return m->second;
			}
			return MethodPtr();
		}

		bool LoadVirtualMethod(const std::string& methodname, int paramCount, ObjectPtr obj);

		MethodPtr RegisterVirtualMethod(const std::string& aNamespace, const std::string& aBaseInterfaceClass, const std::string& virtualMethod, int methodParameterCount);

		bool HasInterface(ClassPtr baseClazz) const;

		ObjectPtr CreateInstance(DomainPtr domain, const char* ctorname, Args args);

	public:
		MethodMap m_methodMap;
	};
}
