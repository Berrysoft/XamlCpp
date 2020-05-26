#ifndef XAML_DEFAULT_HANDLER_COLOR
#if !defined(XAML_WIN32) || defined(XAML_MINGW)
#define XAML_DEFAULT_HANDLER_COLOR
#endif // !XAML_WIN32 || XAML_MINGW
#endif // !XAML_DEFAULT_HANDLER_COLOR

#ifdef XAML_WIN32
#include <Windows.h>
#endif // XAML_WIN32

#ifdef XAML_DEFAULT_HANDLER_COLOR
#include <sf/color.hpp>
#endif // XAML_DEFAULT_HANDLER_COLOR

#include <boost/nowide/iostream.hpp>
#include <iostream>
#include <map>
#include <sf/format.hpp>
#include <sf/sformat.hpp>
#include <sstream>
#include <xaml/event.h>
#include <xaml/result_handler.h>

#ifdef NDEBUG
#define XAML_DEFAULT_HANDLER xaml_result_handler_empty
#else
#define XAML_DEFAULT_HANDLER xaml_result_handler_default
#endif // NDEBUG

using namespace std;

static function<__xaml_result_handler_prototype> s_handler = XAML_DEFAULT_HANDLER;

void XAML_CALL xaml_result_raise(xaml_result hr, xaml_result_raise_level level, char const* file, int32_t line) noexcept
{
    // If exceptions throwed here, the program should terminate.
    std::string msg = sf::sprint(U("{}:{}"), file, line);
    s_handler(hr, level, msg.c_str());
}

void XAML_CALL xaml_result_handler_empty(xaml_result, xaml_result_raise_level, char const*) noexcept
{
}

static map<xaml_result_raise_level, std::string_view> s_level_map{
    { xaml_result_raise_info, U("INFO") },
    { xaml_result_raise_warning, U("WARNING") },
    { xaml_result_raise_error, U("ERROR") }
};

#ifdef XAML_DEFAULT_HANDLER_COLOR
static map<xaml_result_raise_level, sf::preset_color> s_level_color_map{
    { xaml_result_raise_info, sf::bright_blue },
    { xaml_result_raise_warning, sf::yellow },
    { xaml_result_raise_warning, sf::red }
};
#endif // XAML_DEFAULT_HANDLER_COLOR

static ostream& print_msg(ostream& stream, xaml_result hr, xaml_result_raise_level level, char const* msg)
{
#ifndef XAML_DEFAULT_HANDLER_COLOR
    return sf::println(stream, U("{}: 0x{:x}: {}"), s_level_map[level], hr, msg);
#else
    return sf::println(stream, U("{}: 0x{:x}: {}"), sf::make_color_arg(s_level_map[level], s_level_color_map[level]), hr, msg);
#endif // !XAML_DEFAULT_HANDLER_COLOR
}

void XAML_CALL xaml_result_handler_default(xaml_result hr, xaml_result_raise_level level, char const* msg) noexcept
{
#ifndef XAML_DEFAULT_HANDLER_COLOR
    if (IsDebuggerPresent())
    {
        xaml_codecvt_pool pool;
        basic_ostringstream<char, char_traits<char>, pmr::polymorphic_allocator<char>> stream(pmr::string{ pmr::polymorphic_allocator<char>{ pool.resource() } });
        print_msg(stream, hr, level, msg);
        wstring_view s = pool(stream.str());
        OutputDebugStringW(s.data());
    }
    else
#endif // !XAML_DEFAULT_HANDLER_COLOR
    {
        print_msg(boost::nowide::cerr, hr, level, msg);
    }
}

xaml_result XAML_CALL xaml_result_handler_set(function<__xaml_result_handler_prototype> const& handler) noexcept
try
{
    if (!handler)
        s_handler = XAML_DEFAULT_HANDLER;
    else
        s_handler = handler;
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

xaml_result XAML_CALL xaml_result_handler_set(xaml_result_handler handler) noexcept
try
{
    if (!handler) handler = XAML_DEFAULT_HANDLER;
    return xaml_result_handler_set(function<__xaml_result_handler_prototype>{ handler });
}
XAML_CATCH_RETURN()
