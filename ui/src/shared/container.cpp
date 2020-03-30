#include <xaml/ui/container.hpp>

using namespace std;

namespace xaml
{
    void multicontainer::add_child(shared_ptr<control> const& child)
    {
        if (child)
        {
            auto it = find(m_children.begin(), m_children.end(), child);
            if (it == m_children.end())
            {
                m_children.push_back(child);
                child->set_parent(shared_from_this<control>());
                if (get_handle())
                    __parent_redraw();
            }
        }
    }

    void multicontainer::remove_child(shared_ptr<control> const& child)
    {
        auto it = find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
        {
            child->set_parent({});
            m_children.erase(it);
            if (get_handle())
                __parent_redraw();
        }
    }
} // namespace xaml
