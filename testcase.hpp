//@	{"targets":[{"name":"testcase.hpp","type":"include"}]}

#ifndef STIC_TESTCASE_HPP
#define STIC_TESTCASE_HPP

#include <string>

namespace Stic
	{
	class Testcase
		{
		public:
			struct Result
				{
				Result() noexcept:status(0){}
				int status;
				int failure_line;
				std::string failure_text;
				};

			typedef void (*Callback)(Result& res);

			explicit Testcase(const char* name, Callback cb): m_name(name), m_callback(cb)
				{}

			void run()
				{m_callback(m_result);}

			const char* name() const noexcept
				{return m_name.c_str();}

			const Result& result() const noexcept
				{return m_result;}

		private:
			std::string m_name;
			Callback m_callback;
			Result m_result;
		};
	}

#endif


