#pragma once

#include "Types.h"

namespace Ink
{
    /// <summary>
    /// All story state information is included in the StoryState class,
    /// including global variables, read counts, the pointer to the current
    /// point in the story, the call stack (for tunnels, functions, etc),
    /// and a few other smaller bits and pieces. You can save the current
    /// state using the JSON serialisation functions ToJson and LoadJson.
    /// </summary>
    class IStoryState
    {
    public:
        /// The current version of the state save file JSON-based format.
        virtual int GetInkSaveStateVersion() = 0;
    
        /// <summary>
        /// Exports the current state to JSON format, in order to save the game.
        /// </summary>
        /// <returns>The save state in JSON format.</returns>
        virtual std::string ToJson() = 0;

        /// <summary>
        /// Loads a previously saved state in JSON format.
        /// </summary>
        /// <param name="json">The JSON string to load.</param>
        virtual void LoadJson(const std::string& json) = 0;

        /// Gets the visit/read count of a particular Container at the given path.
        virtual int VisitCountAtPathString(const std::string& pathString) = 0;

        // TODO map in RapidJSON ?
        /// <summary>
        /// Object representation of full JSON state. Usually you should use
        /// LoadJson and ToJson since they serialise directly to string for you.
        /// But it may be useful to get the object representation so that you
        /// can integrate it into your own serialisation system.
        /// </summary>
        //virtual Dictionary<string, object> jsonToken;

        /// Ends the current ink flow, unwrapping the call stack but without
        /// affecting any variables. Useful if the ink is (say) in the middle
        /// a nested tunnel, and you want it to reset so that you can divert
        /// elsewhere using ChoosePathString(). Otherwise, after finishing
        /// the content you diverted to, it would continue where it left off.
        /// Calling this is equivalent to calling -> END in ink.
        virtual void ForceEnd() = 0;
    };
};