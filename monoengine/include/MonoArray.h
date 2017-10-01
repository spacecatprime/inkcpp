#pragma once

#include "MonoTypes.h"
#include "MonoArgs.h"

namespace Mono
{
	template <typename T>
	class Array
		: public TypeContainer<MonoArray>
	{
	public:
		using TypeContainer::TypeContainer;

		Array(std::uint32_t count);
		~Array() = default;

		void Add(int index, T element);

		std::uint32_t Count() const
		{
			return m_count;
		}

	protected:
		using StringList = std::vector<StringPtr>;

		std::uint32_t m_count;
		StringList m_stringList;
	};

	template <typename TPtr>
	class ArrayPointers
		: public TypeContainer<MonoArray>
	{
	public:
		using TypeContainer::TypeContainer;

		ArrayPointers(std::uint32_t count, MonoClass* elementType)
			: TypeContainer(nullptr)
			, m_count(count)
			, m_pointerList()
		{
			m_typeInstance = mono_array_new(mono_get_root_domain(), elementType, count);
		}

		~ArrayPointers() = default;

		void Add(int index, TPtr element)
		{
			m_pointerList.push_back(element);
		}

		std::uint32_t Count() const
		{
			return m_count;
		}

	protected:
		using PointerList = std::vector<TPtr>;

		std::uint32_t m_count;
		PointerList m_pointerList;
	};

	template<>
	inline Array<Mono::ObjectPtr>::Array(std::uint32_t count)
		: TypeContainer(nullptr)
		, m_count(count)
	{
		m_typeInstance = mono_array_new(mono_get_root_domain(), mono_get_object_class(), count);
	}

	template<>
	inline void Array<Mono::ObjectPtr>::Add(int index, Mono::ObjectPtr element)
	{
		MonoArray* arry = m_typeInstance;
		MonoObject* objValue = *element;
		mono_array_set(arry, MonoObject*, index, *element);
	}

	template<>
	inline Array<MonoString*>::Array(std::uint32_t count)
		: TypeContainer(nullptr)
		, m_count(count)
	{
		m_typeInstance = mono_array_new(mono_get_root_domain(), mono_get_string_class(), count);
	}

	template<>
	inline void Array<MonoString*>::Add(int index, MonoString* element)
	{
		mono_array_set(m_typeInstance, MonoString*, index, element);
	}
}
