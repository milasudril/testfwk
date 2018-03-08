//@	{
//@	"targets":[{"name":"registry.hpp","type":"include"}]
//@	,"dependencies_extra":[{"ref":"registry.o","rel":"implementation"}]
//@	}

#ifndef STIC_REGISTRY_HPP
#define STIC_REGISTRY_HPP

#include "testcase.hpp"
#include <algorithm>
#include <memory>
#include <vector>
#include <map>

namespace Stic
	{
	class Registry
		{
		public:
			template<class Logger>
			Status runAllTests(Logger&& logger)
				{
				Status status{Status::Success};
				std::for_each(m_tests.begin(), m_tests.end(), [&logger, &status](std::pair<const std::string, std::vector<Testcase>>& item)
					{
					logger.beginSuite(item.first);
					std::for_each(item.second.begin(), item.second.end(),[&logger, &status](Testcase& test)
						{
						logger.beginTest(test.name());
						test.run();
						logger.endTest(test.name(), test.result());
						status = status==Stic::Status::Success? test.result().status : status;
						});
					logger.endSuite(item.first);
					});
				return status;
				}

			void add(const char* filename, Testcase&& test)
				{m_tests[filename].push_back(std::move(test));}

		private:
			std::map<std::string, std::vector<Testcase>> m_tests;
		};

	class TestRegistry
		{
		public:
			template<class Logger>
			static Status runAllTests(Logger&& logger)
				{return s_registry->runAllTests(std::forward<Logger>(logger));}

			static int add(const char* filename, Testcase&& test)
				{
				if(s_registry==nullptr)
					{s_registry.reset(new Registry);}
				s_registry->add(filename, std::move(test));
				return 0;
				}
		private:
			static std::unique_ptr<Registry> s_registry;
		};
	};

#endif
