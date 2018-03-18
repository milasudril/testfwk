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

			explicit Testcase(const char* name, Callback cb): m_name(name), m_callback(cb)
				{}

			void run()
				{m_callback();}

			const char* name() const noexcept
				{return m_name.c_str();}

		private:
			std::string m_name;
			Callback m_callback;
		};
	}

#endif


