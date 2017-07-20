#pragma once

#include "MonoTypes.h"
#include "MonoArgs.h"

namespace Mono
{
	class Assembly
		: public TypeContainer<MonoAssembly>
	{
	public:
		Assembly();
		~Assembly() = default;

		ClassPtr LookupClass(const char* namepath, const char* classname);

		ObjectPtr CreateObject(const char* namepath, const char* classname);
		ObjectPtr CreateObject(const char* namepath, const char* classname, const char* ctorname, Args args);

		bool Init(DomainPtr domain);

	protected:
		using ClassMap = std::unordered_map<std::string, ClassPtr>;

		ImagePtr m_image;
		DomainPtr m_domain;
		ClassMap m_classMap;
	};
}
