#pragma once
// ********************************************************
// Ink Runtime
// ********************************************************

#include "abstract\IStory.h"

namespace Ink
{
	class StoryException 
		: public IStoryException
	{
	public:
		// Inherited via IStoryException
		const std::string& GetMessage() override;

	private:
		std::string m_message;
	};
};
