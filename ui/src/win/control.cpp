#include <Windows.h>
#include <map>
#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    control::control() {}

    void control::create(window_create_params const& params)
    {
        set_handle(CreateWindowEx(
            params.ex_style, params.class_name.c_str(), params.window_name.c_str(),
            params.style, params.x, params.y, params.width, params.height,
            params.parent ? params.parent->get_handle() : nullptr,
            nullptr, GetModuleHandle(nullptr), nullptr));
        THROW_IF_NULL_ALLOC(get_handle());
        SendMessage(get_handle(), WM_SETFONT, (WPARAM)application::current()->__default_font(), TRUE);
    }

    control::~control()
    {
        SendMessage(get_handle(), WM_CLOSE, 0, 0);
    }

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
