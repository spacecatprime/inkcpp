#include "../include/MonoRunner.h"
#include "../include/MonoAssembly.h"
#include "../include/MonoImage.h"
#include "../include/MonoDomain.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

namespace Mono
{
	Runner::Runner()
		: m_mscorlib(new Image)
		, m_domain(new Domain)
		, m_assemblyMap()
	{
	}

	bool Runner::Setup(SetupDesc & desc)
	{
		mono_set_dirs(desc.m_monoLibFolder, desc.m_monoEtcConfigFolder);
		mono_set_assemblies_path(desc.m_assembliesPath);
		mono_config_parse(NULL);
		m_domain->SetInstance(mono_jit_init(desc.m_domainName));
		m_mscorlib->SetInstance(mono_image_loaded("mscorlib"));
		m_baseAssemblyPath = desc.m_assembliesPath;
		return m_domain->IsValid();
	}

	AssemblyPtr Runner::LoadAssembly(const char* assemblyPath)
	{
		// Open a assembly in the domain
		MonoAssembly* a = mono_domain_assembly_open(*m_domain, assemblyPath);
		if (!a)
		{
			MonoImageOpenStatus status;
			MonoAssemblyName* monoAssemblyName = mono_assembly_name_new(assemblyPath);
			a = mono_assembly_load(monoAssemblyName, m_baseAssemblyPath.c_str(), &status);
			mono_assembly_name_free(monoAssemblyName);
			if (!a || status != MONO_IMAGE_OK)
			{
				return AssemblyPtr();
			}
		}

		AssemblyPtr assembly(new Assembly);
		assembly->SetInstance(a);
		assembly->Init(m_domain);
		return assembly;
	}

	ImagePtr Runner::GetMscorlib()
	{
		return m_mscorlib;
	}
}