#include <xaml/ui/container.hpp>

using namespace std;

namespace xaml
{
    optional<LRESULT> multicontainer::__wnd_proc(window_message const& msg)
    {
        optional<LRESULT> result = nullopt;
        for (auto c : m_children)
        {
            auto r = c->__wnd_proc(msg);
            if (r) result = r;
        }
        return result;
    }
} // namespace xaml
