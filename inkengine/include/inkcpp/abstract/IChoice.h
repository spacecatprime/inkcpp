#pragma once
// ********************************************************
// TBD
// ********************************************************

#include "../Types.h"
namespace Ink
{
    class IChoice
    {
    public:
        // The main text to presented to the player for this Choice.
        virtual const std::string& GetText() = 0;

        // The target path that the Story should be diverted to if this Choice is chosen.
        virtual const std::string& GetPathStringOnChoice() = 0;

        // Get the path to the original choice point - where was this choice defined in the story?
        virtual const std::string& GetSourcePath() = 0;

        /// The original index into currentChoices list on the Story when this Choice was generated, for convenience.
        virtual int GetIndex() = 0;
        virtual void SetIndex(int index) = 0;
    };
};