#pragma once
// ********************************************************
// Ink Runtime
// ********************************************************

#include <MonoRunner.h>
#include <MonoTypes.h>

#include <memory>

namespace Mono
{
	class Runner;
}

namespace Ink
{
    /**
      * Manages the Mono runtime 
      */      
	class Runtime
	{
	public:
		struct Configuration
		{
			Mono::Runner::SetupDesc m_monoSetup;
		};

		Runtime();
		~Runtime();

		bool Setup(const Configuration& config);

		Mono::AssemblyPtr GetInkAssembly() const { return m_inkAssembly;  }

	private:
		std::unique_ptr<Mono::Runner> m_runner;
		Mono::Runner::SetupDesc m_setup;
		Mono::AssemblyPtr m_inkAssembly;
	};
};

