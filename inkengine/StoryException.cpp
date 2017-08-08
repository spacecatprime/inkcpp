/**
*/
#include "include\inkcpp\StoryException.h"
#include "include\inkcpp\InkFactory.h"

namespace Ink
{
	// @todo get parameters from params

	template <>
	StoryExceptionPtr Factory::Create(Runtime& runtime, Factory::Parameters params)
	{
		return StoryExceptionPtr(new StoryException);
	}
	const std::string& StoryException::GetMessage()
	{
		return m_message;
	}
};

