//@	{
//@	"targets":[{"name":"test","type":"application"}]
//@	}

#include "stic.hpp"

STIC_TESTCASE("Broken assert")
	{
	STIC_ASSERT(1==2);
	}

STIC_TESTCASE("Working assert")
	{
	STIC_ASSERT(0==0);
	}

STIC_TESTCASE("Broken throw")
	{
	STIC_ASSERT_THROW(puts("No exception");
		puts("More than one line"););
	}

STIC_TESTCASE("Working throw")
	{
	STIC_ASSERT_THROW(
		puts("This is not going to work...");
		throw "This is an exception";
		);
	}
