//@	{"targets":[{"name":"stic.o","type":"object"}]}

#include "registry.hpp"

namespace
	{
	class TestLogger
		{
		public:
			void beginSuite(const std::string& name)
				{
				r_suite=&name;
				fprintf(stdout, "\n\nRunning tests from %s\n\n", r_suite->c_str());
				}

			void beginTest(const std::string& name)
				{
				fprintf(stdout,"Running %s...\n", name.c_str());
				fflush(stdout);
				}

			void endTest(const std::string&, const Stic::Testcase::Result& result)
				{
				if(result.status==Stic::Status::Failure)
					{fprintf(stderr,"%s:%d: error: %s\n", r_suite->c_str(), result.line, result.message.c_str());}
				else
					{
					fprintf(stdout,"Success\n");
					fflush(stdout);
					}
				}

			void endSuite(const std::string&)
				{
				fprintf(stdout,"\n");
				fflush(stdout);
				}

		private:
			const std::string* r_suite;
		};
	}

int main()
	{
	return Stic::TestRegistry::runAllTests(TestLogger{})!=Stic::Status::Success;
	}
