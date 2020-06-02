#ifdef XAML_WIN32
    #include <boost/nowide/args.hpp>
    #include <cstdlib>
#endif // XAML_WIN32

#include <shared/application.hpp>
#include <xaml/ptr.hpp>

using namespace std;

static xaml_ptr<xaml_application> s_current;

xaml_result XAML_CALL xaml_application_init(xaml_application** ptr) noexcept
{
#ifdef XAML_WIN32
    int argc = __argc;
    char** argv = __argv;
    boost::nowide::args _(argc, argv);
    return xaml_application_init_with_args(argc, argv, ptr);
#else
    return xaml_application_init_with_args(0, nullptr, ptr);
#endif // XAML_WIN32
}

xaml_result XAML_CALL xaml_application_init_with_args(int argc, char** argv, xaml_application** ptr) noexcept
{
    s_current = nullptr;
    XAML_RETURN_IF_FAILED(xaml_object_init<xaml_application_impl>(&s_current, argc, argv));
    return s_current->query(ptr);
}

xaml_result XAML_CALL xaml_application_current(xaml_application** ptr) noexcept
{
    return s_current->query(ptr);
}
