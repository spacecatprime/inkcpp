#pragma once

#include "MonoTypes.h"
#include "MonoArgs.h"

namespace Mono
{
	class Assembly
		: public TypeContainer<MonoAssembly>
	{
	public:
		using ClassMap = std::unordered_map<std::string, ClassPtr>;

		Assembly(MonoAssembly* asmb);

		ClassPtr LookupClass(const char* namepath, const char* classname);

		ObjectPtr CreateObject(const char* namepath, const char* classname);
		ObjectPtr CreateObject(const char* namepath, const char* classname, const char* ctorname, Args args);

		bool Init(DomainPtr domain);

		ImagePtr GetImage() const { return m_image; }
		const ClassMap GetClassMap() const { return m_classMap; }

	protected:
		ClassMap m_classMap;
		ImagePtr m_image;
		DomainPtr m_domain;
	};
}
