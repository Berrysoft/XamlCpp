#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    control::control() {}

    void control::set_parent(shared_ptr<control> const& value)
    {
        if (m_parent != value)
        {
            if (auto multic = dynamic_pointer_cast<multicontainer>(m_parent))
            {
                multic->remove_child(shared_from_this());
            }
            else if (auto c = dynamic_pointer_cast<container>(m_parent))
            {
                c->set_child(nullptr);
            }
            if (auto multic = dynamic_pointer_cast<multicontainer>(value))
            {
                multic->add_child(shared_from_this());
            }
            else if (auto c = dynamic_pointer_cast<container>(value))
            {
                c->set_child(shared_from_this());
            }
            else
            {
                return;
            }
            m_parent = value;
            m_parent_changed(*this, value);
        }
    }

    void control::__parent_redraw()
    {
        if (get_parent())
            get_parent()->__parent_redraw();
    }

    void control::__size_to_fit() {}
} // namespace xaml
