#include "../include/MonoObject.h"
#include "../include/MonoRunner.h"
#include "../include/MonoAssembly.h"
#include "../include/MonoImage.h"
#include "../include/MonoClass.h"
#include "../include/MonoArgs.h"
#include "../include/MonoDomain.h"
#include "../include/MonoLogger.h"
#include "../include/MonoMethod.h"

#include <mono/metadata/object.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/mono-config.h>

namespace Mono
{

	Object::Object(ClassPtr clazz)
		: m_class(clazz)
	{
	}

	MethodPtr Object::GetMethod(const char * methodname)
	{
		return MethodPtr();
	}

	ObjectPtr Object::CallMethod(const char * methodname, Args args)
	{
		return ObjectPtr();
	}

	// getters

	template<>
	int Object::GetProperty(const char* propertyname)
	{
		return 0;
	}

	template<>
	std::string Object::GetProperty(const char* propertyname)
	{
		const std::string propgetter("get_" + std::string(propertyname));
		const auto& map = m_class->GetMethodMap();
		for (auto m : map)
		{
			if (m.first.find(propgetter) != m.first.npos)
			{
				MonoObject* ret = mono_runtime_invoke(*m.second, m_typeInstance, nullptr, nullptr); // TODO exception
				if (ret)
				{
					char* pRaw = mono_string_to_utf8(reinterpret_cast<MonoString*>(ret));
					std::string outval(pRaw);
					mono_free(pRaw);
					return std::move(outval);
				}
			}
		}
		return{};
	}

	// setters

	template<>
	void Object::SetProperty(const char * propertyname, int value)
	{
	}

	template<>
	void Object::SetProperty(const char * propertyname, std::string value)
	{
		const std::string propgetter("set_" + std::string(propertyname));
		const auto& map = m_class->GetMethodMap();
		for (auto m : map)
		{
			if (m.first.find(propgetter) != m.first.npos)
			{
				Mono::Args args;
				args.Add(value);
				mono_runtime_invoke(*m.second, m_typeInstance, args, nullptr); // TODO exception
			}
		}
	}

	template<>
	void Object::SetProperty(const char * propertyname, float value)
	{
	}
}
