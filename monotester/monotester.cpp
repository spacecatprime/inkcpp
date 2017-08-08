// monotester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <mono/jit/jit.h>
#include <MonoArgs.h>
#include <MonoRunner.h>
#include <MonoUtility.h>

// --break --success --name <somename> --order lex
// --success --order lex

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("zzz.last.one", "[system]")
{
	REQUIRE(true);
}
