#include <cstdlib>
#include <gtk/gtk.h>
#include <gtk3/resources.hpp>
#include <xaml/ui/application.hpp>

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#endif // WIN32 || __MINGW32__

using namespace std;

namespace xaml
{
    application::application(int argc, char_t const* const* argv)
    {
        gtk_init(&argc, const_cast<char***>(&argv));
        m_cmd_lines.assign(argv, argv + argc);
    }

    int application::run()
    {
        gtk_main();
        return m_quit_value;
    }

    void application::quit(int value)
    {
        m_quit_value = value;
        gtk_main_quit();
    }

    application_theme application::get_theme() const
    {
#if defined(WIN32) || defined(__MINGW32__)
        string theme(32767, L'\0');
        DWORD count = GetEnvironmentVariable("GTK_THEME", theme.data(), (DWORD)theme.length());
        theme.resize(count);
#else
        char* ptheme = getenv("GTK_THEME");
        string_view theme = ptheme ? ptheme : string_view{};
#endif // WIN32 || __MINGW32__

        return theme.ends_with(":dark") ? application_theme::dark : application_theme::light;
    }
} // namespace xaml
