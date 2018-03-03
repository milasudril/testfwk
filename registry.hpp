//@	{
//@	"targets":[{"name":"registry.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"registry.o","rel":"implementation"}]
//@	}

#ifndef STIC_REGISTRY_HPP
#define STIC_REGISTRY_HPP

#include "testcase.hpp"
#include <algorithm>
#include <memory>

namespace Stic
	{
	class Registry
		{
		public:
			template<class Logger>
			int runAllTests(Logger&& logger)
				{
				int status;
				std::for_each(m_tests.begin(), m_tests.end(), [&logger, &status](Testcase& test)
					{
					logger.pre(test.name());
					test.run();
					logger.post(test.result());
					status = status==0? test.result().status : status;
					});
				return status;
				}

			void add(Testcase&& test)
				{m_tests.push_back(std::move(test));}

		private:
			std::vector<Testcase> m_tests;
		};

	class TestRegistry
		{
		public:
			template<class Logger>
			static int runAllTests(Logger&& logger)
				{return s_registry->runAllTests(std::forward<Logger>(logger));}

			static int add(Testcase&& test)
				{
				if(s_registry==nullptr)
					{s_registry.reset(new Registry);}
				s_registry->add(std::move(test));
				return 0;
				}
		private:
			static std::unique_ptr<Registry> s_registry;
		};
	};

#endif
