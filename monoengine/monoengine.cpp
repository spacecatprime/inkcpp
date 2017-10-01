// monoengine.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

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

