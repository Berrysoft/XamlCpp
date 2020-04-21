#ifndef XAML_UI_SHARED_CONTAINER_HPP
#define XAML_UI_SHARED_CONTAINER_HPP

#include <shared/control.hpp>
#include <xaml/ui/container.h>

template <typename T, typename... Base>
struct xaml_container_implement : xaml_control_implement<T, Base..., xaml_container>
{
    XAML_EVENT_IMPL(child_changed)
    XAML_PROP_PTR_IMPL_BASE(child, xaml_control)
    xaml_result XAML_CALL set_child(type* value) noexcept override
    {
        if (m_child != value)
        {
            m_child = value;
            m_child->set_parent(this);
            return on_child_changed(this, m_child);
        }
        return XAML_S_OK;
    }

    xaml_container_implement() : xaml_control_implement() {}
};

template <typename T, typename... Base>
struct xaml_multicontainer_implement : xaml_control_implement<T, Base..., xaml_multicontainer>
{
protected:
    xaml_ptr<xaml_vector> m_children;

public:
    xaml_result XAML_CALL get_children(xaml_vector_view** ptr) noexcept override
    {
        m_children.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add_child(xaml_control* child) noexcept override
    {
        child->set_parent(this);
        XAML_RETURN_IF_FAILED(m_children->append(child));
        if (m_handle) XAML_RETURN_IF_FAILED(parent_redraw());
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove_child(xaml_control* child) noexcept override
    {
        child->set_parent(nullptr);
        // TODO
        if (m_handle) XAML_RETURN_IF_FAILED(parent_redraw());
        return XAML_S_OK;
    }

    xaml_multicontainer_implement() : xaml_control_implement() {}
};

#endif // !XAML_UI_SHARED_CONTAINER_HPP
