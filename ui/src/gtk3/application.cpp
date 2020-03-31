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
        GtkSettings* settings = gtk_settings_get_default();
        gchar* str;
        g_object_get(G_OBJECT(settings), "gtk-theme-name", &str);
        g_free_unique_ptr<gchar> unique_str{ str };
        string_view_t theme{ unique_str.get() };
        if (theme.ends_with("dark")) return application_theme::dark;
        return application_theme::light;
    }
} // namespace xaml
