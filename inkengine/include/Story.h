#pragma once

#include "Types.h"



// reflecting structures from https://github.com/spacecatprime/ink/tree/master/ink-engine-runtime

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
        virtual IVariablesState GetVariablesState() = 0;

        // The entire current state of the story including (but not limited to):
        // 
        //  * Global variables
        //  * Temporary variables
        //  * Read/visit and turn counts
        //  * The callstack and evaluation stacks
        //  * The current threads
        // 
        virtual IStoryState GetStoryState() = 0;

        // Construct a Story object using a JSON string compiled through inklecate.
        virtual bool LoadStory(std::string jsonString) = 0;

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
        // This is as opposed to the Continue() method which only evaluates one line of
        // output at a time.
        virtual std::string ContinueMaximally() = 0;

        // Change the current position of the story to the given path.
        // From here you can call Continue() to evaluate the next line.
        // The path string is a dot-separated path as used internally by the engine.
        virtual void ChoosePathString(std::string path, ArgumentList& arguments) = 0;

        /// Chooses the Choice from the currentChoices list with the given
        /// index. Internally, this sets the current content path to that
        /// pointed to by the Choice, ready to continue story evaluation.
        virtual void ChooseChoiceIndex(int choiceIdx) = 0;

        // Checks if a function exists.
        // <param name="functionName">The name of the function as declared in ink.</param>
        virtual bool HasFunction(const std::string& functionName) const = 0;

        /// <summary>
        /// Evaluates a function defined in ink.
        /// </summary>
        /// <returns>The return value as returned from the ink function with `~ return myValue`, or null if nothing is returned.</returns>
        /// <param name="functionName">The name of the function as declared in ink.</param>
        /// <param name="arguments">The arguments that the ink function takes, if any. Note that we don't (can't) do any validation on the number of arguments right now, so make sure you get it right!</param>
        virtual IObject EvaluateFunction(const std::string&  functionName, ArgumentList& arguments);
    };

    /// <summary>
    /// All story state information is included in the StoryState class,
    /// including global variables, read counts, the pointer to the current
    /// point in the story, the call stack (for tunnels, functions, etc),
    /// and a few other smaller bits and pieces. You can save the current
    /// state using the json serialisation functions ToJson and LoadJson.
    /// </summary>
    class IStoryState
    {
    public:
        /// The current version of the state save file JSON-based format.
        virtual int GetInkSaveStateVersion() = 0;
    
        /// <summary>
        /// Exports the current state to json format, in order to save the game.
        /// </summary>
        /// <returns>The save state in json format.</returns>
        virtual std::string ToJson() = 0;

        /// <summary>
        /// Loads a previously saved state in JSON format.
        /// </summary>
        /// <param name="json">The JSON string to load.</param>
        virtual void LoadJson(std::string json) = 0;

        /// Gets the visit/read count of a particular Container at the given path.
        virtual int VisitCountAtPathString(std::string pathString) = 0;

        // TODO map in RapidJSON 
        /// <summary>
        /// Object representation of full JSON state. Usually you should use
        /// LoadJson and ToJson since they serialise directly to string for you.
        /// But it may be useful to get the object representation so that you
        /// can integrate it into your own serialisation system.
        /// </summary>
        //virtual Dictionary<string, object> jsonToken;

        /// Ends the current ink flow, unwrapping the callstack but without
        /// affecting any variables. Useful if the ink is (say) in the middle
        /// a nested tunnel, and you want it to reset so that you can divert
        /// elsewhere using ChoosePathString(). Otherwise, after finishing
        /// the content you diverted to, it would continue where it left off.
        /// Calling this is equivalent to calling -> END in ink.
        virtual void ForceEnd() = 0;
    };
};