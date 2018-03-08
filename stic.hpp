//@	{
//@	"targets":[{"name":"stic.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"stic.o","rel":"implementation"}]
//@	}

#ifndef STIC_HPP
#define STIC_HPP

#include "registry.hpp"

#define STIC_COMBINE1(X,Y) X##Y  // helper macro
#define STIC_COMBINE(X,Y) STIC_COMBINE1(X,Y)

#define STIC_MAKE_UNIQUE_NAME(identifier, n)\
	STIC_COMBINE(identifier, n)

#define STIC_TESTCASE(name) \
	static void STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__) (Stic::Testcase::Result& result); \
	static auto STIC_MAKE_UNIQUE_NAME(_stic_dummy, __LINE__) = Stic::TestRegistry::add(__FILE__,Stic::Testcase{name, STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__)}); \
	static void STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__) (Stic::Testcase::Result& result)

#define STIC_ASSERT(expression) \
	if(!(expression)) \
		{ \
		result.line = __LINE__; \
		result.status = Stic::Status::Failure; \
		result.message = std::string("Expected ") + #expression; \
		return; \
		}

#endif
