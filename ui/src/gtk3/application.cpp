#include <gtk/gtk.h>
#include <nowide/cstdlib.hpp>
#include <shared/application.hpp>
#include <xaml/ui/application.h>

using namespace std;

xaml_result xaml_application_impl::init(int argc, char** argv) noexcept
{
    m_native_impl.m_outer = this;
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_activate));
    m_native_app.reset(gtk_application_new(nullptr, G_APPLICATION_FLAGS_NONE));
    g_signal_connect(m_native_app.get(), "activate", G_CALLBACK(xaml_application_impl::on_activate_event), this);
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
    m_quit_value = g_application_run(G_APPLICATION(m_native_app.get()), 0, nullptr);
    *pres = m_quit_value;
    return XAML_S_OK;
}

xaml_result xaml_application_impl::quit(int value) noexcept
{
    m_quit_value = value;
    g_application_quit(G_APPLICATION(m_native_app.get()));
    return XAML_S_OK;
}

xaml_result xaml_application_impl::get_theme(xaml_application_theme* ptheme) noexcept
{
    char* theme_buffer = nowide::getenv("GTK_THEME");
    string_view theme = theme_buffer ? theme_buffer : string_view{};
    *ptheme = theme.ends_with(":dark") ? xaml_application_theme_dark : xaml_application_theme_light;
    return XAML_S_OK;
}

void xaml_application_impl::on_activate_event(GApplication*, xaml_application_impl* self) noexcept
{
    xaml_ptr<xaml_event_args> args;
    XAML_ASSERT_SUCCEEDED(xaml_event_args_empty(&args));
    XAML_ASSERT_SUCCEEDED(self->m_activate->invoke(self, args));
}
