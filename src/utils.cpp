#include "utils.h"
std::string scuffy_demangle(const char* name)
{
	std::string demangled_name = boost::core::demangle(name);

	// Remove the class/struct/enum/union keyword at the beginning
	size_t pos = demangled_name.find_first_of(' ');
	if (pos != std::string::npos)
		demangled_name = demangled_name.substr(pos + 1);

	// Remove __cdecl(void) from the end
	std::string cdecl_str = "__cdecl(void)";
	pos = demangled_name.find(cdecl_str);
	if (pos != std::string::npos)
		demangled_name.erase(pos, cdecl_str.length());

	return demangled_name;
}