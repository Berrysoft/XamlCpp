#include <unordered_map>
#include <wil/result_macros.h>
#include <xaml/ui/timer.hpp>

using namespace std;

namespace xaml
{
    static unordered_map<UINT_PTR, timer*> timer_map;

    XAML_API void timer::on_tick(HWND hWnd, UINT Msg, UINT_PTR nIdEvent, DWORD uElapsed)
    {
        auto self = timer_map[nIdEvent];
        if (self)
        {
            self->m_tick(*self);
        }
    }

    XAML_API void timer::start()
    {
        if (!m_enabled.exchange(true))
        {
            __set_id(SetTimer(NULL, 0, (UINT)m_interval.count(), timer::on_tick));
            timer_map[__get_id()] = this;
        }
    }

    XAML_API void timer::stop()
    {
        if (m_enabled.exchange(false))
        {
            THROW_IF_WIN32_BOOL_FALSE(KillTimer(NULL, __get_id()));
            timer_map.erase(__get_id());
        }
    }
} // namespace xaml
