#include "../include/MonoString.h"

namespace Mono
{
	String::String(const char * string)
		: TypeContainer(nullptr)
	{
		if (string)
		{
			m_typeInstance = mono_string_new_wrapper(string);
		}
		else
		{
			m_typeInstance = mono_string_empty(mono_domain_get());
		}
	}

	String::String(std::string string)
		: TypeContainer(nullptr)
	{
		if (string.empty())
		{
			m_typeInstance = mono_string_empty(mono_domain_get());
		}
		else
		{
			m_typeInstance = mono_string_new_wrapper(string.c_str());
		}
	}

	String::String(MonoString* monostring)
		: TypeContainer(monostring)
	{
	}

	String::~String()
	{
		m_typeInstance = nullptr;
	}

	std::string String::ToString() const
	{
		char* pRaw = mono_string_to_utf8(m_typeInstance);
		std::string outval(pRaw);
		mono_free(pRaw);
		return std::move(outval);
	}

}
