#pragma once

#include <string>
#include <functional>

#include <mono/jit/jit.h>

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