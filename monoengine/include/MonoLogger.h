#pragma once

#include <string>

#include <mono/jit/jit.h>

namespace Mono
{
	class ILogger
	{
	public:
		virtual void SetLevel(int level) = 0;
		virtual std::string ToString(const char* format, ...) = 0;

		virtual void Trace(std::string msg) = 0;
		virtual void Debug(std::string msg) = 0;
		virtual void Notice(std::string msg) = 0;
		virtual void Warning(std::string msg) = 0;
		virtual void Error(std::string msg) = 0;
		virtual void Exception(MonoObject* except) = 0;
	};

	ILogger* CreateDefaultLogger();
	void SetLogger(ILogger* logger);
	ILogger* GetLogger();
}