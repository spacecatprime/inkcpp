#include "../include/MonoClass.h"
#include "../include/MonoMethod.h"
#include "../include/MonoObject.h"
#include "../include/MonoLogger.h"
#include "../include/MonoDomain.h"

#include <mono/metadata/debug-helpers.h>

namespace Mono
{
	Class::MethodMap GetClassMethods(MonoClass* klass)
	{
		Class::MethodMap methodMap;
		void* ptr = nullptr;
		MonoMethod* pMethod = mono_class_get_methods(klass, &ptr);
		while (ptr != nullptr && pMethod != nullptr)
		{
			MethodPtr ptrMethod = MethodPtr(new Method(pMethod));
			const char* name = mono_method_get_name(pMethod);
			const char* fullname = mono_method_full_name(pMethod, true);
			methodMap.emplace(fullname, ptrMethod);
			pMethod = mono_class_get_methods(klass, &ptr);
		}

		return methodMap;
	}

	void Class::Reflect()
	{
		if (!m_methodMap.empty())
		{
			return;
		}

		m_methodMap = GetClassMethods(m_typeInstance);

		// fetch all interface class methods inherited
		void* iter = nullptr;
		MonoClass* klass = mono_class_get_interfaces(m_typeInstance, &iter);
		while (iter != nullptr && klass)
		{
			auto methods = GetClassMethods(klass);
			for (auto kvp : methods)
			{
				m_methodMap.insert(kvp);
			}
			klass = mono_class_get_interfaces(m_typeInstance, &iter);
		}
	}


	bool Class::LoadVirtualMethod(const std::string& methodname, int paramCount, ObjectPtr obj)
	{
		auto ifaceMethod = mono_class_get_method_from_name(m_typeInstance, methodname.c_str(), paramCount);
		if (nullptr == ifaceMethod)
		{
			return false;
		}
		auto ifaceImplMethod = mono_object_get_virtual_method(*obj, ifaceMethod);
		if (nullptr == ifaceImplMethod)
		{
			return false;
		}
		return false;
	}

	MethodPtr Class::RegisterVirtualMethod(const std::string& aNamespace, const std::string& aBaseInterfaceClass, const std::string& virtualMethod, int methodParameterCount)
	{
		// https://stackoverflow.com/questions/26253074/how-to-invoke-an-interface-method-with-mono-embedding

		// TODO more checking!
		auto image = mono_class_get_image(m_typeInstance);
		auto interfaceClass = mono_class_from_name(image, aNamespace.c_str(), aBaseInterfaceClass.c_str());
		auto ifaceMethod = mono_class_get_method_from_name(interfaceClass, virtualMethod.c_str(), methodParameterCount);
		return MethodPtr(new Method(ifaceMethod));
	}

	bool Class::HasInterface(ClassPtr baseClazz) const
	{
		if (IsValid() && baseClazz && baseClazz->IsValid())
		{
			return mono_class_implements_interface(m_typeInstance, *baseClazz) != 0;
		}
		return false;
	}

	ObjectPtr Class::CreateInstance(DomainPtr domain, const char* ctorname, Args args)
	{
		// Create a instance of the class
		MonoObject* inst = mono_object_new(*domain, m_typeInstance);
		if (!inst)
		{
			return ObjectPtr();
		}

		// no construction parameters?
		if (args.IsEmpty())
		{
			mono_runtime_object_init(inst);
		}
		else if(ctorname)
		{
			// Get the constructor of the class.
			auto ctorDesc = Method::CreateDesc(ctorname, true);
			if (!ctorDesc->IsValid())
			{
				return ObjectPtr();
			}

			MonoMethod* pConstructorMethod = mono_method_desc_search_in_class(*ctorDesc, m_typeInstance);
			if (!pConstructorMethod)
			{
				return ObjectPtr();
			}

			// Invoke the constructor.
			MonoObject *pException = nullptr;
			mono_runtime_invoke(pConstructorMethod, inst, &args[0], &pException);
			if (pException)
			{
				Mono::GetLogger()->Exception(pException);
				return ObjectPtr();
			}
		}
		else
		{
			// TODO fail msg
			return ObjectPtr();
		}
		Reflect();
		return ObjectPtr(new Object(inst, shared_from_this()));
	}
}

