#include "utils.h"
std::string scuffy_demangle(const char* name)
{
	std::string demangled_name = boost::core::demangle(name);

	// Remove __cdecl(void) from the end
	std::string cdecl_str = "__cdecl(void)";
	size_t pos = demangled_name.find(cdecl_str);
	if (pos != std::string::npos)
		demangled_name.erase(pos, cdecl_str.length());

	// Remove the parentheses if they exist at the end
	while (!demangled_name.empty() && demangled_name.back() == '(')
		demangled_name.pop_back();

	return demangled_name;
}