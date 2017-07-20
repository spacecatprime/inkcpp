#include "../include/MonoClass.h"
#include "../include/MonoMethod.h"

#include <mono/metadata/debug-helpers.h>

namespace Mono
{
	void Class::Reflect()
	{
		void* ptr = nullptr;
		MonoMethod* pMethod = mono_class_get_methods(m_typeInstance, &ptr);
		while (ptr != nullptr && pMethod != nullptr)
		{
			MethodPtr ptrMethod = MethodPtr(new Method);
			ptrMethod->SetInstance(pMethod);

			const char* name = mono_method_get_name(pMethod);
			const char* fullname = mono_method_full_name(pMethod, true);
			m_methodMap.emplace(fullname, ptrMethod);
			pMethod = mono_class_get_methods(m_typeInstance, &ptr);
		}
	}
}
