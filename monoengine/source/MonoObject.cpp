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

#include <algorithm>

namespace Mono
{
	MonoObject* Object_RuntimeInvoke(MonoMethod* method, void *obj, void **params)
	{
		MonoObject* exp = nullptr;
		MonoObject* ret = mono_runtime_invoke(method, obj, params, &exp);
		if (exp)
		{
			Mono::GetLogger()->Exception(exp);
			return nullptr;
		}
		return ret;
	}

	Object::Object(ClassPtr clazz)
		: m_class(clazz)
	{
	}

	Object::Object(MonoObject* obj)
	{
		if (obj)
		{
			m_class = ClassPtr(new Class());
			m_class->SetInstance(mono_object_get_class(obj));
			m_class->Reflect();
			SetInstance(obj);
		}
	}

	MethodPtr Object::GetMethod(const char * methodname)
	{
		throw new std::exception();
		return MethodPtr();
	}

	template <>
	void Object::CallMethod(const char * methodname, Args args)
	{
		CallMethodInternal(methodname, args);
	}

	template <>
	std::string Object::CallMethod(const char * methodname, Args args)
	{
		auto ret = CallMethodInternal(methodname, args);
		if (ret)
		{
			MonoObject* retStr = *ret;
			char* pRaw = mono_string_to_utf8(reinterpret_cast<MonoString*>(retStr));
			std::string outval(pRaw);
			mono_free(pRaw);
			return std::move(outval);
		}
		// TODO: log?
		return std::string();
	}

	template <>
	ObjectPtr Object::CallMethod(const char * methodname, Args args)
	{
		ObjectPtr ret = CallMethodInternal(methodname, args);
		if (ret)
		{
			ret->GetClass()->Reflect();
			return ret;
		}
		return ObjectPtr();
	}

	bool Object::AsBool() const
	{
		return *(bool*)mono_object_unbox(m_typeInstance);
	}

	std::uint32_t Object::AsUint32() const
	{
		return *(std::uint32_t*)mono_object_unbox(m_typeInstance);
	}

	double Object::AsDouble() const
	{
		return *(double*)mono_object_unbox(m_typeInstance);
	}

	ObjectPtr Object::CallMethodInternal(const char* methodname, Args args)
	{
		const auto& map = m_class->GetMethodMap();
		auto m = std::find_if(std::begin(map), std::end(map), [methodname](auto kvp) 
		{
			return kvp.first.find(methodname) != kvp.first.npos;
		});
		if (m != map.end())
		{
			void** params = nullptr;
			if (!args.IsEmpty())
			{
				params = args;
			}
			MonoObject* ret = Object_RuntimeInvoke(*m->second, m_typeInstance, params);
			if (ret)
			{
				auto clsptr = ClassPtr(new Class());
				clsptr->SetInstance(mono_object_get_class(ret));
				clsptr->Reflect();

				ObjectPtr out = ObjectPtr(new Object(clsptr));
				out->SetInstance(ret);
				return out;
			}
		}
		return ObjectPtr();
	}

	///////////////////////////////////////////////////////
	#pragma region property getters

	static MonoObject* GetPropertyInternal(const char* propertyname, Mono::Class* clazz, MonoObject* obj)
	{
		if (clazz == nullptr)
		{
			Mono::GetLogger()->Warning(std::string("No class for prop: ") + propertyname);
			return nullptr;
		}
		const std::string propgetter("get_" + std::string(propertyname));
		const auto& map = clazz->GetMethodMap();
		for (auto m : map)
		{
			if (m.first.find(propgetter) != m.first.npos)
			{
				return Object_RuntimeInvoke(*m.second, obj, nullptr);
			}
		}
		return nullptr;
	}

	template<>
	int Object::GetProperty(const char* propertyname)
	{
		MonoObject* ret = GetPropertyInternal(propertyname, m_class.get(), m_typeInstance);
		if (ret)
		{
			return *(int*)mono_object_unbox(ret);
		}
		return 0;
	}

	template<>
	ObjectPtr Object::GetProperty(const char* propertyname)
	{
		MonoObject* ret = GetPropertyInternal(propertyname, m_class.get(), m_typeInstance);
		if (ret)
		{
			auto clsptr = ClassPtr(new Class());
			clsptr->SetInstance(mono_object_get_class(ret));
			clsptr->Reflect();

			ObjectPtr out = ObjectPtr(new Object(clsptr));
			out->SetInstance(ret);
			return out;
		}
		return ObjectPtr();
	}

	template<>
	bool Object::GetProperty(const char* propertyname)
	{
		const std::string propgetter("get_" + std::string(propertyname));
		const auto& map = m_class->GetMethodMap();
		for (auto m : map)
		{
			if (m.first.find(propgetter) != m.first.npos)
			{
				MonoObject* ret = Object_RuntimeInvoke(*m.second, m_typeInstance, nullptr);
				if (ret)
				{
					// TODO: assert for bool type?
					return *(bool*)mono_object_unbox(ret);
				}
			}
		}
		return false;
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
				MonoObject* ret = Object_RuntimeInvoke(*m.second, m_typeInstance, nullptr);
				if (ret)
				{
					char* pRaw = mono_string_to_utf8(reinterpret_cast<MonoString*>(ret));
					std::string outval(pRaw);
					mono_free(pRaw);
					return std::move(outval);
				}
			}
		}
		return std::string();
	}
#pragma endregion

	///////////////////////////////////////////////////////
	#pragma region property setters

	template<>
	void Object::SetProperty(const char * propertyname, int value)
	{
		throw new std::exception();
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
				Object_RuntimeInvoke(*m.second, m_typeInstance, args);
			}
		}
	}

	template<>
	void Object::SetProperty(const char * propertyname, float value)
	{
		throw new std::exception();
	}
#pragma endregion

	///////////////////////////////////////////////////////
	#pragma region field getters

	// TODO string version of this, maybe others?

	//template<>
	//int Object::GetField(const char * fieldname)
	//{
	//	return 0;
	//}

#pragma endregion

	///////////////////////////////////////////////////////
	#pragma region field setters 

	//template<>
	//bool Object::SetField(const char * fieldname, int value)
	//{
	//	return false;
	//}
#pragma endregion
}
