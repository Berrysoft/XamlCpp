#include <iostream>
#include <xaml/ui/application.h>

#ifdef XAML_WIN32
#include <Windows.h>
#include <tchar.h>
#endif // XAML_WIN32

using namespace std;

static int xaml_main_end(xaml_result hr) noexcept
{
    if (XAML_SUCCEEDED(hr))
    {
        return 0;
    }
    else
    {
        cerr << "Unhandled exception: 0x" << hex << hr << endl;
        return (int)hr;
    }
}

#ifdef XAML_WIN32
static xaml_result xaml_main_begin() noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init(&app));
    return xaml_main(app.get());
}

int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT)
{
    return xaml_main_end(xaml_main_begin());
}
#else
static xaml_result xaml_main_begin(int argc, xaml_char_t** argv) noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init_with_args(argc, argv, &app));
    return xaml_main(app.get());
}

int main(int argc, char** argv)
{
    return xaml_main_end(xaml_main_begin());
}
#endif // XAML_WIN32
