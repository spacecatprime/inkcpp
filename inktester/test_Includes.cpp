/**********************************************************


***********************************************************/
#include <Catch\single_include\catch.hpp>

#include <Types.h>
#include <IInkListItem.h>
#include <IInkList.h>
#include <IStoryException.h>
#include <IVariablesState.h>
#include <IStoryException.h>
#include <IStory.h>
#include <IStoryState.h>
#include <IChoice.h>

TEST_CASE("include compile test", "[ink]")
{
	REQUIRE(true);

	Ink::ObjectPtr objptr;
	Ink::InkListPtr inklist;
	Ink::InkListItemPtr itemptr;
	Ink::StoryPtr s;
	Ink::ChoicePtr ch;
	Ink::StoryStatePtr ss;
	Ink::VariablesStatePtr vs;
}
