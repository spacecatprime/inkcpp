// inkparser.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "inkparser.h"


// This is an example of an exported variable
INKPARSER_API int ninkparser=0;

// This is an example of an exported function.
INKPARSER_API int fninkparser(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see inkparser.h for the class definition
InkParser::InkParser()
{
    return;
}

InkParser::~InkParser()
{
	return;
}
