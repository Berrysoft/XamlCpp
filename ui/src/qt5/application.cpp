#include <shared/application.hpp>

xaml_result xaml_application_impl::init(int argc, char** argv) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_cmd_lines));
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    m_native_app.reset(new QApplication(argc, argv));
    for (int i = 0; i < argc; i++)
    {
        xaml_ptr<xaml_string> arg;
        XAML_RETURN_IF_FAILED(xaml_string_new(argv[i], &arg));
        XAML_RETURN_IF_FAILED(m_cmd_lines->append(arg));
    }
    return XAML_S_OK;
}

xaml_result xaml_application_impl::run(int* pvalue) noexcept
{
    *pvalue = m_quit_value != 0 ? (int)m_quit_value : m_native_app->exec();
    return XAML_S_OK;
}

xaml_result xaml_application_impl::quit(int value) noexcept
{
    m_quit_value = value;
    m_native_app->quit();
    return XAML_S_OK;
}

xaml_result xaml_application_impl::get_theme(xaml_application_theme* ptheme) noexcept
{
    *ptheme = xaml_application_theme_light;
    return XAML_S_OK;
}
