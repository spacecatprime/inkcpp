#pragma once

#include "MonoTypes.h"
#include "MonoArgs.h"

namespace Mono
{
	class Object
		: public TypeContainer<MonoObject>
	{
	public:
		explicit Object() : m_class() {}
		explicit Object(ClassPtr clazz);

		MethodPtr GetMethod(const char* methodname);

		ObjectPtr CallMethod(const char* methodname, Args args = Args());

		template <typename TReturn>
		TReturn GetProperty(const char* propertyname);

		template <typename TValue>
		void SetProperty(const char* propertyname, TValue value);

	protected:
		ClassPtr m_class;
	};
}
