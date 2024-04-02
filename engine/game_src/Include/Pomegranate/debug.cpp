#include "debug.h"
namespace Pomegranate
{
    void print_pass(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[42m" << " PASS " << "\033[0m" << " " << text << std::endl;
    }
    void print_pass(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_fail(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[101m" << " FAIL " << "\033[0m" << " " << text << std::endl;
    }
    void print_fail(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_error(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[41m" << " ERROR " << "\033[0m" << " " << text << std::endl;
    }
    void print_error(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_warn(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[103m" << " WARN " << "\033[0m" << " " << text << std::endl;
    }
    void print_warn(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_info(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[104m" << " INFO " << "\033[0m" << " " << text << std::endl;
    }
    void print_info(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_debug(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[100m" << " DEBUG " << "\033[0m" << " " << text << std::endl;
    }
    void print_debug(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_notice(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[46m" << " NOTICE " << "\033[0m" << " " << text << std::endl;
    }
    void print_notice(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_log(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[100m" << " LOG " << "\033[0m" << " " << text << std::endl;
    }
    void print_log(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_ready(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[45m" << " READY " << "\033[0m" << " " << text << std::endl;
    }
    void print_ready(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
    void print_assert(const std::string& text)
    {
        std::cout << "\033[37m" << "\033[100m" << " ASSERT " << "\033[0m" << " " << text << std::endl;
    }
    void print_assert(const char* text, ...)
    {
        va_list args;
        va_start(args, text);
        vprintf(text, args);
        va_end(args);
        std::cout << std::endl;
    }
}
