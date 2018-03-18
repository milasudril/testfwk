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
	static void STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__) (); \
	static auto STIC_MAKE_UNIQUE_NAME(_stic_dummy, __LINE__) = Stic::TestRegistry::add(__FILE__,Stic::Testcase{name, STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__)}); \
	static void STIC_MAKE_UNIQUE_NAME(_stic_testcase, __LINE__) ()

#define STIC_ASSERT(expression) \
	if(!(expression)) \
		{ \
		throw Stic::Testcase::Result{__LINE__, Stic::Status::Failure, std::string("Expected ") + #expression}; \
		}

#define STIC_ASSERT_THROW(statements) \
	try  \
		{ \
		statements \
		throw Stic::Testcase::Result{__LINE__, Stic::Status::Failure, std::string("Expected ") + #statements + " to throw an exception"}; \
		} \
	catch(const Stic::Testcase::Result& res) \
		{throw;}\
	catch(...) \
		{}


#define STIC_ASSERT_NOTHROW(statements) \
	try  \
		{ \
		statements \
		} \
	catch(...) \
		{ \
		throw Stic::Testcase::Result{__LINE__, Stic::Status::Failure, std::string("Expected ") + #statements + " to not throw an exception"}; \
		}

#endif
