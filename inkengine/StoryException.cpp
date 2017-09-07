/**
*/
#include "include/inkcpp/abstract/IStoryException.h"
#include "include/inkcpp/InkFactory.h"

namespace Ink
{
	// @todo get parameters from params

	class StoryException : public IStoryException
	{
		// Inherited via IStoryException
		virtual const std::string& GetMessage() override
		{
			// TODO: insert return statement here
			return m_message;
		}
		std::string m_message;
	};

	template <>
	StoryExceptionPtr Factory::Create(Runtime& runtime, Factory::Parameters params)
	{
		return StoryExceptionPtr(new StoryException);
	}
};

