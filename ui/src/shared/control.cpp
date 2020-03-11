#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    control::control() : meta_class(), m_is_visible(true)
    {
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
                __parent_redraw();
            }
        });
        add_is_visible_changed([this](control const&, bool) { if (get_handle()) draw_visible(); });
    }

    control::~control() {}

    void control::set_parent(weak_ptr<control> value)
    {
        auto sparent = m_parent.lock();
        auto svalue = value.lock();
        if (sparent != svalue)
        {
            if (auto multic = dynamic_pointer_cast<multicontainer>(sparent))
            {
                multic->remove_child(static_pointer_cast<control>(shared_from_this()));
            }
            else if (auto c = dynamic_pointer_cast<container>(sparent))
            {
                c->set_child(nullptr);
            }
            if (auto multic = dynamic_pointer_cast<multicontainer>(svalue))
            {
                multic->add_child(static_pointer_cast<control>(shared_from_this()));
            }
            else if (auto c = dynamic_pointer_cast<container>(svalue))
            {
                c->set_child(static_pointer_cast<control>(shared_from_this()));
            }
            m_parent = svalue;
            m_parent_changed(*this, *svalue);
        }
    }

    void control::__parent_redraw()
    {
        if (auto sparent = get_parent().lock())
            sparent->__parent_redraw();
    }
} // namespace xaml
