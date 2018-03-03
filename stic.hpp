//@	{
//@	"targets":[{"name":"stic.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"stic.o","rel":"implementation"}]
//@	}

#ifndef STIC_H
#define STIC_H

#include "registry.hpp"

#define STIC_COMBINE1(X,Y) X##Y  // helper macro
#define STIC_COMBINE(X,Y) STIC_COMBINE1(X,Y)

#define STIC_MAKE_UNIQUE_NAME(identifier, n)\
	STIC_COMBINE(identifier, n)

#define TESTCASE(name) \
	static void STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__) (Stic::Testcase::Result& result); \
	static auto STIC_MAKE_UNIQUE_NAME(_stic_dummy, __LINE__) = Stic::TestRegistry::add(Stic::Testcase{name, STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__)}); \
	static void STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__) (Stic::Testcase::Result& result)

#endif
