#include <xaml/ui/container.hpp>

using namespace std;

namespace xaml
{
    container::container() : control() {}

    container::~container() {}

    multicontainer::multicontainer() : control() {}

    multicontainer::~multicontainer() {}

    void multicontainer::add_child(shared_ptr<control> const& child)
    {
        if (child)
        {
            auto it = find(m_children.begin(), m_children.end(), child);
            if (it == m_children.end())
            {
                m_children.push_back(child);
                child->set_parent(static_pointer_cast<container>(shared_from_this()));
            }
        }
    }

    void multicontainer::remove_child(shared_ptr<control> const& child)
    {
        child->set_parent(nullptr);
        auto it = find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
            m_children.erase(it);
    }
} // namespace xaml
