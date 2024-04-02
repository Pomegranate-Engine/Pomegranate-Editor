#include "utils.h"
std::string scuffy_demangle(const char* name)
{
	std::string demangled_name = boost::core::demangle(name);

#ifdef _WIN32
	// Check if the demangled name starts with "class "
	if (demangled_name.substr(0, 6) == "class ")
		demangled_name.erase(0, 6); // Remove "class " from the beginning
#endif

	// Remove __cdecl(void) from the end
	std::string cdecl_str = "__cdecl(void)";
	size_t pos = demangled_name.find(cdecl_str);
	if (pos != std::string::npos)
		demangled_name.erase(pos, cdecl_str.length());

	if(demangled_name.back() == ')')
	{
		// Remove after last (
		size_t last_open_paren = demangled_name.find_last_of('(');
		demangled_name.erase(last_open_paren);
	}

	return demangled_name;
}