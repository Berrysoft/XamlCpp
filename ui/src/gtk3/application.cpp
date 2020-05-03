#include <cstdlib>
#include <gtk/gtk.h>
#include <shared/application.hpp>
#include <xaml/ui/application.h>

#ifdef XAML_WIN32
#include <Windows.h>
#endif // XAML_WIN32

using namespace std;

xaml_result xaml_application_impl::init(int argc, xaml_char_t** argv) noexcept
{
    gtk_init(&argc, &argv);
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_cmd_lines));
    for (int i = 0; i < argc; i++)
    {
        XAML_RETURN_IF_FAILED(m_cmd_lines->append(to_xaml_string(argv[i]).get()));
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
#if defined(WIN32) || defined(__MINGW32__)
    string theme(32767, L'\0');
    DWORD count = GetEnvironmentVariable("GTK_THEME", theme.data(), (DWORD)theme.length());
    theme.resize(count);
#else
    char* ptheme = getenv("GTK_THEME");
    string_view theme = ptheme ? ptheme : string_view{};
#endif // WIN32 || __MINGW32__

    *ptheme = theme.ends_with(":dark") ? xaml_application_theme_dark : xaml_application_theme_light;
    return XAML_S_OK;
}
