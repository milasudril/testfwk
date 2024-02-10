#ifndef TESTFWK_LIFETIME_CHECKER_HPP
#define TESTFWK_LIFETIME_CHECKER_HPP

#include <cstdio>
#include <exception>

namespace testfwk
{
	class object_registry
	{
	public:
		template<class Type>
 		void announce_ctor(Type const* obj)
		{
			if(m_remaining_ctor_expects == 0)
			{
				fprintf(stderr, "error: %p unexpected ctor\n", obj);
				fflush(stderr);
				std::terminate();
			}

			--m_remaining_ctor_expects;
			++m_remaining_dtor_expects;
		}

		template<class Type>
		void announce_dtor(Type const* obj)
		{
			if(m_remaining_dtor_expects == 0)
			{
				fprintf(stderr, "error: %p unexpected destructor\n", obj);
				fflush(stderr);
				std::terminate();
			}

			--m_remaining_dtor_expects;
		}

		template<class Type>
		void announce_move_ctor(Type const& src, Type const* dest)
		{
			if(m_remaining_move_ctor_expects == 0)
			{
				fprintf(stderr, "error: %p -> %p unexpected move ctor\n", &src, dest);
				fflush(stderr);
				std::terminate();
			}

			--m_remaining_move_ctor_expects;
			++m_remaining_dtor_expects;
		}

		template<class Type>
		void announce_copy_ctor(Type const& src, Type const* dest)
		{
			if(m_remaining_copy_ctor_expects == 0)
			{
				fprintf(stderr, "error: %p -> %p unexpected copy ctor\n", &src, dest);
				fflush(stderr);
				std::terminate();
			}

			--m_remaining_copy_ctor_expects;
			++m_remaining_dtor_expects;
		}

		template<class Type>
		void announce_move_assign(Type const& src, Type const* dest)
		{
			if(m_remaining_move_assign_expects == 0)
			{
				fprintf(stderr, "error: %p -> %p unexpected move assign\n", &src, dest);
				fflush(stderr);
				std::terminate();
			}

			--m_remaining_move_assign_expects;
			++m_remaining_dtor_expects;
		}

		template<class Type>
		void announce_copy_assign(Type const& src, Type const* dest)
		{
			if(m_remaining_copy_assign_expects == 0)
			{
				fprintf(stderr, "error: %p -> %p unexpected copy assign\n", &src, dest);
				fflush(stderr);
				std::terminate();
			}

			--m_remaining_copy_assign_expects;
			++m_remaining_dtor_expects;
		}

		void expect_ctor()
		{	++m_remaining_ctor_expects; }

		void expect_move_ctor()
		{ ++m_remaining_move_ctor_expects; }

		void expect_copy_ctor()
		{ ++m_remaining_copy_ctor_expects; }

		void expect_move_assign()
		{ ++m_remaining_move_assign_expects; }

		void expect_copy_assign()
		{ ++m_remaining_copy_assign_expects; }

		~object_registry()
		{
			if(m_remaining_ctor_expects != 0)
			{
				fprintf(stderr, "error: Exepcted %zu more objects\n", m_remaining_ctor_expects);
				fflush(stderr);
				std::terminate();
			}

			if(m_remaining_move_ctor_expects != 0)
			{
				fprintf(stderr, "error: Exepcted %zu more move ctor\n", m_remaining_move_ctor_expects);
				fflush(stderr);
				std::terminate();
			}

			if(m_remaining_copy_ctor_expects != 0)
			{
				fprintf(stderr, "error: Exepcted %zu more copy ctor\n", m_remaining_copy_ctor_expects);
				fflush(stderr);
				std::terminate();
			}

			if(m_remaining_move_assign_expects != 0)
			{
				fprintf(stderr, "error: Exepcted %zu more move assign\n", m_remaining_move_assign_expects);
				fflush(stderr);
				std::terminate();
			}

			if(m_remaining_copy_assign_expects != 0)
			{
				fprintf(stderr, "error: Exepcted %zu more copy assign\n", m_remaining_copy_assign_expects);
				fflush(stderr);
				std::terminate();
			}

			if(m_remaining_dtor_expects != 0)
			{
				fprintf(stderr, "error: %zu objects are still alive\n", m_remaining_dtor_expects);
				fflush(stderr);
				std::terminate();
			}
		}

	private:
		size_t m_remaining_ctor_expects{};
		size_t m_remaining_move_ctor_expects{};
		size_t m_remaining_copy_ctor_expects{};
		size_t m_remaining_move_assign_expects{};
		size_t m_remaining_copy_assign_expects{};

		size_t m_remaining_dtor_expects{};
	};

	template<class T>
	class lifetime_checker
	{
	public:
		template<class ... Args>
		lifetime_checker(Args&&... args): m_value{std::forward<Args>(args)...}
		{ s_known_objects.announce_ctor(this); }

		lifetime_checker(lifetime_checker&& other) noexcept: m_value{std::move(other.m_value)}
		{ s_known_objects.announce_move_ctor(other, this); }

		lifetime_checker(lifetime_checker const& other): m_value{other.m_value}
		{ s_known_objects.announce_copy_ctor(other, this); }

		lifetime_checker& operator=(lifetime_checker&& other) noexcept
		{
			m_value = std::move(other.m_value);
			s_known_objects.announce_move_assign(other, this);
			return *this;
		}

		lifetime_checker& operator=(lifetime_checker const& other)
		{
			m_value = other.m_value;
			s_known_objects.announce_copy_assign(other, this);
			return *this;
		}

		~lifetime_checker()
		{ s_known_objects.announce_dtor(this); }

		operator T const&() const
		{ return m_value; }

		operator T&()
		{ return m_value; }

		auto operator<=>(lifetime_checker const& other) const = default;

		static void expect_ctor()
		{	s_known_objects.expect_ctor(); }

		static void expect_move_ctor()
		{ s_known_objects.expect_move_ctor(); }

		static void expect_copy_ctor()
		{ s_known_objects.expect_copy_ctor(); }

		static void expect_move_assign()
		{ s_known_objects.expect_move_assign(); }

		static void expect_copy_assign()
		{ s_known_objects.expect_copy_assign(); }

	private:
		T m_value;
		static object_registry s_known_objects;
	};

	template<class T>
	constinit object_registry lifetime_checker<T>::s_known_objects;
}

#endif