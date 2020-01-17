#include "winrt/Windows.Foundation.h"
#include <xaml/ui/timer.hpp>

using namespace std;
using namespace std::chrono;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace xaml
{
    void timer::init()
    {
        if (!m_handle)
        {
            m_handle = DispatcherTimer();
            m_handle.Tick([this](IInspectable const&, IInspectable const&) -> void { m_tick(*this); });
        }
        m_handle.Interval(m_interval);
    }

    void timer::start()
    {
        init();
        m_enabled.exchange(true);
        m_handle.Start();
    }

    void timer::stop()
    {
        init();
        m_handle.Stop();
        m_enabled.exchange(false);
    }
} // namespace xaml
