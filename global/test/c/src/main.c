#include <assert.h>
#include <stdio.h>
#include <xaml/string.h>
#include <xaml/vector.h>

#ifdef UNICODE
#ifndef _tprintf
#define _tprintf wprintf
#endif // !_tprintf
#else
#ifndef _tprintf
#define _tprintf printf
#endif // !_tprintf
#endif // UNICODE

inline void check_result(xaml_result res)
{
    assert(XAML_SUCCESS(res));
}

int main()
{
    xaml_string* str;
    check_result(xaml_string_new(U("Hello world!"), &str));
    xaml_char_t const* data;
    check_result(str->vtbl->get_data(str, &data));
    size_t length;
    check_result(str->vtbl->get_length(str, &length));
    _tprintf(U("%*s\n"), (int)length, data);
    str->vtbl->release(str);
}
