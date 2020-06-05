#ifndef XAML_INTERNAL_STREAM_HPP
#define XAML_INTERNAL_STREAM_HPP

#include <cstdio>

#ifdef XAML_WIN32
    #include <boost/nowide/iostream.hpp>
#endif // XAML_WIN32

#ifdef _MSVC_STL_VERSION
    #include <fstream>
#elif defined(__GLIBCXX__)
    #include <ext/stdio_filebuf.h>
#else
    #include <__std_stream>
#endif

template <typename Char, typename F, typename... Args>
decltype(auto) cfile_ostream_invoke(F&& f, std::FILE* file, Args&&... args) noexcept(noexcept(
    std::forward<F>(f)(std::declval<std::basic_ostream<Char>&>(), std::forward<Args>(args)...)))
{
#ifdef XAML_WIN32
    switch (_fileno(file))
    {
    case 1:
        return std::forward<F>(f)(boost::nowide::cout, std::forward<Args>(args)...);
    case 2:
        return std::forward<F>(f)(boost::nowide::cerr, std::forward<Args>(args)...);
    }
#endif // XAML_WIN32

#ifdef _MSVC_STL_VERSION
    std::basic_filebuf<Char> buf{ file };
#elif defined(__GLIBCXX__)
    __gnu_cxx::stdio_filebuf<Char> buf{ file, std::ios_base::out };
#else
    std::mbstate_t state;
    std::__stdoutbuf<Char> buf{ file, &state };
#endif

    std::basic_ostream<Char> stream{ &buf };
    return std::forward<F>(f)(stream, std::forward<Args>(args)...);
}

template <typename Char, typename F, typename... Args>
decltype(auto) cfile_istream_invoke(F&& f, std::FILE* file, Args&&... args) noexcept(noexcept(
    std::forward<F>(f)(std::declval<std::basic_istream<Char>&>(), std::forward<Args>(args)...)))
{
#ifdef XAML_WIN32
    switch (_fileno(file))
    {
    case 0:
        return std::forward<F>(f)(boost::nowide::cin, std::forward<Args>(args)...);
    }
#endif // XAML_WIN32

#ifdef _MSVC_STL_VERSION
    std::basic_filebuf<Char> buf{ file };
#elif defined(__GLIBCXX__)
    __gnu_cxx::stdio_filebuf<Char> buf{ file, std::ios_base::in };
#else
    std::mbstate_t state;
    std::__stdinbuf<Char> buf{ file, &state };
#endif

    std::basic_istream<Char> stream{ &buf };
    return std::forward<F>(f)(stream, std::forward<Args>(args)...);
}

#endif // !XAML_INTERNAL_STREAM_HPP
