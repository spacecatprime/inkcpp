#pragma once

#include "MonoTypes.h"
#include <memory>

namespace Mono
{
	class Domain
		: public std::enable_shared_from_this<Domain>
		, public TypeContainer<MonoDomain>
	{
	public:
		using TypeContainer::TypeContainer;
		using AssemblyMap = std::unordered_map<std::string, AssemblyPtr>;

		AssemblyPtr LoadAssembly(const std::string& assemblyPath, const std::string& basePathname);
		AssemblyPtr LoadFullAssembly(const std::string& etcFolder, const std::string& fullAssemblyName);
		AssemblyPtr FindAssembly(const std::string& assemblyLabel);
		void RegisterAssemby(const std::string& assemblyLabel, AssemblyPtr assembly);

	private:
		AssemblyMap m_assemblyMap;
	};
}
