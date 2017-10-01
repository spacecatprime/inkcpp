#include "../include/MonoLogger.h"
#include "../include/MonoUtility.h"

#include <mono/jit/jit.h>

#include <cstdio>
#include <cstdarg>

#ifdef _WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#define DEBUG_PRINT ::OutputDebugStringA
#else
	#define DEBUG_PRINT // 
#endif

namespace Mono
{
	struct DefaultLogger 
		: public ILogger
	{
		int m_level = 0;

		void Printf(const char* category, const char* msg)
		{
			std::printf("%s: %s\n", category, msg);
			DEBUG_PRINT(category);
			DEBUG_PRINT(": ");
			DEBUG_PRINT(msg);
			DEBUG_PRINT("\n");
		}

		// Inherited via ILogger
		void SetLevel(int level) override
		{
			m_level = level;
		}
		
		std::string ToString(const char* format, ...) override
		{
			if (!format)
			{
				return "<null-format>";
			}
			char dest[1024];
			va_list argptr;
			va_start(argptr, format);
			vsprintf_s(dest, format, argptr);
			va_end(argptr);
			return std::string(dest);
		}

		void Trace(std::string msg) override
		{
			if (m_level <= 0)
			{
				Printf("trace", msg.c_str());
			}
		}
		void Debug(std::string msg) override
		{
			if (m_level <= 1)
			{
				Printf("debug", msg.c_str());
			}
		}
		void Notice(std::string msg) override
		{
			if (m_level <= 2)
			{
				Printf("notice", msg.c_str());
			}
		}
		void Warning(std::string msg) override
		{
			if (m_level <= 3)
			{
				Printf("warning", msg.c_str());
			}
		}
		void Error(std::string msg) override
		{
			if (m_level <= 4)
			{
				Printf("error", msg.c_str());
			}
		}
		void Exception(MonoObject* except) override
		{
			Printf("except", Mono::ToString(except).c_str());
		}
	};

	static ILogger* s_theLogger = nullptr;

	ILogger* CreateDefaultLogger()
	{
		return new DefaultLogger();
	}

	void SetLogger(ILogger* logger)
	{
		s_theLogger = logger;
	}

	ILogger* GetLogger()
	{
		if (!s_theLogger)
		{
			SetLogger(CreateDefaultLogger());
			s_theLogger->Trace("Auto-installed default logger.");
		}
		return s_theLogger;
	}
}

