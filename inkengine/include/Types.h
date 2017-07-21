#pragma once
// ********************************************************
// Ink types
// ********************************************************

#include <string>
#include <vector>

namespace Ink
{
    class IObject;
    class IVariablesState;
    class IChoice;
    class IStory;
    class IObject;
    class IStoryState;
    class IVariablesState;

    using ChoiceList = std::vector<IChoice>;
    using TagList = std::vector<std::string>;
    using StringList = std::vector<std::string>;
    using ArgumentList = const std::vector<void*>;
    // TODO map in RapidJSON using JsonDOM = std::unordered_map<std::string, std::any>;
};

