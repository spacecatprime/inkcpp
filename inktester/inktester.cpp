// inktester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <Catch\single_include\catch.hpp>

// See also: https://github.com/philsquared/Catch/blob/master/docs/tutorial.md

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

#include "../inkparser/inkparser.h"

TEST_CASE("ink parser basics", "[ink,parser]")
{
	InkParser parser;
}

#include "../inkengine/inkengine.h"

TEST_CASE("ink engine","[]")
{
	fninkengine();
}

/*
-h, -?, --help
-l, --list-tests
-t, --list-tags
-s, --success
-b, --break
-e, --nothrow
-i, --invisibles
-o, --out
-r, --reporter
-n, --name
-a, --abort
-x, --abortx
-w, --warn
-d, --durations
-f, --input-file
-c, --section
-#, --filenames-as-tags


--list-test-names-only
--list-reporters
--order
--rng-seed
*/