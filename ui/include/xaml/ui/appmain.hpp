#ifndef XAML_UI_APPMAIN_HPP
#define XAML_UI_APPMAIN_HPP

#include <xaml/ui/application.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#include <tchar.h>
#endif // WIN32 || __MINGW32__

#if defined(WIN32) || defined(__MINGW32__)
#define XAML_APP_MAIN_START()                                 \
    INT APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT) \
    {                                                         \
        ::xaml::application::init();
#else
#define XAML_APP_MAIN_START()                   \
    int main(int argc, char const* const* argv) \
    {                                           \
        ::xaml::application::init(argc, argv);
#endif // WIN32 || __MINGW32__

#define XAML_APP_MAIN_END()                       \
    return ::xaml::application::current()->run(); \
    }

#endif // !XAML_UI_APPMAIN_HPP
