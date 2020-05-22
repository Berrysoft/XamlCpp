#include <boost/nowide/cstdlib.hpp>
#include <gtk/gtk.h>
#include <shared/application.hpp>
#include <xaml/ui/application.h>

using namespace std;

xaml_result xaml_application_impl::init(int argc, char** argv) noexcept
{
    gtk_init(&argc, &argv);
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
    char* theme_buffer = boost::nowide::getenv("GTK_THEME");
    string_view theme = theme_buffer ? theme_buffer : string_view{};
    *ptheme = theme.ends_with(":dark") ? xaml_application_theme_dark : xaml_application_theme_light;
    return XAML_S_OK;
}
