#ifndef XAML_STRING_H
#define XAML_STRING_H

#ifdef __cplusplus
#include <string>
#endif // __cplusplus

#include <xaml/object.h>

#ifndef U
#ifdef UNICODE
#ifdef __cplusplus
#define U(x) (char const*)u8##x
#else
#define U(x) u8##x
#endif // __cplusplus
#else
#define U(x) x
#endif // UNICODE
#endif // !U

#ifndef U_
#define U_(x) U(x)
#endif // !U_

#if defined(__cplusplus) && defined(UNICODE)
XAML_API std::wstring XAML_CALL to_wstring(std::string_view);
XAML_API xaml_result XAML_CALL to_wstring(std::string_view, std::wstring*) noexcept;
#endif // __cplusplus && UNICODE

#endif // !XAML_STRING_H
