#pragma once

#include "MonoTypes.h"
#include "MonoArgs.h"
#include "MonoClass.h"
#include "MonoObject.h"

namespace Mono
{
	class Object
		: public TypeContainer<MonoObject>
	{
	public:
		explicit Object() : m_class() {}
		explicit Object(ClassPtr clazz);
		explicit Object(MonoObject* obj);

		MethodPtr GetMethod(const char* methodname);

		template <typename TReturn> 
		TReturn CallMethod(const char* methodname, Args args = Args());

		template <typename TReturn>
		TReturn GetProperty(const char* propertyname);

		template <typename TValue>
		void SetProperty(const char* propertyname, TValue value);

		template <typename TReturn, const int TCheck = MONO_TYPE_END>
		TReturn GetField(const char* fieldname); 

		template <typename TReturn, const int TCheck = MONO_TYPE_END>
		TReturn GetStaticField(const char* fieldname);

		template <typename TValue>
		bool SetField(const char* fieldname, TValue value);

		bool AsBool() const;
		std::uint32_t AsUint32() const;
		double AsDouble() const;

		ClassPtr GetClass() { return m_class; }

	protected:
		ObjectPtr CallMethodInternal(const char* methodname, Args args = Args());

	protected:
		ClassPtr m_class;
	};

	template <>	
	std::string Object::CallMethod(const char * methodname, Args args);

	template <>
	void Object::CallMethod(const char * methodname, Args args);

	template <>
	ObjectPtr Object::CallMethod(const char * methodname, Args args);

	template <typename TReturn>
	inline TReturn Object::CallMethod(const char * methodname, Args args)
	{
		ObjectPtr ret = CallMethodInternal(methodname, args);
		if (ret)
		{
			MonoObject* obj = *ret;
			return *(TReturn*)mono_object_unbox(obj);
		}
		// TODO: log?
		TReturn v = 0;
		return v;
	}

	template<typename TReturn, int TCheck>
	inline TReturn Object::GetStaticField(const char * fieldname)
	{
		TReturn value = 0;
		auto f = mono_class_get_field_from_name(*m_class, fieldname);
		if (!f)
		{
			return value;
		}
		if (TCheck != MONO_TYPE_END && mono_type_get_type(mono_field_get_type(f)) != TCheck)
		{
			return value;
		}
		mono_field_static_get_value(mono_class_vtable(mono_get_root_domain(), *m_class), f, &value);
		return value;
	}

	template<typename TReturn, const int TCheck>
	inline TReturn Object::GetField(const char * fieldname)
	{
		TReturn value = 0;
		auto f = mono_class_get_field_from_name(*m_class, fieldname);
		if (!f)
		{
			return value;
		}
		if (TCheck != MONO_TYPE_END && mono_type_get_type(mono_field_get_type(f)) != TCheck)
		{
			return value;
		}
		mono_field_get_value(m_typeInstance, f, &value);
		return value;
	}

	template<typename TValue>
	inline bool Object::SetField(const char * fieldname, TValue value)
	{
		auto f = mono_class_get_field_from_name(*m_class, fieldname);
		if (!f)
		{
			return false;
		}
		void* arg = reinterpret_cast< void* >(&value);
		mono_field_set_value(m_typeInstance, f, arg);
		return true;
	}

}
