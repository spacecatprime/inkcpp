// monoengine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "monoengine.h"

#include <mono/jit/jit.h>

#include "include\MonoArgs.h"
#include "include\MonoAssembly.h"
#include "include\MonoClass.h"
#include "include\MonoDomain.h"
#include "include\MonoImage.h"
#include "include\MonoMethod.h"
#include "include\MonoObject.h"
#include "include\MonoRunner.h"
#include "include\MonoString.h"
#include "include\MonoTypes.h"

#pragma comment(lib, "mono-2.0-sgen.lib")

// This is an example of an exported variable
MONOENGINE_API int nmonoengine=0;

// This is an example of an exported function.
MONOENGINE_API int fnmonoengine(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see monoengine.h for the class definition
Cmonoengine::Cmonoengine()
{
    return;
}
