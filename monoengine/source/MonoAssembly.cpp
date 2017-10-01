#include "../include/MonoAssembly.h"
#include "../include/MonoImage.h"
#include "../include/MonoClass.h"
#include "../include/MonoObject.h"
#include "../include/MonoArgs.h"
#include "../include/MonoDomain.h"
#include "../include/MonoLogger.h"
#include "../include/MonoMethod.h"

#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Mono
{
	Assembly::Assembly(MonoAssembly* asmb)
		: TypeContainer(asmb)
		, m_classMap()
		, m_image()
		, m_domain()
	{
	}

	ClassPtr Assembly::LookupClass(const char* namepath, const char* classname)
	{
		if (!namepath || !classname)
		{
			Mono::GetLogger()->Error("both namepath and classname required; empty namepath okay");
			return ClassPtr();
		}

		// determine namespace+class hash
		std::string np(namepath);
		std::string cn(classname);
		std::string hash;
		if (!np.empty() && !cn.empty())
		{
			hash = np + "." + cn;
		}		
		else if (!cn.empty())
		{
			hash = cn;
		}
		else
		{
			Mono::GetLogger()->Error("classname has to be non-empty");
			return ClassPtr();
		}

		// check on class cache
		auto it = m_classMap.find(hash);
		if (it != m_classMap.end())
		{
			return it->second;
		}

		MonoClass* klass = mono_class_from_name(*m_image, namepath, classname);
		if (!klass)
		{
			return ClassPtr();
		}
		auto outClass = ClassPtr(new Class(klass));
		outClass->Reflect();
		m_classMap.emplace(hash, outClass);
		return outClass;
	}

	ObjectPtr Assembly::CreateObject(const char* namepath, const char* classname)
	{
		auto classPtr = LookupClass(namepath, classname);
		if (!classPtr)
		{
			return ObjectPtr();
		}

		// Create a instance of the class
		MonoObject* inst = mono_object_new(*m_domain, *classPtr);
		if (!inst)
		{
			return ObjectPtr();
		}

		mono_runtime_object_init(inst);

		auto out = ObjectPtr(new Object(inst, classPtr));
		out->GetClass()->Reflect();
		return out;
	}

	ObjectPtr Assembly::CreateObject(const char* namepath, const char* classname, const char* ctorname, Args args)
	{
		// @namepath eg 'Ink'
		// @ctorname eg 'InkParser'
		// @ctorname eg 'Ink.InkParser:.ctor(string,string,Ink.ErrorHandler)'

		// Get the class
		auto classPtr = LookupClass(namepath, classname);
		if (!classPtr)
		{
			return ObjectPtr();
		}

		// Create a instance of the class
		MonoObject* inst = mono_object_new(*m_domain, *classPtr);
		if (!inst)
		{
			return ObjectPtr();
		}

		// no construction parameters?
		if (args.IsEmpty())
		{
			mono_runtime_object_init(inst);
		}
		else
		{
			// Get the constructor of the class.
			auto ctorDesc = Method::CreateDesc(ctorname, true);
			if (!ctorDesc->IsValid())
			{
				return ObjectPtr();
			}

			MonoMethod* pConstructorMethod = mono_method_desc_search_in_class(*ctorDesc, *classPtr);
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

		ObjectPtr obj = ObjectPtr(new Object(inst, classPtr));
		obj->GetClass()->Reflect();
		return obj;
	}

	bool Assembly::Init(DomainPtr domain)
	{
		if (m_typeInstance == nullptr)
		{
			return false;
		}

		m_domain = domain;

		// Get a image from the assembly
		MonoImage* img = mono_assembly_get_image(m_typeInstance);
		if (!img)
		{
			return false;
		}
		m_image.reset(new Image(img));
		return true;
	}
}
