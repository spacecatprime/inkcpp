#pragma once

#include <string>
#include <functional>

#include <mono/jit/jit.h>

#include "MonoObject.h"

namespace Mono
{
	static std::string ToString(MonoObject* obj)
	{
		if (!obj)
		{
			return "<null>";
		}
		MonoObject* exc = nullptr;
		MonoString* monoStr = mono_object_to_string(obj, &exc);
		if (!monoStr || exc)
		{
			return "";
		}
		return mono_string_to_utf8(monoStr);
	}

	template <typename T>
	ObjectPtr BoxValue(T value)
	{
		std::static_assert("Need an override?");
	}

	template <>
	inline ObjectPtr BoxValue(int value)
	{
		MonoObject* obj = mono_value_box(mono_get_root_domain(), mono_get_int32_class(), &value);
		return ObjectPtr(new Object(obj));
	}

	template <>
	inline ObjectPtr BoxValue(bool value)
	{
		MonoObject* obj = mono_value_box(mono_get_root_domain(), mono_get_boolean_class(), &value);
		return ObjectPtr(new Object(obj));
	}

	template <>
	inline ObjectPtr BoxValue(float value)
	{
		double dValue = value;
		MonoObject* obj = mono_value_box(mono_get_root_domain(), mono_get_double_class(), &dValue);
		return ObjectPtr(new Object(obj));
	}

	template <>
	inline ObjectPtr BoxValue(double value)
	{
		MonoObject* obj = mono_value_box(mono_get_root_domain(), mono_get_double_class(), &value);
		return ObjectPtr(new Object(obj));
	}

	/*
		Entry place to store and retrieve a singleton pointer; 
		the application must hold the pointer through out the life cycle of the object
	*/
	template<class TClass>
	class Singleton
	{
	protected:
		inline static void Set(TClass* obj)
		{
			s_instance = obj;
		}

	public:
		inline static TClass* Get()
		{
			return s_instance;
		}

	private:
		static TClass* s_instance;
	};
}