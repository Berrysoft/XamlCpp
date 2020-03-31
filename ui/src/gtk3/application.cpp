#include <cstdlib>
#include <gtk/gtk.h>
#include <gtk3/resources.hpp>
#include <xaml/ui/application.hpp>

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
#ifdef _MSC_VER
        char ptheme[1024];
        errno_t err = getenv_s(nullptr, ptheme, "GTK_THEME");
        string_view theme = err ? string_view{} : ptheme;
#else
        char* ptheme = getenv("GTK_THEME");
        string_view theme = ptheme ? ptheme : string_view{};
#endif // _MSC_VER

        return theme.ends_with(":dark") ? application_theme::dark : application_theme::light;
    }
} // namespace xaml
