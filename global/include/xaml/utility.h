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

#ifndef __XAML_META_EXPORT
#define __XAML_META_EXPORT __EXPORT_IMPL
#endif // !__XAML_META_EXPORT
#ifndef __XAML_META_IMPORT
#define __XAML_META_IMPORT __IMPORT_IMPL
#endif // !__XAML_META_IMPORT

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

#ifndef XAML_UI_CONTROLS_META_API
#define XAML_UI_CONTROLS_META_API __XAML_META_IMPORT
#endif // !XAML_UI_CONTROLS_META_API

#ifndef XAML_UI_CANVAS_API
#define XAML_UI_CANVAS_API __XAML_IMPORT
#endif // !XAML_UI_CANVAS_API

#ifndef XAML_UI_CANVAS_META_API
#define XAML_UI_CANVAS_META_API __XAML_META_IMPORT
#endif // !XAML_UI_CANVAS_META_API

#ifndef XAML_UI_WEBVIEW_API
#define XAML_UI_WEBVIEW_API __XAML_IMPORT
#endif // !XAML_UI_WEBVIEW_API

#ifndef XAML_UI_WEBVIEW_META_API
#define XAML_UI_WEBVIEW_META_API __XAML_META_IMPORT
#endif // !XAML_UI_WEBVIEW_META_API

#ifndef XAML_API
#define XAML_API __XAML_IMPORT
#endif // !XAML_API

#ifndef XAMLC_API
#define XAMLC_API __XAML_IMPORT
#endif // !XAMLC_API

#ifndef XAML_CMDLINE_API
#define XAML_CMDLINE_API __XAML_IMPORT
#endif // !XAML_CMDLINE_API

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#define EXTERN_C_START \
    EXTERN_C           \
    {
#define EXTERN_C_END }
#else
#define EXTERN_C
#define EXTERN_C_START
#define EXTERN_C_END
#endif // __cplusplus
#endif // !EXTERN_C

#ifndef XAML_CALL
#ifdef _MSC_VER
#define XAML_CALL __stdcall
#else
#define XAML_CALL
#endif // _MSC_VER
#endif // !XAML_CALL

#ifndef XAML_CONST_REF
#ifdef __cplusplus
#define XAML_CONST_REF const&
#else
#define XAML_CONST_REF const*
#endif // __cplusplus
#endif // !XAML_CONST_REF

#ifndef XAML_CONSTEXPR
#ifdef __cplusplus
#define XAML_CONSTEXPR constexpr
#else
#define XAML_CONSTEXPR inline
#endif // __cplusplus
#endif // !XAML_CONSTEXPR

#ifndef XAML_CONSTEXPR_VAR
#ifdef __cplusplus
#define XAML_CONSTEXPR_VAR inline constexpr
#else
#define XAML_CONSTEXPR_VAR const
#endif // __cplusplus
#endif // !XAML_CONSTEXPR_VAR

#ifndef XAML_NOEXCEPT
#ifdef __cplusplus
#define XAML_NOEXCEPT noexcept
#else
#define XAML_NOEXCEPT
#endif // __cplusplus
#endif // !XAML_NOEXCEPT

#endif // !XAML_UTILITY_HPP