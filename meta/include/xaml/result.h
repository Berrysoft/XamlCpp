#ifndef XAML_RESULT_HPP
#define XAML_RESULT_HPP

#include <stddef.h>

typedef size_t xaml_result;

#ifndef XAML_SUCCESS
#define XAML_SUCCESS(expr) (!(expr))
#endif // !XAML_SUCCESS

#ifndef XAML_FAILED
#define XAML_FAILED(expr) (expr)
#endif // !XAML_FAILED

#endif // !XAML_RESULT_HPP
