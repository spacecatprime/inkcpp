#pragma once

#include "MonoTypes.h"

namespace Mono
{
	class Runner
	{
	public:
		struct SetupDesc
		{
			const char* m_monoLibFolder;
			const char* m_monoEtcConfigFolder;
			const char* m_assembliesPath;
			const char* m_domainName;
		};
		using AssemblyMap = std::unordered_map<std::string, AssemblyPtr>;

		Runner();
		~Runner() = default;

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
