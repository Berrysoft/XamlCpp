#include <nowide/args.hpp>
#include <nowide/iostream.hpp>
#include <sf/format.hpp>
#include <xaml/ui/application.h>

#ifdef XAML_UI_WINDOWS
    #include <combaseapi.h>
#endif // XAML_UI_WINDOWS

using namespace std;

static int xaml_main_end(xaml_result hr, int* pcode) noexcept
{
    if (XAML_SUCCEEDED(hr))
    {
        return *pcode;
    }
    else
    {
        sf::println(nowide::cerr, U("Unhandled exception: {}"), xaml_result_get_message(hr));
        return (int)hr;
    }
}

static xaml_result xaml_main_begin(int argc, char** argv, int* pcode) noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init_with_args(argc, argv, &app));
#ifdef XAML_UI_WINDOWS
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(hr))
        {
            xaml_result_raise_message(hr, xaml_result_raise_warning, "Cannot initialize COM.");
        }
    }
#endif // XAML_UI_WINDOWS
    xaml_result hr = xaml_main(app, pcode);
#ifdef XAML_UI_WINDOWS
    CoUninitialize();
#endif // XAML_UI_WINDOWS
    return hr;
}

int main(int argc, char** argv)
{
    nowide::args _(argc, argv);
    int code;
    return xaml_main_end(xaml_main_begin(argc, argv, &code), &code);
}
