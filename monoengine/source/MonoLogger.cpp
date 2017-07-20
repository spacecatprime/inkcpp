#include "../include/MonoLogger.h"
#include "../include/MonoUtility.h"

#include <mono/jit/jit.h>

#include  <cstdio>

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
		virtual void SetLevel(int level) override
		{
			m_level = level;
		}
		virtual void Trace(std::string msg) override
		{
			if (m_level <= 0)
			{
				Printf("trace", msg.c_str());
			}
		}
		virtual void Debug(std::string msg) override
		{
			if (m_level <= 1)
			{
				Printf("debug", msg.c_str());
			}
		}
		virtual void Notice(std::string msg) override
		{
			if (m_level <= 2)
			{
				Printf("notice", msg.c_str());
			}
		}
		virtual void Warning(std::string msg) override
		{
			if (m_level <= 3)
			{
				Printf("warning", msg.c_str());
			}
		}
		virtual void Error(std::string msg) override
		{
			if (m_level <= 4)
			{
				Printf("error", msg.c_str());
			}
		}
		virtual void Exception(MonoObject* except) override
		{
			std::printf("except: %s\n", Mono::ToString(except).c_str());
		}
	};

	ILogger* CreateDefaultLogger()
	{
		return new DefaultLogger();
	}

	ILogger* theLogger = nullptr;

	void SetLogger(ILogger* logger)
	{
		theLogger = logger;
	}

	ILogger* GetLogger()
	{
		return theLogger;
	}
}

