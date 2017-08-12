#pragma once

#include "MonoTypes.h"
#include "MonoString.h"

namespace Mono
{
	class Args
	{
	public:
		operator void** ()
		{
			if (m_args.empty())
			{
				return nullptr;
			}
			return &m_args.front();
		}

		bool IsEmpty() const
		{
			return m_args.empty();
		}

		template<typename T>
		void Add(T arg)
		{
			m_args.push_back(arg);
		}

		template<> void Add(const char* arg);
		template<> void Add(std::string arg);

		Args()
			: m_args()
			, m_strings()
		{
		}

		Args(std::initializer_list<void*> values)
			: m_args()
			, m_strings()
		{
			std::move(values.begin(), values.end(), m_args.begin());
		}

		Args(std::initializer_list<const char*> values)
			: m_args()
			, m_strings()
		{
			for (auto v : values)
			{
				Add(v);
			}
		}

	private:
		std::vector<void*> m_args;
		std::vector<StringPtr> m_strings;
	};

	template<>
	inline void Args::Add(std::string arg)
	{
		StringPtr str(new String(arg));
		MonoString* raw = *str;
		m_args.push_back(reinterpret_cast<void*>(raw));
		m_strings.push_back(str);
	}

	template<>
	inline void Args::Add(const char* arg)
	{
		StringPtr str(new String(arg));
		MonoString* raw = *str;
		m_args.push_back(reinterpret_cast<void*>(raw));
		m_strings.push_back(str);
	}
}
