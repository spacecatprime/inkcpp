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
		explicit Object(MonoObject* obj);

		MethodPtr GetMethod(const char* methodname);

		template <typename TReturn> 
		TReturn CallMethod(const char* methodname, Args args = Args());

		template <typename TReturn>
		TReturn GetProperty(const char* propertyname);

		template <typename TValue>
		void SetProperty(const char* propertyname, TValue value);

		bool AsBool() const;
		std::uint32_t AsUint32() const;
		double AsDouble() const;

		ClassPtr GetClass() { return m_class; }

	protected:
		ObjectPtr CallMethodInternal(const char* methodname, Args args = Args());

	protected:
		ClassPtr m_class;
	};
}
