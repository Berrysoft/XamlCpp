#include "winrt/Windows.Foundation.h"
#include <xaml/ui/timer.hpp>

using namespace std;
using namespace std::chrono;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

namespace xaml
{
    XAML_API void timer::start()
    {
        if (!m_handle) m_handle = DispatcherTimer();
        m_enabled.exchange(true);
        m_handle.Interval(m_interval);
        m_handle.Start();
    }

    XAML_API void timer::stop()
    {
        if (!m_handle) m_handle = DispatcherTimer();
        m_handle.Interval(m_interval);
        m_handle.Stop();
        m_enabled.exchange(false);
    }
} // namespace xaml
