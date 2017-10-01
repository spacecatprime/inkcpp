#include "../include/MonoDomain.h"
#include "../include/MonoAssembly.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

namespace Mono
{
	AssemblyPtr Domain::LoadAssembly(const std::string& assemblyPath, const std::string& basePathname)
	{
		auto cached = FindAssembly(assemblyPath);
		if (cached)
		{
			return cached;
		}

		// Open a assembly in the domain
		MonoAssembly* a = mono_domain_assembly_open(m_typeInstance, assemblyPath.c_str());
		if (!a)
		{
			MonoImageOpenStatus status;
			MonoAssemblyName* monoAssemblyName = mono_assembly_name_new(assemblyPath.c_str());
			a = mono_assembly_load(monoAssemblyName, basePathname.c_str(), &status);
			mono_assembly_name_free(monoAssemblyName);
			if (!a || status != MONO_IMAGE_OK)
			{
				return AssemblyPtr();
			}
		}

		AssemblyPtr assembly(new Assembly(a));
		assembly->Init(shared_from_this());
		RegisterAssemby(assemblyPath, assembly);
		return assembly;
	}

	AssemblyPtr Domain::LoadFullAssembly(const std::string& etcFolder, const std::string& fullAssemblyName)
	{
		AssemblyPtr cached = FindAssembly(fullAssemblyName);
		if (cached)
		{
			return cached;
		}

		mono_domain_set_config(m_typeInstance, (etcFolder + "/mono/4.5/").c_str(), "machine.config");

		MonoImageOpenStatus status = MONO_IMAGE_IMAGE_INVALID;
		auto systemAssemblyName = mono_assembly_name_new(fullAssemblyName.c_str());
		auto systemAssembly = mono_assembly_load_full(systemAssemblyName, nullptr, &status, false);
		mono_assembly_name_free(systemAssemblyName);

		if (nullptr == systemAssembly || status != MONO_IMAGE_OK)
		{
			return{};
		}

		AssemblyPtr assembly = AssemblyPtr(new Assembly(systemAssembly));
		assembly->Init(shared_from_this());
		RegisterAssemby(fullAssemblyName, assembly);
		return assembly;
	}

	AssemblyPtr Domain::FindAssembly(const std::string & assemblyLabel)
	{
		auto it = m_assemblyMap.find(assemblyLabel);
		if (it != m_assemblyMap.end())
		{
			return it->second;
		}
		return AssemblyPtr();
	}

	void Domain::RegisterAssemby(const std::string& assemblyLabel, AssemblyPtr assembly)
	{
		auto ret = m_assemblyMap.insert({ assemblyLabel, assembly });
		if (ret.second == false)
		{
			// log it?
		}
	}
}

