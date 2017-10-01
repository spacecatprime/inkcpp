#include "../include/MonoMethod.h"
#include "../include/MonoTypes.h"

#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Mono
{
	Method::DescPtr Method::CreateDesc(const char * name, bool bIncludeNamespace)
	{
		auto pDesc = mono_method_desc_new(name, bIncludeNamespace);
		if (!pDesc)
		{
			return Method::DescPtr();
		}
		return Method::DescPtr(new Desc(pDesc));
	}

	Method::Desc::~Desc()
	{
		mono_method_desc_free(m_typeInstance);
	}
}
