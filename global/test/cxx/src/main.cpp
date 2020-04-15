#include <cassert>
#include <iostream>
#include <xaml/string.hpp>
#include <xaml/vector.hpp>
#include <xaml/xaml_ptr.hpp>

#ifdef UNICODE
#ifndef _tcout
#define _tcout wcout
#endif // !_tcout
#else
#ifndef _tcout
#define _tcout cout
#endif // !_tcout
#endif // UNICODE

using namespace std;

inline void check_result(xaml_result res)
{
    assert(XAML_SUCCESS(res));
}

int main()
{
    xaml_ptr<xaml_string> str;
    check_result(xaml_string_new(U("Hello world!"), &str));
    xaml_char_t const* data;
    check_result(str->get_data(&data));
    size_t length;
    check_result(str->get_length(&length));
    _tcout << xaml_std_string_view_t(data, length) << endl;
}
