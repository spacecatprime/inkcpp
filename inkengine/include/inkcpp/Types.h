#pragma once
// ********************************************************
// Ink types
// ********************************************************

#include <string>
#include <vector>
#include <memory>

namespace Ink
{
    class IObject;
    class IVariablesState;
    class IChoice;
    class IStory;
    class IStoryState;
	class IInkListItem;
	class IInkList;
	class IStoryException;

    using ChoiceList = std::vector<IChoice>;
    using TagList = std::vector<std::string>;
    using StringList = std::vector<std::string>;
    using ArgumentList = const std::vector<void*>;
    // TODO map in RapidJSON using JsonDOM = std::unordered_map<std::string, std::any>;

	using ObjectPtr = std::shared_ptr<IObject>;
	using VariablesStatePtr = std::shared_ptr<IVariablesState>;
	using ChoicePtr = std::shared_ptr<IChoice>;
	using StoryStatePtr = std::shared_ptr<IStoryState>;
	using StoryPtr = std::shared_ptr<IStory>;
	using InkListItemPtr = std::shared_ptr<IInkListItem>;
	using InkListPtr = std::shared_ptr<IInkList>;
	using StoryExceptionPtr = std::shared_ptr<IStoryException>;
};

