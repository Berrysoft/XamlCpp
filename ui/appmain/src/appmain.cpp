#include <boost/nowide/iostream.hpp>
#include <iostream>
#include <sf/format.hpp>
#include <xaml/ui/application.h>

#ifdef XAML_WIN32
    #include <Windows.h>
    #include <tchar.h>
#endif // XAML_WIN32

using namespace std;

static int xaml_main_end(xaml_result hr, int* pcode) noexcept
{
    if (XAML_SUCCEEDED(hr))
    {
        return *pcode;
    }
    else
    {
        sf::println(boost::nowide::cerr, U("Unhandled exception: 0x{:x}"), hr);
        return (int)hr;
    }
}

#ifdef XAML_WIN32
static xaml_result xaml_main_begin(int* pcode) noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init(&app));
    return xaml_main(app, pcode);
}

int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    int code;
    return xaml_main_end(xaml_main_begin(&code), &code);
}
#else
static xaml_result xaml_main_begin(int argc, char** argv, int* pcode) noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init_with_args(argc, argv, &app));
    return xaml_main(app, pcode);
}

int main(int argc, char** argv)
{
    int code;
    return xaml_main_end(xaml_main_begin(argc, argv, &code), &code);
}
#endif // XAML_WIN32
