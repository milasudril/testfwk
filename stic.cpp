//@	{"targets":[{"name":"stic.o","type":"object"}]}

#include "registry.hpp"

namespace
	{
	class TestLogger
		{
		public:
			void pre(const char* test_name)
				{
				fprintf(stderr,"Running \"%s\"...         ", test_name);
				}
			void post(const Stic::Testcase::Result& result)
				{
				if(result.status)
					{fprintf(stderr,"Failed\n");}
				else
					{fprintf(stderr,"Successed\n");}
				}
		};
	}

int main()
	{
	return Stic::TestRegistry::runAllTests(TestLogger{});
	}
