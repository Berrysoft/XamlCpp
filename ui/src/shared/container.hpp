#ifndef XAML_UI_SHARED_CONTAINER_HPP
#define XAML_UI_SHARED_CONTAINER_HPP

#include <shared/control.hpp>
#include <xaml/ui/container.h>

#ifdef XAML_UI_WINDOWS
    #include <optional>
#endif // XAML_UI_WINDOWS

struct xaml_container_internal : xaml_control_internal
{
    XAML_UI_API xaml_container_internal() noexcept;
    XAML_UI_API ~xaml_container_internal();

    XAML_EVENT_IMPL(child_changed, xaml_object, xaml_control)
    XAML_PROP_PTR_IMPL_BASE(child, xaml_control)
    xaml_result XAML_CALL set_child(xaml_control* value) noexcept
    {
        if (m_child.get() != value)
        {
            m_child = value;
            XAML_RETURN_IF_FAILED(m_child->set_parent(static_cast<xaml_control*>(m_outer_this)));
            return m_child_changed->invoke(m_outer_this, m_child);
        }
        return XAML_S_OK;
    }

    XAML_UI_API xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename Base>
struct xaml_container_implement : xaml_control_implement<T, Internal, Base>
{
    XAML_EVENT_INTERNAL_IMPL(child_changed, xaml_object, xaml_control)
    XAML_PROP_PTR_INTERNAL_IMPL(child, xaml_control)
};

struct xaml_multicontainer_internal : xaml_control_internal
{
    XAML_UI_API xaml_multicontainer_internal() noexcept;
    XAML_UI_API ~xaml_multicontainer_internal();

    xaml_ptr<xaml_vector<xaml_control>> m_children;

    xaml_result XAML_CALL get_children(xaml_vector_view<xaml_control>** ptr) noexcept
    {
        return m_children->query(ptr);
    }

    xaml_result XAML_CALL add_child(xaml_control* child) noexcept
    {
        child->set_parent(static_cast<xaml_control*>(m_outer_this));
        XAML_RETURN_IF_FAILED(m_children->append(child));
        if (m_handle) XAML_RETURN_IF_FAILED(parent_redraw());
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove_child(xaml_control* child) noexcept
    {
        child->set_parent(nullptr);
        int32_t index;
        XAML_RETURN_IF_FAILED(m_children->index_of(child, &index));
        if (index >= 0)
        {
            XAML_RETURN_IF_FAILED(m_children->remove_at(index));
            if (m_handle) XAML_RETURN_IF_FAILED(parent_redraw());
        }
        return XAML_S_OK;
    }

#ifdef XAML_UI_WINDOWS
    XAML_UI_API xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LRESULT* pres) noexcept override;
#endif // XAML_UI_WINDOWS

    XAML_UI_API xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename Base>
struct xaml_multicontainer_implement : xaml_control_implement<T, Internal, Base>
{
    XAML_PROP_INTERNAL_IMPL_BASE(children, xaml_vector_view<xaml_control>**)
    XAML_CPROP_INTERNAL_IMPL(child, xaml_control*, xaml_control*)
};

#endif // !XAML_UI_SHARED_CONTAINER_HPP
