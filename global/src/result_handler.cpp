#include <iostream>
#include <map>
#include <sf/format.hpp>
#include <sf/sformat.hpp>
#include <sstream>
#include <xaml/event.h>
#include <xaml/result_handler.h>

#ifdef XAML_WIN32
#include <Windows.h>
#else
#include <sf/color.hpp>
#endif // XAML_WIN32

#ifdef UNICODE
#define _tcerr ::std::wcerr
#else
#define _tcerr ::std::cerr
#endif // UNICODE

#ifdef NDEBUG
#define XAML_DEFAULT_HANDLER xaml_result_handler_empty
#else
#define XAML_DEFAULT_HANDLER xaml_result_handler_default
#endif // NDEBUG

using namespace std;

static function<__xaml_result_handler_prototype> s_handler = XAML_DEFAULT_HANDLER;

void XAML_CALL xaml_result_raise(xaml_result hr, xaml_result_raise_level level, xaml_char_t const* file, int32_t line, xaml_char_t const* func) noexcept
{
    // If exceptions throwed here, the program should terminate.
    xaml_std_string_t msg = sf::sprint<xaml_char_t>(U("{}:{}:{}"), file, line, func);
    s_handler(hr, level, msg.c_str());
}

void XAML_CALL xaml_result_handler_empty(xaml_result, xaml_result_raise_level, xaml_char_t const*) noexcept
{
}

static map<xaml_result_raise_level, xaml_std_string_view_t> s_level_map{
    { xaml_result_raise_info, U("INFO") },
    { xaml_result_raise_warning, U("WARNING") },
    { xaml_result_raise_error, U("ERROR") }
};

#ifndef XAML_WIN32
static map<xaml_result_raise_level, sf::preset_color> s_level_color_map{
    { xaml_result_raise_info, sf::bright_blue },
    { xaml_result_raise_warning, sf::yellow },
    { xaml_result_raise_warning, sf::red }
};
#endif // !XAML_WIN32

static basic_ostream<xaml_char_t>& print_msg(basic_ostream<xaml_char_t>& stream, xaml_result hr, xaml_result_raise_level level, xaml_char_t const* msg)
{
#ifdef XAML_WIN32
    return sf::println(stream, U("{}: 0x{:x}: {}"), s_level_map[level], hr, msg);
#else
    return sf::println(stream, U("{}: 0x{:x}: {}"), sf::make_color_arg(s_level_map[level], s_level_color_map[level]), hr, msg);
#endif // XAML_WIN32
}

void XAML_CALL xaml_result_handler_default(xaml_result hr, xaml_result_raise_level level, xaml_char_t const* msg) noexcept
{
#if defined(XAML_WIN32) && !defined(XAML_MINGW)
    basic_ostringstream<xaml_char_t> stream;
    print_msg(stream, hr, level, msg);
    xaml_std_string_t s = stream.str();
    if (IsDebuggerPresent())
    {
        OutputDebugString(s.c_str());
    }
    else
    {
        WriteConsole(GetStdHandle(STD_ERROR_HANDLE), s.c_str(), (DWORD)s.size(), nullptr, 0);
    }
#else
    print_msg(_tcerr, hr, level, msg);
#endif // XAML_WIN32 && !XAML_MINGW
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
{
    if (!handler) handler = XAML_DEFAULT_HANDLER;
    return xaml_result_handler_set(function<__xaml_result_handler_prototype>{ handler });
}
