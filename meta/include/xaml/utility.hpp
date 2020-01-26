#ifndef XAML_UTILITY_HPP
#define XAML_UTILITY_HPP

#ifdef XAML_STATIC_DEFINE
#ifndef __XAML_EXPORT
#define __XAML_EXPORT
#endif // !__XAML_EXPORT
#ifndef __XAML_IMPORT
#define __XAML_IMPORT
#endif // !__XAML_IMPORT
#else
#ifndef __XAML_EXPORT
#if defined(_MSC_VER)
#define __XAML_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define __XAML_EXPORT __attribute__((visibility("default")))
#else
#define __XAML_EXPORT
#endif
#endif // !__XAML_EXPORT
#ifndef __XAML_IMPORT
#ifdef _MSC_VER
#define __XAML_IMPORT __declspec(dllimport)
#else
#define __XAML_IMPORT
#endif
#endif // !__XAML_IMPORT
#endif // XAML_STATIC_DEFINE

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
#define XAML_UI_META_API __XAML_EXPORT
#else
#define XAML_UI_META_API __XAML_IMPORT
#endif // xaml_ui_meta_EXPORTS

#ifdef xaml_ui_controls_EXPORTS
#define XAML_UI_CONTROLS_API __XAML_EXPORT
#else
#define XAML_UI_CONTROLS_API __XAML_IMPORT
#endif // xaml_ui_controls_EXPORTS

#ifdef xaml_ui_controls_meta_EXPORTS
#define XAML_UI_CONTROLS_META_API __XAML_EXPORT
#else
#define XAML_UI_CONTROLS_META_API __XAML_IMPORT
#endif // xaml_ui_controls_meta_EXPORTS

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
