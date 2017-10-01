#pragma once

#include "MonoTypes.h"
#include "MonoUtility.h"

namespace Mono
{
	class Runner 
		: public Singleton<Runner>
	{
	public:
		struct SetupDesc
		{
			std::string m_monoLibFolder;
			std::string m_monoEtcConfigFolder;
			std::string m_assembliesPath;
			std::string m_domainName;
		};

		Runner();
		~Runner();

		bool Setup(SetupDesc& desc);

		inline ImagePtr GetMscorlib()
		{
			return m_mscorlib;
		}

		inline DomainPtr GetDomain() 
		{ 
			return m_domain;
		}

		inline AssemblyPtr GetSystemAssembly() const
		{
			return m_systemAssembly;
		}

		inline const std::string& GetBaseAssemblyPath() const
		{
			return m_baseAssemblyPath;
		}

	protected:

		AssemblyPtr m_systemAssembly;
		ImagePtr m_mscorlib;
		DomainPtr m_domain;
		std::string m_baseAssemblyPath;
	};
}
