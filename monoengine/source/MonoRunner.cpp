#include "../include/MonoRunner.h"
#include "../include/MonoAssembly.h"
#include "../include/MonoImage.h"
#include "../include/MonoDomain.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

namespace Mono
{
	// Static
	static int s_numRunners = 0;

	template <>
	Runner* Singleton<Runner>::s_instance = nullptr;

	Runner::Runner()
		: m_mscorlib()
		, m_domain()
	{
		if (s_numRunners == 0)
		{
			// Load the default Mono configuration file, this is needed
			// if you are planning on using the dllmaps defined on the system configuration
			mono_config_parse(nullptr);
			s_numRunners++;
			Set(this);
		}
	}

	Runner::~Runner()
	{
		// mono_jit_cleanup seems like it breaks things
		//if (m_domain->IsValid())
		//{
		//	mono_jit_cleanup(*m_domain);
		//	m_domain.reset();
		//}
		--s_numRunners;
		if (s_numRunners == 0)
		{
			Set(nullptr);
		}
	}

	bool Runner::Setup(SetupDesc& desc)
	{
		mono_set_dirs(desc.m_monoLibFolder.c_str(), desc.m_monoEtcConfigFolder.c_str());
		mono_set_assemblies_path(desc.m_assembliesPath.c_str());

		MonoDomain* dom = nullptr;
		dom = mono_get_root_domain();
		if (!dom)
		{
			dom = mono_jit_init(desc.m_domainName.c_str());
			if (!dom)
			{
				return false;
			}
		}

		m_domain.reset(new Domain(dom));
		m_mscorlib.reset(new Image(mono_image_loaded("mscorlib")));
		m_systemAssembly = m_domain->LoadFullAssembly(desc.m_monoEtcConfigFolder, "System, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089");
		m_baseAssemblyPath = desc.m_assembliesPath;
		return m_domain->IsValid();
	}
}

