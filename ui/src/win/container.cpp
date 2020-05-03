#include <shared/container.hpp>

using namespace std;

xaml_result xaml_multicontainer_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT* pres) noexcept
{
    optional<LPARAM> result{ nullopt };
    XAML_FOREACH_START(c, m_children);
    {
        xaml_ptr<xaml_win32_control> win32_control = c.query<xaml_win32_control>();
        if (win32_control)
        {
            LPARAM res;
            if (XAML_SUCCEEDED(win32_control->wnd_proc(msg, &res))) result = res;
        }
    }
    XAML_FOREACH_END();
    if (result)
    {
        *pres = *result;
        return XAML_S_OK;
    }
    else
        return XAML_E_NOTIMPL;
}
