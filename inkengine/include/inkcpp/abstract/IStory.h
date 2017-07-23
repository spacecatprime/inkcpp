#pragma once
// ********************************************************
// TBD
// ********************************************************

#include "../Types.h"

namespace Ink
{
    class IStory
    {
    public:
        // The current version of the ink story file format.
        virtual int GetInkVersionCurrent() = 0;

        // The list of Choice objects available at the current point in
        // the Story. This list will be populated as the Story is stepped
        // through with the Continue() method. Once canContinue becomes
        // false, this list will be populated, and is usually
        // (but not always) on the final Continue() step.
        virtual ChoiceList GetCurrentChoices() = 0;

        // The latest line of text to be generated from a Continue() call.
        virtual std::string GetCurrentText() = 0;

        // Gets a list of tags as defined with '#' in source that were seen during the latest Continue() call.
        virtual TagList GetCurrentTags() = 0;

        // Any errors generated during evaluation of the Story.
        virtual StringList GetCurrentErrors() = 0;

        // Whether the currentErrors list contains any errors.
        virtual bool HasError() = 0;

        // The VariablesState object contains all the global variables in the story.
        // However, note that there's more to the state of a Story than just the
        // global variables. This is a convenience accessor to the full state object.
        virtual VariablesStatePtr GetVariablesState() = 0;

        // The entire current state of the story including (but not limited to):
        // 
        //  * Global variables
        //  * Temporary variables
        //  * Read/visit and turn counts
        //  * The callstack and evaluation stacks
        //  * The current threads
        // 
        virtual StoryStatePtr GetStoryState() = 0;

        // Construct a Story object using a JSON string compiled through inklecate.
        virtual bool LoadStory(const std::string& jsonString) = 0;

        // The Story itself in JSON representation.
        virtual std::string ToJsonString() = 0;

        /// Reset the Story back to its initial state as it was when it was first constructed.
        virtual void ResetState() = 0;

        // Reset the runtime error list within the state.
        virtual void ResetErrors() = 0;

        virtual void ResetGlobals() = 0;

        // Check whether more content is available if you were to call <c>Continue()</c> - i.e.
        // are we mid story rather than at a choice point or at the end.
        virtual bool CanContinue() = 0;

        // Continue the story until the next choice point or until it runs out of content.
        // This is as opposed to the Continue() method which only evaluates one line of output at a time.
        virtual std::string ContinueMaximally() = 0;

        // Change the current position of the story to the given path.
        // From here you can call Continue() to evaluate the next line.
        // The path string is a dot-separated path as used internally by the engine.
        virtual void ChoosePathString(const std::string& path, ArgumentList& arguments) = 0;

        /// Chooses the Choice from the currentChoices list with the given
        /// index. Internally, this sets the current content path to that
        /// pointed to by the Choice, ready to continue story evaluation.
        virtual void ChooseChoiceIndex(int choiceIdx) = 0;

        // Checks if a function exists.
        // <param name="functionName">The name of the function as declared in ink.</param>
        virtual bool HasFunction(const std::string& functionName) const = 0;

        /// Evaluates a function defined in ink.
        virtual ObjectPtr EvaluateFunction(const std::string&  functionName, ArgumentList& arguments) = 0;
    };
};