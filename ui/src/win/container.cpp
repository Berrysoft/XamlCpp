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
            if (m_children.find(child->get_handle()) == m_children.end())
            {
                m_children.emplace(child->get_handle(), child);
                child->set_parent(reinterpret_pointer_cast<container>(shared_from_this()));
            }
        }
    }

    void multicontainer::remove_child(shared_ptr<control> const& child)
    {
        m_children.erase(child->get_handle());
        child->set_parent(shared_ptr<multicontainer>{});
    }
} // namespace xaml
