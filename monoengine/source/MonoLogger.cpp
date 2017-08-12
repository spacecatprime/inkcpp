#include "../include/MonoLogger.h"
#include "../include/MonoUtility.h"

#include <mono/jit/jit.h>

#include <cstdio>
#include <cstdarg>

namespace Mono
{
	struct DefaultLogger 
		: public ILogger
	{
		int m_level = 0;

		void Printf(const char* category, const char* msg)
		{
			std::printf("%s: %s\n", category, msg);
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
			std::printf("except: %s\n", Mono::ToString(except).c_str());
		}
	};

	static ILogger* theLogger = nullptr;

	ILogger* CreateDefaultLogger()
	{
		return new DefaultLogger();
	}

	void SetLogger(ILogger* logger)
	{
		theLogger = logger;
	}

	ILogger* GetLogger()
	{
		return theLogger;
	}
}

