#pragma once

#include <string>
#include <functional>

#include <mono/jit/jit.h>

#include "MonoObject.h"

namespace Mono
{
	template <typename ToType, typename FromType>
	using ConvertTo = std::function<ToType(FromType)>;

	template <typename T>
	struct Convert
	{
		//static auto To(ConvertTo func, T &&t) -> T
		//{
		//	return std::forward<T>(t);
		//}

		static auto From(T &&t) -> T
		{
			return std::forward<T>(t);
		}
	};

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

	//template <>
	//struct Convert<std::string>
	//{
	//	using mono_type_name = MonoString *;
	//	static auto to_mono(mono_assembly &assembly, const std::string &str) -> MonoString *
	//	{
	//		return assembly.new_string(str).get_mono_string();
	//	}
	//	static auto from_mono(MonoString *mono_str) -> std::string
	//	{
	//		return mono_string(mono_str).str();
	//	}
	//};
}