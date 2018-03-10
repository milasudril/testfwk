//@	{"targets":[{"name":"stic.o","type":"object"}]}

#include "registry.hpp"
#include <unistd.h>
#include <signal.h>
#include <cstring>

namespace
	{
	class TestLogger
		{
		public:
			void beginSuite(const std::string& name)
				{
				r_suite=&name;
				fprintf(stderr, "\n\nRunning tests from %s\n\n", r_suite->c_str());
				}

			void beginTest(const std::string& name)
				{
				fprintf(stderr,"Running %s...\n", name.c_str());
				fflush(stderr);
				}

			void endTest(const std::string&, const Stic::Testcase::Result& result)
				{
				if(result.status==Stic::Status::Failure)
					{fprintf(stderr,"%s:%d: error: %s\n", r_suite->c_str(), result.line, result.message.c_str());}
				else
					{
					fprintf(stderr,"Success\n");
					fflush(stderr);
					}
				}

			void endSuite(const std::string&)
				{
				fprintf(stderr,"\n");
				fflush(stderr);
				}

		private:
			const std::string* r_suite;
		};
	}


int main()
	{
	struct sigaction sigact{};
	struct sigaction oldact{};
	sigact.sa_handler=[](int signum)
		{
		char buff[512]={};
		auto buffer=&buff[0];
		strcat(buffer,"Process received signal ");
		buffer+=24;

		switch(signum)
			{
			case SIGQUIT:
				strcat(buffer,"SIGQUIT");
				buffer+=7;
				break;

			case SIGILL:
				strcat(buffer,"SIGILL");
				buffer+=6;
				break;

			case SIGABRT:
				strcat(buffer,"SIGABRT");
				buffer+=7;
				break;

			case SIGFPE:
				strcat(buffer,"SIGFPE");
				buffer+=6;
				break;

			case SIGSEGV:
				strcat(buffer,"SIGSEGV");
				buffer+=7;
				break;
			}
		strcat(buffer,". Terminating.\n");
		buffer+=15;
		write(STDERR_FILENO, buff, buffer - buff);
		_exit(128 + signum);
		};

	sigaction(SIGQUIT, &sigact, &oldact);
	sigaction(SIGILL, &sigact, &oldact);
	sigaction(SIGABRT, &sigact, &oldact);
	sigaction(SIGFPE, &sigact, &oldact);
	sigaction(SIGSEGV, &sigact, &oldact);

	return Stic::TestRegistry::runAllTests(TestLogger{})!=Stic::Status::Success;
	}
