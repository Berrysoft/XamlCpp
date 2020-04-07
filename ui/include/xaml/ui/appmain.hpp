#ifndef XAML_UI_APPMAIN_HPP
#define XAML_UI_APPMAIN_HPP

#include <xaml/ui/application.hpp>

#ifdef XAML_WIN32
#include <Windows.h>
#include <tchar.h>

#define XAML_APP_MAIN_START()                                 \
    INT APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT) \
    {                                                         \
        ::xaml::application::init();
#else
#define XAML_APP_MAIN_START()                   \
    int main(int argc, char const* const* argv) \
    {                                           \
        ::xaml::application::init(argc, argv);
#endif // XAML_WIN32

#define XAML_APP_MAIN_END()                       \
    return ::xaml::application::current()->run(); \
    }

#endif // !XAML_UI_APPMAIN_HPP
