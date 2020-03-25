#ifndef XAML_UTILITY_HPP
#define XAML_UTILITY_HPP

#ifndef __EXPORT_IMPL
#ifdef _MSC_VER
#define __EXPORT_IMPL __declspec(dllexport)
#elif defined(__GNUC__)
#define __EXPORT_IMPL __attribute__((visibility("default")))
#else
#define __EXPORT_IMPL
#endif // !__EXPORT_IMPL

#ifndef __IMPORT_IMPL
#ifdef _MSC_VER
#define __IMPORT_IMPL __declspec(dllimport)
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

#ifndef XAML_GLOBAL_API
#define XAML_GLOBAL_API __XAML_IMPORT
#endif // !XAML_GLOBAL_API

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

#endif // !XAML_UTILITY_HPP
