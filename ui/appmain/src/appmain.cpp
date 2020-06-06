#include <boost/nowide/args.hpp>
#include <boost/nowide/iostream.hpp>
#include <sf/format.hpp>
#include <xaml/ui/application.h>

using namespace std;

static int xaml_main_end(xaml_result hr, int* pcode) noexcept
{
    if (XAML_SUCCEEDED(hr))
    {
        return *pcode;
    }
    else
    {
        sf::println(boost::nowide::cerr, U("Unhandled exception: {:x8,s}"), hr);
        return (int)hr;
    }
}

static xaml_result xaml_main_begin(int argc, char** argv, int* pcode) noexcept
{
    xaml_ptr<xaml_application> app;
    XAML_RETURN_IF_FAILED(xaml_application_init_with_args(argc, argv, &app));
    return xaml_main(app, pcode);
}

int main(int argc, char** argv)
{
    boost::nowide::args _(argc, argv);
    int code;
    return xaml_main_end(xaml_main_begin(argc, argv, &code), &code);
}
