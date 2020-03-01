#include <xaml/ui/native_timer.hpp>
#include <xaml/ui/timer.hpp>

using namespace std;

namespace xaml
{
    timer::timer(chrono::milliseconds interval) : m_interval(interval)
    {
        m_handle = make_shared<native_timer>();
    }
} // namespace xaml
