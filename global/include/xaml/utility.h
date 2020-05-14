#ifndef XAML_UTILITY_HPP
#define XAML_UTILITY_HPP

#ifndef __EXPORT_IMPL
#ifdef _MSC_VER
#define __EXPORT_IMPL __declspec(dllexport)
#elif defined(__GNUC__)
#ifdef __MINGW32__
#define __EXPORT_IMPL __attribute__((dllexport))
#else
#define __EXPORT_IMPL __attribute__((visibility("default")))
#endif // __MINGW32__
#else
#define __EXPORT_IMPL
#endif // !__EXPORT_IMPL

#ifndef __IMPORT_IMPL
#ifdef _MSC_VER
#define __IMPORT_IMPL __declspec(dllimport)
#elif defined(__GNUC__) && defined(__MINGW32__)
#define __IMPORT_IMPL __attribute__((dllimport))
#else
#define __IMPORT_IMPL
#endif
#endif // !__IMPORT_IMPL

#ifdef XAML_STATIC_DEFINE
#ifndef __XAML_EXPORT
#define __XAML_EXPORT
#endif // !__XAML_EXPORT
#ifndef __XAML_IMPORT
#define __XAML_IMPORT
#endif // !__XAML_IMPORT
#else
#endif // !__EXPORT_IMPL
#ifndef __XAML_EXPORT
#define __XAML_EXPORT __EXPORT_IMPL
#endif // !__XAML_EXPORT
#ifndef __XAML_IMPORT
#define __XAML_IMPORT __IMPORT_IMPL
#endif // !__XAML_IMPORT
#endif // XAML_STATIC_DEFINE

#ifndef XAML_API
#define XAML_API __XAML_IMPORT
#endif // !XAML_API

#ifndef XAML_META_API
#define XAML_META_API __XAML_IMPORT
#endif // !XAML_META_API

#ifndef XAML_UI_API
#define XAML_UI_API __XAML_IMPORT
#endif // !XAML_UI_API

#ifndef XAML_UI_META_API
#define XAML_UI_META_API __XAML_META_IMPORT
#endif // !XAML_UI_META_API

#ifndef XAML_UI_CONTROLS_API
#define XAML_UI_CONTROLS_API __XAML_IMPORT
#endif // !XAML_UI_CONTROLS_API

#ifndef XAML_UI_CANVAS_API
#define XAML_UI_CANVAS_API __XAML_IMPORT
#endif // !XAML_UI_CANVAS_API

#ifndef XAML_UI_WEBVIEW_API
#define XAML_UI_WEBVIEW_API __XAML_IMPORT
#endif // !XAML_UI_WEBVIEW_API

#ifndef XAML_PARSER_API
#define XAML_PARSER_API __XAML_IMPORT
#endif // !XAML_PARSER_API

#ifndef XAML_COMPILER_API
#define XAML_COMPILER_API __XAML_IMPORT
#endif // !XAML_COMPILER_API

#ifndef XAML_CMDLINE_API
#define XAML_CMDLINE_API __XAML_IMPORT
#endif // !XAML_CMDLINE_API

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif // __cplusplus
#endif // !EXTERN_C

#ifndef XAML_CSTD
#ifdef __cplusplus
#define XAML_CSTD ::std::
#else
#define XAML_CSTD
#endif // __cplusplus
#endif // !XAML_CSTD

#ifndef XAML_CALL
#ifdef _MSC_VER
#define XAML_CALL __stdcall
#elif defined(__MINGW32__)
#define XAML_CALL __attribute__((__stdcall__))
#else
#define XAML_CALL
#endif // _MSC_VER
#endif // !XAML_CALL

#ifndef XAML_NOVTBL
#ifdef _MSC_VER
#define XAML_NOVTBL __declspec(novtable)
#else
#define XAML_NOVTBL
#endif // _MSC_VER
#endif // !XAML_NOVTBL

#ifndef XAML_CONST_REF
#ifdef __cplusplus
#define XAML_CONST_REF const&
#else
#define XAML_CONST_REF const*
#endif // __cplusplus
#endif // !XAML_CONST_REF

#ifndef XAML_ADDRESSOF_REF
#ifdef __cplusplus
#define XAML_ADDRESSOF_REF(x) (&x)
#else
#define XAML_ADDRESSOF_REF(x) (x)
#endif // __cplusplus
#endif // !XAML_ADDRESSOF_REF

#ifndef XAML_CONSTEXPR
#ifdef __cplusplus
#define XAML_CONSTEXPR constexpr
#else
#define XAML_CONSTEXPR inline
#endif // __cplusplus
#endif // !XAML_CONSTEXPR

#ifndef XAML_CONSTEXPR_VAR
#ifdef __cplusplus
#define XAML_CONSTEXPR_VAR inline constexpr const
#else
#define XAML_CONSTEXPR_VAR static const
#endif // __cplusplus
#endif // !XAML_CONSTEXPR_VAR

#ifndef XAML_NOEXCEPT
#ifdef __cplusplus
#define XAML_NOEXCEPT noexcept
#else
#define XAML_NOEXCEPT
#endif // __cplusplus
#endif // !XAML_NOEXCEPT

#ifdef UNICODE
typedef wchar_t xaml_char_t;
#else
typedef char xaml_char_t;
#endif // UNICODE

#ifndef U
#ifdef UNICODE
#define U(x) L##x
#else
#define U(x) x
#endif // UNICODE
#endif // !U

#endif // !XAML_UTILITY_HPP
