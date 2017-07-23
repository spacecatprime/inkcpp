#include "include/inkcpp/InkRuntime.h"
#include "include/inkcpp/InkFactory.h"

#include <MonoAssembly.h>

namespace Ink
{
	Runtime::Runtime()
	{
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
		m_inkAssembly = m_runner->LoadAssembly("ink-engine-runtime-debug.dll");
#else
		m_inkAssembly = m_runner->LoadAssembly("ink-engine-runtime-release.dll");
#endif
		if (!m_inkAssembly->IsValid())
		{
			m_runner.release();
			return false;
		}

		return true;
	}
};

#include "include\inkcpp\abstract\IStoryException.h"

namespace Ink
{
	class StoryException: public IStoryException
	{
		// Inherited via IStoryException
		const std::string & GetMessage() override
		{
			return "";
		}
	};

	template <>
	StoryExceptionPtr Factory::Create(Runtime& runtime)
	{
		return StoryExceptionPtr(new StoryException);
	}
};