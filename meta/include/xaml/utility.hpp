#ifndef XAML_UTILITY_HPP
#define XAML_UTILITY_HPP

#if defined(xaml_meta_EXPORTS) || defined(xaml_ui_EXPORTS) || defined(xaml_ui_controls_EXPORTS) || defined(xaml_EXPORTS) || defined(xamlc_EXPORTS)
#define XAML_EXPORTS
#endif // xaml_meta_EXPORTS || xaml_ui_EXPORTS || xaml_EXPORTS || xamlc_EXPORTS

#ifdef XAML_STATIC_DEFINE
#ifndef XAML_API
#define XAML_API
#endif // !XAML_API
#else
#ifndef XAML_API
#if defined(_MSC_VER)
#ifdef XAML_EXPORTS
#define XAML_API __declspec(dllexport)
#else // XAML_EXPORTS
#define XAML_API __declspec(dllimport)
#endif // XAML_EXPORTS
#elif defined(__GNUC__)
#ifdef XAML_EXPORTS
#define XAML_API __attribute__((visibility("default")))
#else // XAML_EXPORTS
#define XAML_API
#endif // XAML_EXPORTS
#else
#define XAML_API
#endif
#endif // !XAML_API
#endif // XAML_STATIC_DEFINE

#endif // !XAML_UTILITY_HPP
