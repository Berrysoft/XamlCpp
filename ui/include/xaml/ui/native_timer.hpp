#ifndef XAML_UI_NATIVE_TIMER_HPP
#define XAML_UI_NATIVE_TIMER_HPP

#include <xaml/utility.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#elif defined(XAML_UI_COCOA) && defined(__OBJC__)
#import <xaml/ui/cocoa/XamlDelegate.h>
#endif

namespace xaml
{
    struct native_timer
    {
#ifdef XAML_UI_WINDOWS
        UINT_PTR id;
        XAML_UI_API static void CALLBACK on_tick(HWND hWnd, UINT Msg, UINT_PTR nIdEvent, DWORD uElapsed);
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_COCOA
    public:
        using native_handle_type = OBJC_OBJECT(NSTimer);

    public:
        native_handle_type handle;
#endif // XAML_UI_COCOA
    };
} // namespace xaml

#endif // !XAML_UI_NATIVE_TIMER_HPP
