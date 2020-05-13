#include <Windows.h>
#include <shared/timer.hpp>
#include <unordered_map>
#include <xaml/result_win32.h>
#include <xaml/ui/timer.h>

using namespace std;

static unordered_map<UINT_PTR, xaml_timer_impl*> timer_map;

static void CALLBACK on_win32_timer_tick(HWND hWnd, UINT Msg, UINT_PTR nIdEvent, DWORD uElapsed)
{
    auto self = timer_map[nIdEvent];
    if (self)
    {
        self->on_tick(self);
    }
}

xaml_result xaml_timer_impl::start() noexcept
{
    if (!m_is_enabled.exchange(true))
    {
        m_id = SetTimer(nullptr, 0, (UINT)m_interval, on_win32_timer_tick);
        timer_map[m_id] = this;
    }
    return XAML_S_OK;
}

xaml_result xaml_timer_impl::stop() noexcept
{
    if (m_is_enabled.exchange(false))
    {
        XAML_RETURN_IF_WIN32_BOOL_FALSE(KillTimer(nullptr, m_id));
        timer_map.erase(m_id);
    }
    return XAML_S_OK;
}
