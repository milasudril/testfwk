//@	{"targets":[{"name":"testcase.hpp","type":"include"}]}

#ifndef STIC_TESTCASE_HPP
#define STIC_TESTCASE_HPP

#include <string>

namespace Stic
	{
	enum class Status:int{Success,Failure};

	class Testcase
		{
		public:
			enum class ShouldFail:int{No, Yes};
			struct Result
				{
				explicit Result(int line, Status status, std::string&& message) noexcept:
					line(line), status(status), message(std::move(message))
					{}

				Result() noexcept:status(Status::Success){}

				int line;
				Status status;
				std::string message;
				};

			typedef void (*Callback)();

			explicit Testcase(const char* name, Callback cb, int line, ShouldFail should_fail=ShouldFail::No):
				m_name(name), m_callback(cb), m_line(line), m_should_fail(should_fail)
				{}

			void run()
				{
				if(m_should_fail == ShouldFail::Yes)
					{
					try
						{
						m_callback();
						throw Result(m_line, Status::Failure, "Testcase did not fail");
						}
					catch(...)
						{}
					}
				else
					{m_callback();}
				}

			const char* name() const noexcept
				{return m_name.c_str();}

		private:
			std::string m_name;
			Callback m_callback;
			int m_line;
			ShouldFail m_should_fail;
		};
	}

#endif


