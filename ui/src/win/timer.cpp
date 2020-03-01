#include <unordered_map>
#include <wil/result_macros.h>
#include <xaml/ui/native_timer.hpp>
#include <xaml/ui/timer.hpp>

using namespace std;

namespace xaml
{
    static unordered_map<UINT_PTR, timer*> timer_map;

    void native_timer::on_tick(HWND hWnd, UINT Msg, UINT_PTR nIdEvent, DWORD uElapsed)
    {
        auto self = timer_map[nIdEvent];
        if (self)
        {
            self->m_tick(*self);
        }
    }

    void timer::start()
    {
        if (!m_enabled.exchange(true))
        {
            get_handle()->id = SetTimer(NULL, 0, (UINT)m_interval.count(), native_timer::on_tick);
            timer_map[get_handle()->id] = this;
        }
    }

    void timer::stop()
    {
        if (m_enabled.exchange(false))
        {
            THROW_IF_WIN32_BOOL_FALSE(KillTimer(NULL, get_handle()->id));
            timer_map.erase(get_handle()->id);
        }
    }
} // namespace xaml
