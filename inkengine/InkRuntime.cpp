#include "include/inkcpp/InkRuntime.h"
#include "include/inkcpp/InkFactory.h"

#include <MonoAssembly.h>
#include <MonoClass.h>
#include <MonoLogger.h>
#include <MonoRunner.h>
#include <MonoDomain.h>

namespace Ink
{
	Runtime::Runtime()
	{
		if (Mono::GetLogger() == nullptr)
		{
			Mono::SetLogger(Mono::CreateDefaultLogger());
		}
	}
	Runtime::~Runtime()
	{
	}
	bool Runtime::Setup(const Configuration& config)
	{
		m_setup = config.m_monoSetup;
		m_runner = std::make_unique<Mono::Runner>();

		if (!m_runner->Setup(m_setup))
		{
			m_runner.release();
			return false;
		}

#ifdef _DEBUG
		m_inkAssembly = m_runner->GetDomain()->LoadAssembly("ink-engine-runtime-debug.dll", m_setup.m_assembliesPath);
#else
		m_inkAssembly = m_runner->GetDomain()->LoadAssembly("ink-engine-runtime-release.dll", m_setup.m_assembliesPath);
#endif
		if (!m_inkAssembly->IsValid())
		{
			m_runner.release();
			return false;
		}

		return true;
	}
};
