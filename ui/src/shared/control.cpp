#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    control::control() : meta_class() {}

    control::~control()
    {
    }

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
            else
            {
                return;
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

#ifndef XAML_UI_WINDOWS
    size control::__get_real_size() const
    {
        return get_size();
    }

    void control::__set_real_size(size value)
    {
        set_size(value);
    }

    margin control::__get_real_margin() const
    {
        return get_margin();
    }

    void control::__set_real_margin(margin const& value)
    {
        set_margin(value);
    }
#endif // !XAML_UI_WINDOWS
} // namespace xaml
