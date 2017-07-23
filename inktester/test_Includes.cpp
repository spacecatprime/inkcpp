/**********************************************************


***********************************************************/
#include <Catch\single_include\catch.hpp>

#include <inkcpp/Types.h>
#include <inkcpp/abstract/IInkListItem.h>
#include <inkcpp/abstract/IInkList.h>
#include <inkcpp/abstract/IStoryException.h>
#include <inkcpp/abstract/IVariablesState.h>
#include <inkcpp/abstract/IStoryException.h>
#include <inkcpp/abstract/IStory.h>
#include <inkcpp/abstract/IStoryState.h>
#include <inkcpp/abstract/IChoice.h>

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

#include <inkcpp/InkRuntime.h>
#include <inkcpp/InkFactory.h>

TEST_CASE("simple construction", "[ink]")
{
	Ink::Runtime runtime;

	Ink::Factory f;
	auto excp = f.Create<Ink::IStoryException>(runtime);
}