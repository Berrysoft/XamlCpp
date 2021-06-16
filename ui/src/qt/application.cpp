#include <QFont>
#include <QPalette>
#include <shared/application.hpp>

xaml_result xaml_application_impl::init(int argc, char** argv) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_activate));
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_cmd_lines));
    m_argc = argc;
#ifdef XAML_UI_QT5
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    m_native_app.reset(new QApplication(m_argc, argv));
    m_native_app->setFont(QApplication::font("QMenu"));
    QObject::connect(m_native_app.get(), &QGuiApplication::lastWindowClosed, m_native_app.get(), &QCoreApplication::quit, Qt::QueuedConnection);
    for (int i = 0; i < m_argc; i++)
    {
        xaml_ptr<xaml_string> arg;
        XAML_RETURN_IF_FAILED(xaml_string_new(argv[i], &arg));
        XAML_RETURN_IF_FAILED(m_cmd_lines->append(arg));
    }
    return XAML_S_OK;
}

xaml_result xaml_application_impl::run(int* pvalue) noexcept
{
    xaml_ptr<xaml_event_args> args;
    XAML_RETURN_IF_FAILED(xaml_event_args_empty(&args));
    XAML_RETURN_IF_FAILED(m_activate->invoke(this, args));
    int result = m_native_app->exec();
    m_native_app.reset();
    *pvalue = m_quit_value != 0 ? (int)m_quit_value : result;
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
    auto back = m_native_app->palette().color(QPalette::Window);
    *ptheme = back.green() > 127 ? xaml_application_theme_light : xaml_application_theme_dark;
    return XAML_S_OK;
}
