#ifdef XAML_USE_BOOST_NOWIDE
#include <boost/nowide/args.hpp>
#include <boost/nowide/cstdlib.hpp>
#endif // XAML_USE_BOOST_NOWIDE

#include <cstdlib>
#include <gtk/gtk.h>
#include <shared/application.hpp>
#include <xaml/ui/application.h>

#ifdef XAML_WIN32
#include <Windows.h>
#endif // XAML_WIN32

#ifdef XAML_USE_BOOST_NOWIDE
#define u8getenv ::boost::nowide::getenv
#else
#define u8getenv ::std::getenv
#endif // XAML_USE_BOOST_NOWIDE

using namespace std;

xaml_result xaml_application_impl::init(int argc, char** argv) noexcept
{
    gtk_init(&argc, &argv);
#ifdef XAML_USE_BOOST_NOWIDE
    boost::nowide::args _(argc, argv);
#endif // XAML_USE_BOOST_NOWIDE
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_cmd_lines));
    for (int i = 0; i < argc; i++)
    {
        xaml_ptr<xaml_string> arg;
        XAML_RETURN_IF_FAILED(xaml_string_new_view(argv[i], &arg));
        XAML_RETURN_IF_FAILED(m_cmd_lines->append(arg));
    }
    return XAML_S_OK;
}

xaml_result xaml_application_impl::run(int* pres) noexcept
{
    gtk_main();
    *pres = m_quit_value;
    return XAML_S_OK;
}

xaml_result xaml_application_impl::quit(int value) noexcept
{
    m_quit_value = value;
    gtk_main_quit();
    return XAML_S_OK;
}

xaml_result xaml_application_impl::get_theme(xaml_application_theme* ptheme) noexcept
{
#if defined(XAML_WIN32) && !defined(XAML_USE_BOOST_NOWIDE)
    string theme(32767, L'\0');
    DWORD count = GetEnvironmentVariable("GTK_THEME", theme.data(), (DWORD)theme.length());
    theme.resize(count);
#else
    char* theme_buffer = u8getenv("GTK_THEME");
    string_view theme = theme_buffer ? theme_buffer : string_view{};
#endif // WIN32 || __MINGW32__

    *ptheme = theme.ends_with(":dark") ? xaml_application_theme_dark : xaml_application_theme_light;
    return XAML_S_OK;
}
