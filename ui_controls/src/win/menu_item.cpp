#include <atomic>
#include <optional>
#include <shared/menu_item.hpp>
#include <wil/result_macros.h>
#include <xaml/result_win32.h>
#include <xaml/ui/controls/menu_item.h>
#include <xaml/ui/menu_bar.h>

using namespace std;

static atomic<UINT> s_menu_item_id{ 0 };

xaml_result XAML_CALL xaml_win32_menu_item_generate_id(UINT* pid) noexcept
{
    *pid = ++s_menu_item_id;
    return XAML_S_OK;
}

xaml_result xaml_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    return draw_impl(MF_STRING);
}

xaml_result xaml_menu_item_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT*) noexcept
{
    switch (msg.Msg)
    {
    case WM_COMMAND:
    {
        if (LOWORD(msg.wParam) == m_menu_id)
        {
            XAML_RETURN_IF_FAILED(on_click(m_outer_this));
        }
        break;
    }
    }
    return XAML_E_NOTIMPL;
}

xaml_result xaml_menu_item_internal::draw_impl(UINT flags) noexcept
{
    HMENU hpmenu = nullptr;
    if (m_parent)
    {
        xaml_ptr<xaml_win32_popup_menu_item> native_menu_item;
        if (XAML_SUCCEEDED(m_parent->query(&native_menu_item)))
        {
            XAML_RETURN_IF_FAILED(native_menu_item->get_handle(&hpmenu));
        }
        else
        {
            xaml_ptr<xaml_win32_menu_bar> native_menu_bar;
            if (XAML_SUCCEEDED(m_parent->query(&native_menu_bar)))
            {
                XAML_RETURN_IF_FAILED(native_menu_bar->get_handle(&hpmenu));
            }
        }
    }
    if (hpmenu)
    {
        if (!m_menu_id)
        {
            XAML_RETURN_IF_FAILED(draw_append(hpmenu, flags));
        }
        else if (hpmenu != m_menu_parent)
        {
            XAML_RETURN_IF_WIN32_BOOL_FALSE(DeleteMenu(m_menu_parent, m_menu_id, MF_BYCOMMAND));
            m_menu_parent = hpmenu;
            XAML_RETURN_IF_FAILED(draw_append(hpmenu, flags));
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_menu_item_internal::draw_append(HMENU pmenu, UINT flags) noexcept
{
    if (!m_menu_id)
    {
        m_menu_parent = pmenu;
        XAML_RETURN_IF_FAILED(xaml_win32_menu_item_generate_id(&m_menu_id));
    }
    char const* data;
    XAML_RETURN_IF_FAILED(m_text->get_data(&data));
    XAML_RETURN_IF_WIN32_BOOL_FALSE(InsertMenu(m_menu_parent, m_menu_id, flags, m_menu_id, data));
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    return draw_impl(MF_STRING | MF_POPUP);
}

xaml_result xaml_popup_menu_item_internal::wnd_proc(xaml_win32_window_message const& msg, LRESULT* pres) noexcept
{
    std::optional<LPARAM> result;
    XAML_FOREACH_START(c, m_submenu);
    {
        xaml_ptr<xaml_win32_control> win32_control = c.query<xaml_win32_control>();
        if (win32_control)
        {
            LPARAM res;
            if (XAML_SUCCEEDED(win32_control->wnd_proc(msg, &res))) result = res;
        }
    }
    XAML_FOREACH_END();
    if (result)
    {
        *pres = *result;
        return XAML_S_OK;
    }
    else
        return XAML_E_NOTIMPL;
}

xaml_result xaml_popup_menu_item_internal::draw_submenu() noexcept
{
    XAML_FOREACH_START(child, m_submenu);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(child->query(&cc));
        XAML_RETURN_IF_FAILED(cc->draw({}));
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::draw_append(HMENU pmenu, UINT flags) noexcept
{
    if (!m_menu_id)
    {
        m_menu_parent = pmenu;
        XAML_RETURN_IF_FAILED(xaml_win32_menu_item_generate_id(&m_menu_id));
        m_menu.reset(CreateMenu());
        XAML_RETURN_IF_FAILED(draw_submenu());
    }
    char const* data;
    XAML_RETURN_IF_FAILED(m_text->get_data(&data));
    XAML_RETURN_IF_WIN32_BOOL_FALSE(InsertMenu(m_menu_parent, m_menu_id, flags, (UINT_PTR)m_menu.get(), data));
    return XAML_S_OK;
}

xaml_result xaml_check_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    return draw_impl(MF_STRING | MF_UNCHECKED);
}

xaml_result xaml_check_menu_item_internal::draw_checked() noexcept
{
    CheckMenuItem(m_menu_parent, m_menu_id, MF_BYCOMMAND | (m_is_checked ? MF_CHECKED : MF_UNCHECKED));
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    XAML_RETURN_IF_FAILED(draw_impl(MF_STRING | MF_UNCHECKED));
    MENUITEMINFO info;
    info.cbSize = sizeof(MENUITEMINFO);
    XAML_RETURN_IF_WIN32_BOOL_FALSE(GetMenuItemInfo(m_menu_parent, m_menu_id, FALSE, &info));
    info.fType |= MFT_RADIOCHECK;
    XAML_RETURN_IF_WIN32_BOOL_FALSE(SetMenuItemInfo(m_menu_parent, m_menu_id, FALSE, &info));
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw_checked() noexcept
{
    CheckMenuItem(m_menu_parent, m_menu_id, MF_BYCOMMAND | (m_is_checked ? MF_CHECKED : MF_UNCHECKED));
    return XAML_S_OK;
}

xaml_result xaml_separator_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    return draw_impl(MF_SEPARATOR);
}
