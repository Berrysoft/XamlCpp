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

#ifdef xaml_meta_EXPORTS
#define XAML_META_API __XAML_EXPORT
#else
#define XAML_META_API __XAML_IMPORT
#endif // xaml_meta_EXPORTS

#ifdef xaml_ui_EXPORTS
#define XAML_UI_API __XAML_EXPORT
#else
#define XAML_UI_API __XAML_IMPORT
#endif // xaml_ui_EXPORTS

#ifdef xaml_ui_meta_EXPORTS
#define XAML_UI_META_API __XAML_META_EXPORT
#else
#define XAML_UI_META_API __XAML_META_IMPORT
#endif // xaml_ui_meta_EXPORTS

#ifdef xaml_ui_controls_EXPORTS
#define XAML_UI_CONTROLS_API __XAML_EXPORT
#else
#define XAML_UI_CONTROLS_API __XAML_IMPORT
#endif // xaml_ui_controls_EXPORTS

#ifdef xaml_ui_controls_meta_EXPORTS
#define XAML_UI_CONTROLS_META_API __XAML_META_EXPORT
#else
#define XAML_UI_CONTROLS_META_API __XAML_META_IMPORT
#endif // xaml_ui_controls_meta_EXPORTS

#ifdef xaml_ui_canvas_EXPORTS
#define XAML_UI_CANVAS_API __XAML_EXPORT
#else
#define XAML_UI_CANVAS_API __XAML_IMPORT
#endif // xaml_ui_canvas_EXPORTS

#ifdef xaml_ui_canvas_meta_EXPORTS
#define XAML_UI_CANVAS_META_API __XAML_META_EXPORT
#else
#define XAML_UI_CANVAS_META_API __XAML_META_IMPORT
#endif // xaml_ui_canvas_meta_EXPORTS

#ifdef xaml_ui_webview_EXPORTS
#define XAML_UI_WEBVIEW_API __XAML_EXPORT
#else
#define XAML_UI_WEBVIEW_API __XAML_IMPORT
#endif // xaml_ui_webview_EXPORTS

#ifdef xaml_ui_webview_meta_EXPORTS
#define XAML_UI_WEBVIEW_META_API __XAML_META_EXPORT
#else
#define XAML_UI_WEBVIEW_META_API __XAML_META_IMPORT
#endif // xaml_ui_webview_meta_EXPORTS

#ifdef xaml_EXPORTS
#define XAML_API __XAML_EXPORT
#else
#define XAML_API __XAML_IMPORT
#endif // xaml_EXPORTS

#ifdef xamlc_EXPORTS
#define XAMLC_API __XAML_EXPORT
#else
#define XAMLC_API __XAML_IMPORT
#endif // xamlc_EXPORTS

#endif // !XAML_UTILITY_HPP
