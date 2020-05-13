#ifndef XAML_INTERNAL_STREAM_HPP
#define XAML_INTERNAL_STREAM_HPP

#include <cstdio>

#ifdef _MSC_VER
#include <fstream>
#elif defined(XAML_APPLE)
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#else
#include <ext/stdio_filebuf.h>
#endif // _MSC_VER

template <typename Char, typename F, typename... Args>
decltype(auto) call_with_file_to_stream(F&& f, std::FILE* file, Args&&... args) noexcept(noexcept(
    std::forward<F>(f)(std::declval<std::basic_ostream<Char>&>(), std::forward<Args>(args)...)))
{
#ifdef _MSC_VER
    std::basic_filebuf<Char> buf{ file };
#elif defined(XAML_APPLE)
    boost::iostreams::stream_buffer<boost::iostreams::file_descriptor_sink> buf{ fileno(file), boost::iostreams::never_close_handle };
#else
    __gnu_cxx::stdio_filebuf<Char> buf{ file, std::ios_base::out };
#endif // _MSC_VER

    std::basic_ostream<Char> stream{ &buf };
    return std::forward<F>(f)(stream, std::forward<Args>(args)...);
}

#endif // !XAML_INTERNAL_STREAM_HPP
