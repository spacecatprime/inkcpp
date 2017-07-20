// monotester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <mono/jit/jit.h>
#include <MonoArgs.h>
#include <MonoRunner.h>
#include <MonoUtility.h>

// --break --success --name <somename> --order lex

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

unsigned int Factorial(unsigned int number) 
{
	return number <= 1 ? number : Factorial(number - 1)*number;
}

TEST_CASE("Factorials are computed", "[factorial]") 
{
	REQUIRE(Factorial(1) == 1);
	REQUIRE(Factorial(2) == 2);
	REQUIRE(Factorial(3) == 6);
	REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("zzz_lastone", "[system]")
{
	REQUIRE(true);
}
