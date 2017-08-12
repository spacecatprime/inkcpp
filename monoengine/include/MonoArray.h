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
		Array(std::uint32_t count);
		~Array() = default;

		void Add(int index, T element);

	protected:
		std::uint32_t m_count;
	};

	template<>
	inline Array<Mono::ObjectPtr>::Array(std::uint32_t count)
		: m_count(count)
	{
		MonoClass* eclass = mono_get_object_class();
		m_typeInstance = mono_array_new(mono_get_root_domain(), mono_get_object_class(), count);
	}

	template<>
	inline void Array<Mono::ObjectPtr>::Add(int index, Mono::ObjectPtr element)
	{/*
	 MonoType*
	 mono_class_get_type          (MonoClass *klass)

		MonoArray* theArray = mono_array_new(domain, containsClass, 1);
		mono_array_set(theArray, MonoObject*, 0, containsObject);
		args[0] = theArray;
		mono_runtime_invoke(returnElementMethod, NULL, args, &exception);*/
		//mono_array_set(m_typeInstance, mono_get_object_class(), index, value);
		MonoArray* arry = m_typeInstance;
		MonoObject* objValue = *element;
		mono_array_set(arry, MonoObject*, index, *element);
	}
}
