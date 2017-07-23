#pragma once

#include "MonoTypes.h"

namespace Mono
{
	class Runner
	{
	public:
		struct SetupDesc
		{
			std::string m_monoLibFolder;
			std::string m_monoEtcConfigFolder;
			std::string m_assembliesPath;
			std::string m_domainName;
		};
		using AssemblyMap = std::unordered_map<std::string, AssemblyPtr>;

		Runner();
		~Runner();

		bool Setup(SetupDesc& desc);

		AssemblyPtr LoadAssembly(const char* assemblyPath);

		ImagePtr GetMscorlib();

	protected:

		ImagePtr m_mscorlib;
		DomainPtr m_domain;
		AssemblyMap m_assemblyMap;
		std::string m_baseAssemblyPath;
	};
}
