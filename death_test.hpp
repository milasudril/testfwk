#ifndef TESTFWK_DEATHTEST_HPP
#define TESTFWK_DEATHTEST_HPP

#include "./testsuite.hpp"

#include <string_view>
#include <unistd.h>
#include <source_location>
#include <memory>
#include <cstdio>
#include <sys/wait.h>
#include <array>
#include <print>
#include <string>

namespace TestFwk
{
	namespace detail
	{
		struct file_deleter
		{
			static void operator()(FILE* file)
			{
				if(file != nullptr)
				{ fclose(file); }
			}
		};

		inline std::string read_until_closed(int fd)
		{
			std::unique_ptr<FILE, file_deleter> file{fdopen(fd, "rb")};
			std::string ret;
			while(true)
			{
				auto const ch_in = getc(file.get());
				if(ch_in == EOF)
				{ return ret; }
				ret += static_cast<char>(ch_in);
			}
			return ret;
		}
	}

	template<class Callable>
	void expect_death(
		Callable&& death_func,
		std::string_view expected_stderr,
		int expected_signal,
		std::source_location source_loc = std::source_location::current()
	)
	{
		std::array<int, 2> errpipe{};
		auto const res = pipe(std::data(errpipe));
		if(res == -1)
		{
			perror("Syscall pipe failed");
			abort();
		}

		auto const child = fork();
		if(child == -1)
		{
			perror("Syscall fork failed");
			abort();
		}

		if(child == 0)
		{
			if(dup2(errpipe[1], STDERR_FILENO) == -1)
			{
				perror("Syscall dup2 failed");
				abort();
			}
			if(close(errpipe[0]) == -1)
			{
				perror("Syscall close failed");
				abort();
			}
			std::forward<Callable>(death_func)();
			_exit(-1);
		}
		else
		{
			close(errpipe[1]);
			auto const output = detail::read_until_closed(errpipe[0]);
			int w_status{};
			auto const res = waitpid(child, &w_status, 0);
			if(res != child)
			{
				fprintf(stderr, "Wrong child");
				abort();
			}

			if(WIFEXITED(w_status))
			{
				TestFwk::currentTestcase->testcaseFailed();
				std::print(
					stderr,
					"{}:{}: Expected crash got return value",
					source_loc.file_name(),
					source_loc.line()
				);
			}

			if(!WIFSIGNALED(w_status))
			{
				TestFwk::currentTestcase->testcaseFailed();
				std::print(
					stderr,
					"{}:{}: Expected process to be killed by signal",
					source_loc.file_name(),
					source_loc.line()
				);
			}

			if(WTERMSIG(w_status) != expected_signal)
			{
				TestFwk::currentTestcase->testcaseFailed();
				std::print(
					stderr,
					"{}:{}: Expected process to be killed by signal {} but got {}",
					source_loc.file_name(),
					source_loc.line(),
					expected_signal,
					WTERMSIG(w_status)
				);
			}

			if(output != expected_stderr)
			{
				TestFwk::currentTestcase->testcaseFailed();
				std::print(
					stderr,
					"{}:{}: Expected process output to be equal to `{}` but got `{}`",
					source_loc.file_name(),
					source_loc.line(),
					expected_stderr,
					output
				);
			}
		}
	}
}

#endif
