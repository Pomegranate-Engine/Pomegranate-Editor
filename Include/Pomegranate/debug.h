#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include<string>
#include<cstdarg>
namespace Pomegranate
{
	void print_pass(const std::string&);
	void print_pass(const char*, ...);
	void print_fail(const std::string&);
	void print_fail(const char*, ...);
	void print_error(const std::string&);
	void print_error(const char*, ...);
	void print_warn(const std::string&);
	void print_warn(const char*, ...);
	void print_info(const std::string&);
	void print_info(const char*, ...);
	void print_debug(const std::string&);
	void print_debug(const char*, ...);
	void print_notice(const std::string&);
	void print_notice(const char*, ...);
	void print_log(const std::string&);
	void print_log(const char*, ...);
	void print_ready(const std::string&);
	void print_ready(const char*, ...);
	void print_assert(const std::string&);
	void print_assert(const char*, ...);
}

#endif //DEBUG_H
