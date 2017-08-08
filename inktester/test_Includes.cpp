/**********************************************************
 Compile tests
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

//
// only a compile test, no running intended 
//
TEST_CASE("include.compile.test", "[ink][!hide]")
{
	Ink::ObjectPtr objptr;
	Ink::InkListPtr inklist;
	Ink::InkListItemPtr itemptr;
	Ink::StoryPtr s;
	Ink::ChoicePtr ch;
	Ink::StoryStatePtr ss;
	Ink::VariablesStatePtr vs;
	Ink::StoryExceptionPtr exp;
}
