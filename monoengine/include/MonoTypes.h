#pragma once

#include <mono/jit/jit.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <initializer_list>
#include <algorithm>

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
	class Runner;

	// -------------------------------------------------------
	using ImagePtr = std::shared_ptr<Image>;
	using DomainPtr = std::shared_ptr<Domain>;
	using ClassPtr = std::shared_ptr<Class>;
	using AssemblyPtr = std::shared_ptr<Assembly>;
	using ObjectPtr = std::shared_ptr<Object>;
	using StringPtr = std::shared_ptr<String>;
	using MethodPtr = std::shared_ptr<Method>;
	using RunnerPtr = std::shared_ptr<Runner>;

	// -------------------------------------------------------
	template <typename T>
	class TypeContainer
	{
	public:
		TypeContainer(T* inst)
			: m_typeInstance(inst)
		{
		}

		~TypeContainer() 
		{
			m_typeInstance = nullptr;
		}

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
		T* GetInstance() const
		{
			return m_typeInstance;
		}

		bool IsValid() const
		{
			if(this)
			{
				return m_typeInstance != nullptr;
			}
			return false;
		}

	protected:
		T* m_typeInstance;
	};
}
