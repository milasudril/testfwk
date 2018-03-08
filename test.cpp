//@	{
//@	"targets":[{"name":"test","type":"application"}]
//@	}

#include "stic.hpp"

STIC_TESTCASE("Do awsome stuff")
	{
	STIC_ASSERT(1==2);
	}

STIC_TESTCASE("Do other stuff")
	{
	STIC_ASSERT(0==0);
	}
