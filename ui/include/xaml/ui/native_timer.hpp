#ifndef XAML_UI_NATIVE_TIMER_HPP
#define XAML_UI_NATIVE_TIMER_HPP

#include <xaml/ui/objc.hpp>
#include <xaml/utility.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
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
        using native_handle_type = OBJC_OBJECT(NSTimer);

        native_handle_type handle;
#endif // XAML_UI_COCOA
    };
} // namespace xaml

#endif // !XAML_UI_NATIVE_TIMER_HPP
