// inkengine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "inkengine.h"


// This is an example of an exported variable
INKENGINE_API int ninkengine=0;

// This is an example of an exported function.
INKENGINE_API int fninkengine(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see inkengine.h for the class definition
Cinkengine::Cinkengine()
{
    return;
}
