#pragma once

#include <mono/jit/jit.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <initializer_list>

namespace Mono
{
	// -------------------------------------------------------
	class Image;
	class Domain;
	class Class;
	class Assembly;
	class Object;
	class String;
	class Method;

	// -------------------------------------------------------
	using ImagePtr = std::shared_ptr<Image>;
	using DomainPtr = std::shared_ptr<Domain>;
	using ClassPtr = std::shared_ptr<Class>;
	using AssemblyPtr = std::shared_ptr<Assembly>;
	using ObjectPtr = std::shared_ptr<Object>;
	using StringPtr = std::shared_ptr<String>;
	using MethodPtr = std::shared_ptr<Method>;

	// -------------------------------------------------------
	template <typename T>
	class TypeContainer
	{
	public:
		TypeContainer()
			: m_typeInstance(nullptr)
		{
		}

		~TypeContainer() = default;

		template <typename T>
		operator const T*() const
		{
			return m_typeInstance;
		}

		template <typename T>
		operator T* ()
		{
			return m_typeInstance;
		}

		template <typename T>
		void SetInstance(T* inst)
		{
			m_typeInstance = inst;
		}

		bool IsValid() const
		{
			return m_typeInstance != nullptr;
		}

	protected:
		T* m_typeInstance;
	};
}
