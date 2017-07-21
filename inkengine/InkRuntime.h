#pragma once
// ********************************************************
// Ink Runtime
// ********************************************************

#include <MonoRunner.h>

namespace Ink
{
	class Runtime
	{
	public:
		struct Configuration
		{
			Mono::Runner::SetupDesc m_monoSetup;
		};

		Runtime();
		~Runtime();

		bool Setup(Configuration config);
	};
};

