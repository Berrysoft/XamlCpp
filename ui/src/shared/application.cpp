#include <shared/application.hpp>
#include <xaml/ptr.hpp>

#ifdef XAML_WIN32
#include <Windows.h>
#include <cstdlib>
#include <tchar.h>
#endif // XAML_WIN32

using namespace std;

static xaml_ptr<xaml_application> s_current;

xaml_result xaml_application_init(xaml_application** ptr) noexcept
{
#ifdef XAML_WIN32
    return xaml_application_init_with_args(__argc, __targv, ptr);
#else
    return xaml_application_init_with_args(0, nullptr, ptr);
#endif // XAML_WIN32
}

xaml_result xaml_application_init_with_args(int argc, xaml_char_t** argv, xaml_application** ptr) noexcept
{
    s_current = nullptr;
    XAML_RETURN_IF_FAILED(xaml_object_init<xaml_application_impl>(&s_current, argc, argv));
    return s_current->query(ptr);
}

xaml_result xaml_application_current(xaml_application** ptr) noexcept
{
    return s_current->query(ptr);
}
