#ifndef XAML_UI_SHARED_CONTAINER_HPP
#define XAML_UI_SHARED_CONTAINER_HPP

#include <shared/control.hpp>
#include <xaml/ui/container.h>

#ifdef XAML_UI_WINDOWS
#include <optional>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_container_implement : xaml_control_implement<T, Base..., xaml_container>
{
    XAML_EVENT_IMPL(child_changed)
    XAML_PROP_PTR_IMPL_BASE(child, xaml_control)
    xaml_result XAML_CALL set_child(xaml_control* value) noexcept override
    {
        if (m_child.get() != value)
        {
            m_child = value;
            m_child->set_parent(this);
            return on_child_changed(this, m_child);
        }
        return XAML_S_OK;
    }
};

template <typename T, typename... Base>
struct xaml_multicontainer_implement : xaml_control_implement<T, Base..., xaml_multicontainer>
{
protected:
    xaml_ptr<xaml_vector> m_children;

public:
    xaml_result XAML_CALL get_children(xaml_vector_view** ptr) noexcept override
    {
        return m_children->query(ptr);
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

#ifdef XAML_UI_WINDOWS
    xaml_result wnd_proc(xaml_win32_window_message const& msg, LPARAM* pres) noexcept override
    {
        try
        {
            std::optional<LPARAM> result;
            for (auto c : m_children)
            {
                xaml_ptr<xaml_win32_control> win32_control = c.query<xaml_win32_control>();
                if (win32_control)
                {
                    LPARAM res;
                    if (XAML_SUCCEEDED(win32_control->wnd_proc(msg, &res))) result = res;
                }
            }
            if (result)
            {
                *pres = *result;
                return XAML_S_OK;
            }
            else
                return XAML_E_NOTIMPL;
        }
        XAML_CATCH_RETURN()
    }
#endif // XAML_UI_WINDOWS

    xaml_result init() noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_control_implement::init());
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_children));
        return XAML_S_OK;
    }
};

#endif // !XAML_UI_SHARED_CONTAINER_HPP
