#pragma once
#include <boost/exception/all.hpp>
#include <boost/stacktrace.hpp>


typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

template <class exception>
[[noreturn]] inline void fngn_throw(const exception& e) noexcept(false)
{
	throw boost::enable_error_info(e) << traced(boost::stacktrace::stacktrace());
}