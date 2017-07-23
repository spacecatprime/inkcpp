#pragma once
// ********************************************************
// Ink Runtime
// ********************************************************

#include <MonoRunner.h>

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
	};
};

