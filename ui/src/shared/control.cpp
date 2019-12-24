#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    control::control() {}

    void control::set_parent(shared_ptr<control> const& value)
    {
        if (m_parent != value && value->is_container())
        {
            if (m_parent)
            {
                if (m_parent->is_multicontainer())
                    static_pointer_cast<multicontainer>(m_parent)->remove_child(shared_from_this());
                else
                    static_pointer_cast<container>(m_parent)->set_child(nullptr);
            }
            if (value)
            {
                if (value->is_multicontainer())
                    static_pointer_cast<multicontainer>(value)->add_child(shared_from_this());
                else
                    static_pointer_cast<container>(value)->set_child(shared_from_this());
            }
            m_parent = value;
            m_parent_changed(*this, value);
        }
    }
} // namespace xaml
