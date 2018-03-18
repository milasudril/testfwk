# stic
Simple Test Infrastructure for C++

This is a simple testing framework for C++, especially designed for Maike integration. It is inspired by googletest, but it has no integrated mock system. It features IDE-friendly logging in *file*:*line*: *message* format. Also, test cases can have arbitrary name. For usage, see `test.cpp`. Notice that including `stic.hpp` will pull in an implementation of main to the dependency list. In case one test suite has multiple TU, mark all as type `object`, and add them as dependencies to an application target defined in a stub file, ie

    //@	{
    //@	"targets":[{"name":"test","type":"application","dependencies":[{"ref":"foo.o","rel":"implementation"}]}]
    //@	}

    // This file is empty, as all functions are defined in foo.o, and its dependencies.

STIC includes the following macros:

  * `STIC_TESTCASE("name") {/*code*/}` - Defines a testcase
  * `STIC_ASSERT(expression)` - Log an error and continue to the next testcase if expression is false
  * `STIC_ASSERT_THROW(statements)` - Log and error and continue to the next testcase if statmets does not throw an exception
  * `STIC_ASSERT_NOTHROW(statements)` - Log and error and continue to the next testcase if statmets does throw an exception

It is possible to use these macros in a non-void function, as they use exceptions insstad of an empty return. STIC will treat any uncaught exception as an error, and thus, it is possible to see unexpected exceptions without `STIC_ASSERT_NOTHROW`. However, using `STIC_ASSERT_NOTHROW` will give you a faster way of finding the testcase that threw the exception.
